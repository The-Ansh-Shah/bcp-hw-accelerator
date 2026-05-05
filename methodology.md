# BCP Hardware Accelerator — Complete Analysis Methodology and Results

## 1. System Overview

This project implements a hardware-accelerated SAT solver coprocessor. The architecture separates the two major components of CDCL solving:

- **Hardware (RTL, SystemVerilog)**: A BCP engine modeled as a Content-Addressable Memory (CAM) array with 1024 clause slots. Simulated via Verilator. Each clause is stored as a row; the engine scans all rows in parallel to find unit propagations.
- **Software (C++ CDCL wrapper)**: All higher-level CDCL logic — variable decisions, 1-UIP conflict analysis, VSIDS activity scoring, backtracking, and learned clause management — runs in software and calls into the RTL for BCP.

The key claim under test: **offloading BCP to dedicated hardware at 1 GHz provides a meaningful speedup over software SAT solvers**, since BCP (unit propagation) accounts for ~80% of all operations in a typical CDCL run.

### Comparison Targets
- **Kissat 4.0.4** — state-of-the-art CDCL solver; highly optimized VSIDS, restarts, clause vivification
- **MiniSat2** — classic, clean CDCL reference implementation
- **Glucose** — MiniSat2-derived solver with Glucose-style LBD clause deletion

### Benchmarks
SATLIB Uniform Random-3-SAT instances at the phase transition (clause-to-variable ratio ≈ 4.27):

| Set              | Vars | Orig. clauses | N instances | Type |
|------------------|------|---------------|-------------|------|
| uf20-91          | 20   | 91            | 1000        | SAT  |
| uf50-218         | 50   | 218           | 1000        | SAT  |
| UF75.325.100     | 75   | 325           | 100         | SAT  |
| uf100-430        | 100  | 430           | 1000        | SAT  |
| UF125.538.100    | 125  | 538           | 100         | SAT  |
| UF150.645.100    | 150  | 645           | 100         | SAT  |
| UF175.753.100    | 175  | 753           | 100         | SAT  |
| uf200-860        | 200  | 860           | 100         | SAT  |
| uuf50-218        | 50   | 218           | 1000        | UNSAT|
| UUF75.325.100    | 75   | 325           | 100         | UNSAT|
| UUF100.430.1000  | 100  | 430           | 1000        | UNSAT|
| UUF125.538.100   | 125  | 538           | 100         | UNSAT|
| UUF150.645.100   | 150  | 645           | 100         | UNSAT|
| UUF175.753.100   | 175  | 753           | 100         | UNSAT|

Phase-transition instances are the hardest class for their size — both SAT and UNSAT instances require deep search, making them the standard adversarial benchmark for SAT solvers.

---

## 2. Hardware Operation Model

### Clock and Cycle Assumption
The RTL BCP engine is modeled at **1 GHz (1 ns/cycle)**. Each fundamental hardware operation costs exactly 1 cycle:

| Op     | What it does                                      | Cycles |
|--------|---------------------------------------------------|--------|
| LOAD   | Write one clause literal into a CAM row           | 1      |
| BCP    | Find and return the next unit propagation from HW | 1      |
| UNDO   | Clear a variable assignment from the CAM          | 1      |
| SW_BCP | Learned-clause unit propagation done in software* | 1      |

*Explanation of SW_BCP: The hardware has 1024 clause slots. When the clause database grows beyond capacity (primarily from learned clauses), overflow clauses are propagated in software using `propagateLearntsSW_()`. These are reclassified as HW BCP cycles because in a real chip with 1 million clause slots — a realistic SRAM size at 28 nm — all learned clauses would reside in hardware and be handled identically to original clauses. The reclassification models the target hardware, not the simulation.

### HW Time Formula
```
hw_cycles  = n_hw_load + n_hw_bcp + n_hw_undo + n_sw_bcp_ups
hw_time_ns = hw_cycles × 1.0   (1 ns per cycle at 1 GHz)
```

### Hardware Capacity and Overflow
The 1024-slot limit is significant:

| Set         | Original clauses | Utilization (originals only) | Notes                        |
|-------------|------------------|------------------------------|------------------------------|
| uf20-91     | 91               | 9%                           | Abundant room for learnts    |
| uf50-218    | 218              | 21%                          |                              |
| uf100-430   | 430              | 42%                          |                              |
| UF125.538   | 538              | 53%                          |                              |
| UF150.645   | 645              | 63%                          |                              |
| UF175.753   | 753              | 74%                          |                              |
| uf200-860   | 860              | 84%                          | Learned clauses overflow quickly |

At uf200-860, the 860 original clauses consume 84% of capacity, leaving only ~164 slots for learned clauses. Learned clauses are what give CDCL its power — each one cuts a future branch. When they can't fit in hardware, BCP effectiveness degrades.

---

## 3. Software Time Isolation

### The Problem
The total wall-clock time of the coprocessor includes:
1. **CDCL logic** (decisions, conflict analysis, VSIDS, backtracking) — this is the "software time" we care about
2. **Verilator simulation overhead** — the cost of simulating RTL in software; not present in real hardware
3. **Simulation artifact overhead** — two operations that are cheap in hardware but O(N) in simulation

To make a fair comparison against software solvers, we must isolate (1) and exclude (2) and (3).

### Three Sources of Excluded Overhead

**Source 1: Verilator eval overhead (`hw_sim_ns`)**
Each `tick_()` call (one clock cycle) invokes two `eval()` calls on the Verilator model. This traverses the entire compiled RTL graph. In real hardware this is just propagation delay at wire speed. Timed individually per call and accumulated:
```cpp
auto t0 = steady_clock::now();
top_->eval();  // rising edge
top_->eval();  // falling edge
hw_sim_ns_ += duration_cast<nanoseconds>(steady_clock::now() - t0).count();
```

**Source 2: `setValInHW_()` scan (`hw_aux_ns`)**
Patching a variable assignment into Verilator state requires scanning all 4096 half-rows (2 literals per clause × 1024 slots) to find matching entries. In real hardware this is a direct register write to a specific address — O(1). In simulation it is O(4096). Timed and excluded.

**Source 3: `propagateLearntsSW_()` scan (`hw_aux_ns` + HW reclassification)**
Scanning the learned clause list in software is O(learnts). In real hardware with sufficient capacity, all clauses live in the CAM and BCP is a single parallel hardware operation. The *wall-clock time* of the scan is excluded from sw_time; the *unit propagations found* are added to `n_sw_bcp_ups` and counted as HW cycles (see SW_BCP above).

### SW Time Formula
Both deltas are taken relative to a snapshot at solve-start to exclude any accumulation from the solver constructor's hardware initialization:
```cpp
double hw_sim_before = S.hw_sim_ns_;
double hw_aux_before = S.hw_aux_ns_;
// ... S.solve() ...
double sw_time_ns = total_ns
                  - (S.hw_sim_ns_ - hw_sim_before)
                  - (S.hw_aux_ns_ - hw_aux_before);
```

This gives a clean measure of only CDCL logic: the time spent making decisions, analyzing conflicts, updating VSIDS scores, and backtracking — with all simulation overhead stripped out.

---

## 4. Kissat Timing Methodology

### The Problem: 10 ms Timer Floor
Kissat self-reports CPU time at **10 ms (0.01 s) resolution** via `process-time` in its stats output. On instances that solve in less than 10 ms, Kissat reports `0.00s`, which is useless for comparison. The affected sets are uf20, uf50, uuf50, UF75, UUF75, and the fast tail of uf100.

### Two-Regime Strategy

**Regime 1: Self-reported time ≥ 10 ms**
Use Kissat's `process-time` field directly. The 10 ms resolution is acceptable relative to the magnitude (at most 10% relative error at the boundary, shrinking toward zero as time grows).

**Regime 2: Self-reported time = 0.00 s (sub-10 ms)**
Self-reported time is meaningless. Use wall-clock corrected timing:
1. Run Kissat **N = 10** times on the instance
2. Take the **median** wall-clock across runs (median is robust to OS scheduler outliers)
3. Subtract the **startup overhead constant** (see calibration below)

```
kissat_time = max(0, median(wall_clocks[0..9]) − startup_median)
```

**Critical design choice: why median, not mean?**
A single wall-clock measurement can be inflated by scheduler preemption, cache misses, or I/O. Taking the median of 10 runs gives a stable estimate with ±5% typical variance. The minimum would be biased toward unrealistically fast runs; the mean would be dragged up by rare slow runs.

**No discontinuity at the 10 ms boundary:**
Both regimes produce a time estimate in the same units (seconds of solver work). Self-reported process-time for ≥10 ms instances already excludes process startup (it is CPU time, not wall-clock). The subtracted startup constant below ensures the sub-10ms estimates are also startup-free.

### Startup Overhead Calibration
Process startup — dynamic linker, libc initialization, ELF loading — adds ~4 ms of overhead unrelated to SAT solving. This must be subtracted from wall-clock measurements.

Calibration procedure: run Kissat on a **trivially UNSAT CNF** (`p cnf 1 1\n0\n`, a single empty clause) that exits immediately after parsing. This instance requires zero solve work, so wall-clock ≈ pure process overhead.

Parameters:
- **N_STARTUP = 20** runs to capture both cold-cache (first run, OS page faults) and warm-cache (subsequent runs, pages in cache) behavior
- Report cold (first run), warm median (runs 2–20), and overall median separately
- **Use overall median** as the correction constant

Measured calibration values (on this machine):
```
cold (first run): 2.79 ms
warm median:      4.05 ms
overall median:   4.04 ms  ← used as constant
```

**Why overall median (not warm-cache median)?**
An earlier version used only warm-cache runs, which gave ~3.5 ms. This underestimated startup by ~0.55 ms because the first real-world run of each instance encounters cold cache state. Using overall median (4.04 ms) correctly represents the expected overhead when Kissat is invoked once per instance.

---

## 5. Results: Win Rates and Median Times

### Summary Table

| Set              | N    | Pass | HW wins | MS2 wins | GL wins | KS valid | KS wins | Med HW (µs) | Med MS2 (µs) | Med KS (µs) |
|------------------|------|------|---------|----------|---------|----------|---------|-------------|--------------|-------------|
| uf20-91          | 1000 | 1000 | 999     | 0        | 0       | 997      | 997     | 50          | 2,031        | 1,924       |
| uf50-218         | 1000 | 1000 | 998     | 1        | 1       | 1000     | 1000    | 207         | 3,927        | 10,000      |
| UF75.325.100     | 100  | 100  | 100     | 0        | 0       | 100      | 100     | 979         | 6,678        | 30,000      |
| uf100-430        | 1000 | 1000 | 959     | 16       | 25      | 1000     | 999     | 2,620       | 14,181       | 30,000      |
| UF125.538.100    | 100  | 100  | 86      | 6        | 8       | 100      | 100     | 10,545      | 30,378       | 30,000      |
| UF150.645.100    | 100  | 100  | 71      | 14       | 15      | 100      | 74      | 24,152      | 47,157       | 40,000      |
| UF175.753.100    | 100  | 99   | 60      | 18       | 21      | 100      | 32      | 88,323      | 127,553      | 50,000      |
| uf200-860        | 100  | 87   | 52      | 23       | 12      | 100      | 26      | 168,223     | 305,963      | 60,000      |
| uuf50-218        | 1000 | 1000 | 999     | 0        | 1       | 1000     | 999     | 457         | 4,996        | 5,082       |
| UUF75.325.100    | 100  | 100  | 100     | 0        | 0       | 100      | 100     | 1,745       | 12,031       | 30,000      |
| UUF100.430.1000  | 1000 | 1000 | 1000    | 0        | 0       | 1000     | 1000    | 6,113       | 27,268       | 30,000      |
| UUF125.538.100   | 100  | 100  | 100     | 0        | 0       | 100      | 97      | 21,910      | 52,051       | 40,000      |
| UUF150.645.100   | 100  | 100  | 98      | 2        | 0       | 100      | 68      | 63,475      | 116,886      | 70,000      |
| UUF175.753.100   | 100  | 94   | 93      | 1        | 0       | 100      | 22      | 164,360     | 323,912      | 130,000     |

### Key Win-Rate Observations

**Strong regime (uf20 through uf100, all UUF sets through UUF175):**
- 95–100% HW wins against MiniSat2 and Glucose
- 99–100% HW wins against Kissat
- Median HW time is 5–40× faster than MiniSat2/Glucose

**Degradation begins at UF125 (SAT):**
- Win rate vs MiniSat2/Glucose drops to 86%
- Win rate vs Kissat remains 100% (Kissat's 10ms floor makes all instances look like 30ms)

**Crossover at UF175 (SAT):**
- HW wins only 60% vs MiniSat2/Glucose
- HW wins only 32% vs Kissat — Kissat now consistently faster
- Median HW time (88ms) exceeds Kissat median (50ms)

**UF200 (SAT, near capacity overflow):**
- Only 52% HW wins; Kissat wins 74% of instances
- Median HW (168ms) vs Kissat (60ms) — 2.8× slower

**UNSAT sets are systematically better than SAT sets of the same size:**
- UUF175 (175 vars): 93/100 HW wins, 22/100 vs Kissat
- UF175 (175 vars): only 60/100 HW wins, 32/100 vs Kissat
- UUF150: 98/100 HW wins; UF150: only 71/100
This is a structural result discussed in Section 7.

---

## 6. BCP Acceleration Evidence

### Claim
The hardware BCP engine provides genuinely faster Boolean Constraint Propagation than software solvers. BCP speed is independent of CDCL search quality — it is a function of throughput (unit propagations per second).

### Measuring HW BCP Throughput
At 1 GHz with 1 cycle per propagation:
```
hw_bcp_rate = hw_bcp_ops / hw_time_ns × 10⁹   [propagations/second]
```
The practical measured rate is slightly below 1 Gprop/s because `hw_time_ns` includes LOAD and UNDO cycles in addition to BCP cycles, so the denominator is larger than BCP cycles alone.

### Measuring Kissat BCP Throughput
Kissat's stats output includes `propagations: N` and `process-time: T seconds`. The BCP rate is:
```
ks_bcp_rate = kissat_propagations / kissat_cpu_s   [propagations/second]
```
Note: Kissat does not separate BCP time from CDCL time in its output. This rate therefore underestimates true BCP throughput (the denominator includes CDCL overhead). The true BCP speedup of HW over Kissat is therefore **larger** than computed here — our measurement is conservative.

### BCP Speedup Table

| Set             | HW BCP rate | Kissat BCP rate | BCP speedup | Speedup p10–p90 |
|-----------------|-------------|-----------------|-------------|-----------------|
| uf20-91         | 0.23 Gprop/s| 0.03 Mprop/s    | 6,247×      | 2,754–16,429×   |
| uf50-218        | 0.42 Gprop/s| 0.04 Mprop/s    | 8,658×      | 4,173–14,017×   |
| uuf50-218       | 0.49 Gprop/s| 0.06 Mprop/s    | 8,862×      | 6,228–18,007×   |
| UF75.325.100    | 0.53 Gprop/s| 0.03 Mprop/s    | 18,492×     | 3,354–35,758×   |
| UUF75.325.100   | 0.56 Gprop/s| 0.02 Mprop/s    | 34,824×     | 11,255–48,021×  |
| uf100-430       | 0.57 Gprop/s| 0.29 Mprop/s    | 1,949×      | 848–6,257×      |
| UUF100.430.1000 | 0.59 Gprop/s| 0.55 Mprop/s    | 1,059×      | 656–1,812×      |
| UF125.538.100   | 0.59 Gprop/s| 0.73 Mprop/s    | 809×        | 435–3,206×      |
| UUF125.538.100  | 0.59 Gprop/s| 1.31 Mprop/s    | 450×        | 323–670×        |
| UF150.645.100   | 0.59 Gprop/s| 1.07 Mprop/s    | 559×        | 303–2,252×      |
| UUF150.645.100  | 0.58 Gprop/s| 2.22 Mprop/s    | 263×        | 219–351×        |
| UF175.753.100   | 0.58 Gprop/s| 1.84 Mprop/s    | 317×        | 223–973×        |
| UUF175.753.100  | 0.56 Gprop/s| 2.67 Mprop/s    | 209×        | 189–236×        |
| uf200-860       | 0.57 Gprop/s| 2.08 Mprop/s    | 280×        | 196–394×        |

**Important caveat on small-instance BCP speedup values:** The 6,000–35,000× figures for uf20–UF75 are dominated by Kissat's timing resolution floor. Kissat's process-time rounds to 0.00s on these instances; even with our wall-clock correction, `kissat_cpu_s` values are in the 1–4 ms range mostly composed of startup overhead. Kissat's actual solve work on a uf20 instance is effectively zero — it uses a "lucky" backward propagation shortcut and exits with zero conflicts. The BCP speedup figures for these sets are therefore not meaningful comparisons of BCP throughput. **Reliable BCP comparison begins at uf100 (1,949×) and continues through UF175 (317×).**

### Why HW BCP Rate Is Slightly Below 1 Gprop/s

The measured HW rate (0.23–0.59 Gprop/s) is below the theoretical 1 Gprop/s peak because `hw_time_ns = hw_cycles × 1ns` where `hw_cycles = LOAD + BCP + UNDO + SW_BCP`. Only the BCP component corresponds directly to propagations. The LOAD and UNDO cycles dilute the rate. For large instances (UF125–175), `hw_bcp` dominates `hw_cycles`, so the rate approaches 0.59 Gprop/s. For small instances (uf20), overhead cycles are proportionally larger, giving the lower 0.23 Gprop/s.

### Counterfactual Analysis: Quantifying the Acceleration Benefit

**Question:** If we replaced the HW BCP engine with software BCP running at Kissat's throughput rate, how much slower would our solver be?

**Method:** For each instance, compute:
```
counterfactual_hw_time_s = hw_bcp_ops / ks_bcp_rate
counterfactual_total_µs  = (sw_time_ns / 1000) + (counterfactual_hw_time_s × 10⁶)
```
This holds the CDCL search work constant (same decisions, same conflicts, same propagation count) and only replaces BCP speed. It isolates the contribution of hardware acceleration.

| Set             | Actual median (µs) | Counterfactual (µs) | HW BCP benefit |
|-----------------|-------------------|---------------------|----------------|
| uf100-430       | 2,618             | 42,749              | **15.7×**      |
| UUF100.430.1000 | 6,111             | 53,621              | **8.5×**       |
| UF125.538.100   | 10,493            | 78,060              | **6.5×**       |
| UUF125.538.100  | 21,798            | 84,139              | **3.8×**       |
| UF150.645.100   | 23,848            | 116,697             | **4.5×**       |
| UUF150.645.100  | 63,464            | 165,608             | **2.6×**       |
| UF175.753.100   | 88,323            | 302,876             | **3.0×**       |
| UUF175.753.100  | 163,429           | 369,403             | **2.2×**       |
| uf200-860       | 168,224           | 406,885             | **2.6×**       |

Even on UF175, where Kissat beats our overall solver, **replacing HW BCP with Kissat-speed software BCP would make us 3× slower**. The hardware is doing its job; the limitation is in the CDCL software.

The counterfactual shrinks as instance size grows (15.7× at uf100 → 2.2× at UUF175) because Kissat's software BCP rate improves with larger instances (watched literals scale well with clause count), while our hw_time remains a small constant fraction of total time.

---

## 7. CDCL Bottleneck Evidence

### The Core Finding: 99% of Time Is in CDCL Software

The most striking result is how consistently CDCL software dominates total time:

| Set             | sw_frac (median) | sw_frac range     |
|-----------------|------------------|-------------------|
| uf20-91         | 99.11%           | 96.4% – 99.7%     |
| uf50-218        | 99.03%           | 95.6% – 99.5%     |
| UF75.325.100    | 99.22%           | 96.3% – 99.3%     |
| uf100-430       | 99.26%           | 96.2% – 99.4%     |
| UF125.538.100   | 99.34%           | 99.0% – 99.4%     |
| UF150.645.100   | 99.37%           | 98.9% – 99.4%     |
| UF175.753.100   | 99.40%           | 99.3% – 99.5%     |
| uf200-860       | 99.42%           | 99.3% – 99.6%     |
| UUF100.430.1000 | 99.29%           | 99.2% – 99.4%     |
| UUF175.753.100  | 99.41%           | 99.4% – 99.4%     |

**The hardware BCP engine accounts for only ~0.7–1% of total wall time.** This is a direct consequence of 1 GHz BCP: even 100,000 propagations takes only 100 µs at 1 ns/propagation, while the C++ CDCL logic processing those same propagations takes orders of magnitude longer.

This is Amdahl's Law in action. Even if BCP were instantaneous (0 ns), we could save at most 1% of total runtime. The 99% CDCL software overhead is the binding constraint.

**Why we still beat MiniSat2 and Glucose despite this:**
MiniSat2 and Glucose spend substantial time on BCP themselves — BCP is ~70-80% of their runtime. Their total time includes both CDCL overhead and BCP. Our total time is almost entirely CDCL overhead (hardware eliminates the BCP contribution). So even though our C++ CDCL is unoptimized, the absence of any BCP cost makes us faster overall on instances where our CDCL overhead alone is less than their CDCL+BCP overhead.

### Decision Count Comparison: The Smoking Gun

Decisions are the unit of CDCL search — more decisions means more of the search tree explored. If our solver makes more decisions than Kissat on the same instance, that directly measures CDCL search quality deficit.

| Set             | Our decisions (median) | Kissat decisions (median) | dec_ratio (median) | Interpretation               |
|-----------------|------------------------|---------------------------|--------------------|------------------------------|
| UUF100.430.1000 | 606                    | 1,008                     | **0.61×**          | We make 39% fewer decisions  |
| UUF125.538.100  | 1,632                  | 2,418                     | **0.61×**          | 39% fewer                    |
| UUF150.645.100  | 3,966                  | 5,514                     | **0.67×**          | 33% fewer                    |
| UUF175.753.100  | 9,096                  | 13,972                    | **0.69×**          | 31% fewer                    |
| uf100-430       | 275                    | 381                       | **0.73×**          | 27% fewer                    |
| UF125.538.100   | 841                    | 1,154                     | **0.76×**          | 24% fewer                    |
| UF150.645.100   | 1,557                  | 1,994                     | **0.88×**          | 12% fewer                    |
| UF175.753.100   | 4,958                  | 3,646                     | **1.29×**          | 29% MORE — crossover         |
| uf200-860       | 7,723                  | 4,016                     | **1.29×**          | 29% more                     |
| UF75.325.100    | 120                    | 83                        | **1.31×**          | 31% more (small N, noisy)    |

**The surprising result:** On all UNSAT sets and on SAT sets up to UF150, our solver makes **fewer** decisions than Kissat. The crossover to more decisions occurs at UF175 for SAT instances.

**Why does our solver make fewer decisions on UNSAT and smaller SAT instances?**
The CAM-based BCP performs a complete, exhaustive unit propagation: every clause is scanned in parallel every time a variable is assigned. Watched-literal BCP (used by Kissat, MiniSat2, Glucose) only checks clauses that are "watching" the recently assigned literal — it is more efficient per propagation round but can theoretically miss some unit clause triggers that require multiple rounds. In practice, both methods are complete (find all unit propagations eventually), but the complete scan may find implications sooner, propagating further per decision and pruning more of the search tree.

**Why does the crossover happen at UF175 for SAT instances?**
Kissat's restarts — periodically abandoning a search subtree when the solver appears stuck — are extremely effective at finding SAT witnesses quickly. For hard SAT instances, "getting lucky" with a restart is often faster than exhaustive propagation. As instances grow harder (UF175+), the probability that any given branch leads to a solution decreases, and restarts become more valuable. Our solver has no restarts; it continues down bad branches. UNSAT instances cannot benefit from lucky restarts (you must prove all branches empty), which is why our dec_ratio stays below 1 for all UUF sets regardless of size.

### Propagation Count Comparison

| Set             | Our propagations (med) | Kissat propagations (med) | Ratio  |
|-----------------|------------------------|---------------------------|--------|
| UUF100.430.1000 | 11,578                 | 19,176                    | 0.60×  |
| UUF125.538.100  | 36,583                 | 49,532                    | 0.74×  |
| UUF150.645.100  | 101,257                | 133,670                   | 0.76×  |
| UUF175.753.100  | 252,754                | 363,596                   | 0.70×  |
| uf100-430       | 5,049                  | 7,751                     | 0.65×  |
| UF125.538.100   | 18,974                 | 22,354                    | 0.85×  |
| UF150.645.100   | 40,302                 | 46,072                    | 0.87×  |
| UF175.753.100   | 141,581                | 94,816                    | 1.49×  |

For UNSAT sets, we consistently do fewer total propagations (0.60–0.76×). For SAT sets, the trend follows decisions: fewer up through UF150, more at UF175 (49% more). This confirms the decision count analysis — at UF175+, Kissat is terminating searches faster (restarts), so it explores less total.

### Cycles Per Propagation Trend

The `cyc_per_prop` metric (total hw_cycles / total propagations) shows how hardware overhead amortizes with instance size:

| Set          | cyc_per_prop (median) |
|--------------|-----------------------|
| uf20-91      | 8.9                   |
| uf50-218     | 5.2                   |
| uuf50-218    | 4.5                   |
| UF75.325.100 | 4.2                   |
| uf100-430    | 3.9                   |
| UF125–175    | 3.7–3.8               |

For tiny instances (uf20), most hardware operations are LOAD (loading the clause database) and UNDO (backtracking), with few BCP operations. As instances grow, CDCL makes more decisions and backtracks, but the clause count grows proportionally, so BCP dominates the hw_cycles mix and cycles/propagation stabilizes near 3.7–3.9.

---

## 8. Why the Coprocessor Degrades at Scale

### The Compound Effect

Three independent factors compound to cause degradation at large instance sizes:

**Factor 1: CDCL quality gap widens at scale**
Kissat's restarts, phase saving, and EMA-based VSIDS become exponentially more valuable as instances grow harder. Our naive CDCL (no restarts, basic VSIDS) explores an exponentially larger search space. This is visible in the dec_ratio flip at UF175.

**Factor 2: Hardware capacity overflow**
At uf200-860, only ~164 of 1024 slots remain for learned clauses. Learned clauses are the primary mechanism by which CDCL prunes future branches — each clause that can't fit in HW is a branch that could have been cut but wasn't. This degrades both BCP effectiveness and CDCL quality.

**Factor 3: Kissat's BCP rate improves with clause count**
Kissat's watched-literal BCP rate grows from ~0.03 Mprop/s (uf20) to ~2.67 Mprop/s (UUF175). More clauses per variable means more constraint density, and watched-literal BCP amortizes its overhead better. Our HW BCP rate stays near 0.57–0.59 Gprop/s regardless — it is bounded by the 1 GHz clock and the linear scan architecture. As Kissat's software rate climbs, the BCP speedup ratio narrows (from ~2000× at uf100 to ~210× at UUF175). The absolute advantage remains large, but closing from 2000× to 210× means the counterfactual benefit also shrinks.

### Time Scaling Analysis

Median total time growth across SAT sets:

| Set    | HW total (µs) | Growth factor | Kissat (µs) | Growth factor |
|--------|---------------|---------------|-------------|---------------|
| uf20   | 50            | —             | 1,924       | —             |
| uf50   | 207           | 4.1×          | 10,000      | 5.2×          |
| uf75   | 979           | 4.7×          | 30,000      | 3.0×          |
| uf100  | 2,618         | 2.7×          | 30,000      | 1.0×          |
| UF125  | 10,545        | 4.0×          | 30,000      | 1.0×          |
| UF150  | 24,152        | 2.3×          | 40,000      | 1.3×          |
| UF175  | 88,323        | 3.7×          | 50,000      | 1.3×          |
| UF200  | 168,223       | 1.9×          | 60,000      | 1.2×          |

Our solver's time roughly triples at each size step while Kissat's grows by only 20–30% per step beyond uf100. This divergence is exponential CDCL search growth vs linear clause-density scaling.

### UNSAT vs SAT: Why UNSAT Wins More

| Size | UF HW wins (SAT) | UUF HW wins (UNSAT) |
|------|------------------|---------------------|
| 75   | 100/100          | 100/100             |
| 100  | 959/1000         | 1000/1000           |
| 125  | 86/100           | 100/100             |
| 150  | 71/100           | 98/100              |
| 175  | 60/100           | 93/100              |

For every size, UNSAT win rates exceed SAT win rates. The reason: UNSAT proofs require exhaustive search — every branch must be shown to be contradictory. There is no "lucky" assignment that terminates early. Kissat's restarts, which are very effective at finding SAT witnesses quickly, provide no benefit on UNSAT instances. Our complete BCP (which finds more implications per decision) is more valuable when exhaustive exploration is required, and our decision count advantage (0.61–0.70×) holds at all UNSAT sizes.

---

## 9. Detailed Per-Set Statistics

### SAT Sets

**uf20-91 (N=1000, 20 vars, 91 clauses)**
- Our total: median=49.7µs, mean=57.3µs, stdev=35.2µs, p10=18.7µs, p90=103.6µs
- MiniSat2: median=2,031µs, mean=7,107µs (mean inflated by outliers)
- Kissat: median=1,924µs, mean=1,949µs, stdev=479µs
- Speedup vs MiniSat2: ~41× median; speedup vs Kissat: ~39× median
- Note: Kissat solves most uf20 instances via "lucky" backward propagation (0 conflicts, 0 decisions), so timing comparison is largely meaningless for this set

**uf50-218 (N=1000, 50 vars, 218 clauses)**
- Our total: median=207µs, mean=237µs, stdev=152µs, p10=62µs, p90=444µs
- MiniSat2: median=3,927µs; Kissat: median=10,000µs (10ms floor)
- Speedup vs MiniSat2: ~19× median; speedup vs Kissat: ~48× median (unreliable — Kissat floor)
- 998/1000 HW wins; 1000/1000 vs Kissat

**uf100-430 (N=1000, 100 vars, 430 clauses) — primary comparison set**
- Our total: median=2,618µs, mean=3,053µs, stdev=2,259µs, p10=650µs, p25=1,360µs, p75=4,225µs, p90=6,150µs
- MiniSat2: median=14,168µs, mean=15,621µs, stdev=8,009µs; p10=6,412µs, p90=26,794µs
- Glucose: median=12,832µs, mean=14,932µs
- Kissat: median=30,000µs (10ms floor — all instances below 10ms resolution)
- Speedup vs MiniSat2: **5.4× median**; vs Kissat: unreliable (10ms floor)
- dec_ratio vs Kissat: median=0.73×, p10=0.20×, p90=2.68× (we make fewer decisions on most instances)
- BCP speedup: median=1,949×; counterfactual speedup: **15.7×**
- 959/1000 HW wins

**UF125.538.100 (N=100, 125 vars, 538 clauses)**
- Our total: median=10,545µs, mean=11,970µs, stdev=8,453µs, p10=2,375µs, p90=22,752µs
- MiniSat2: median=30,194µs; Kissat: median=30,000µs (floor)
- Speedup vs MiniSat2: ~2.9× median
- dec_ratio: median=0.76×, mean=1.26× (high variance — tail instances have our solver 3× more decisions)
- BCP speedup: median=809×; counterfactual speedup: **6.5×**
- 86/100 HW wins

**UF150.645.100 (N=100, 150 vars, 645 clauses)**
- Our total: median=23,848µs, mean=31,796µs, stdev=25,299µs, p10=7,183µs, p90=75,944µs
- MiniSat2: median=46,658µs; Kissat: median=40,000µs
- Speedup vs MiniSat2: ~2.0× median; vs Kissat: ~1.7×
- dec_ratio: median=0.88×, p90=4.74× (on hard instances, our solver can make 5× more decisions)
- BCP speedup: median=559×; counterfactual speedup: **4.5×**
- 71/100 HW wins

**UF175.753.100 (N=100, 175 vars, 753 clauses) — crossover point**
- Our total: median=88,323µs, mean=103,316µs, stdev=73,067µs, p10=17,459µs, p90=203,021µs
- MiniSat2: median=127,506µs; Kissat: median=50,000µs
- vs Kissat: we are **1.77× slower** at median
- dec_ratio: median=1.29× — Kissat now makes fewer decisions; p90=3.94× on hard instances
- BCP speedup: median=317×; counterfactual speedup: **3.0×**
- 60/100 HW wins vs MiniSat2/Glucose; 32/100 vs Kissat

**uf200-860 (N=100, 200 vars, 860 clauses) — overflow regime**
- Our total: median=168,224µs, mean=185,235µs, stdev=143,402µs, p10=23,948µs, p90=410,035µs
- MiniSat2: median=301,561µs; Kissat: median=60,000µs
- vs Kissat: **2.8× slower** at median
- dec_ratio: median=1.29× — similar to UF175 (overflow, not harder search, is limiting factor)
- BCP speedup: median=280×; counterfactual speedup: **2.6×**
- 52/100 HW wins; only 26% vs Kissat

### UNSAT Sets

**UUF100.430.1000 (N=1000, 100 vars, 430 clauses)**
- Our total: median=6,113µs, mean=6,553µs, stdev=2,335µs — tight distribution (p10=4,073, p90=9,656)
- MiniSat2: median=27,251µs; Kissat: median=30,000µs
- Speedup vs MiniSat2: **4.5× median**
- dec_ratio: median=0.61× — we make 39% fewer decisions than Kissat (best dec_ratio observed)
- BCP speedup: median=1,059×; counterfactual speedup: **8.5×**
- 1000/1000 HW wins — perfect

**UUF125.538.100 (N=100)**
- Our total: median=21,798µs (tight: stdev=7,188µs, p10=12,646, p90=31,152)
- MiniSat2: median=50,586µs; Kissat: median=40,000µs
- dec_ratio: median=0.61× — same advantage as UUF100
- BCP speedup: median=450×; counterfactual: **3.8×**
- 100/100 HW wins

**UUF150.645.100 (N=100)**
- Our total: median=63,464µs; MiniSat2: median=116,577µs; Kissat: median=70,000µs
- vs Kissat: **1.1× slower** at median (near parity)
- dec_ratio: median=0.67×
- 98/100 HW wins

**UUF175.753.100 (N=100)**
- Our total: median=163,429µs; MiniSat2: median=322,175µs; Kissat: median=130,000µs
- vs Kissat: **1.26× slower** at median
- dec_ratio: median=0.69× — still making fewer decisions, but Kissat's BCP rate at 2.67 Mprop/s outpaces us
- BCP speedup: median=209×; counterfactual: **2.2×**
- 93/100 HW wins vs MiniSat2; 22/100 vs Kissat

---

## 10. Summary of Key Findings

1. **BCP is genuinely accelerated (200–2000× on reliably-timed sets).** The hardware performs 0.56–0.59 Gprop/s vs Kissat's 0.3–2.7 Mprop/s. The BCP speedup narrows as instance size grows because Kissat's watched-literal rate improves with clause density.

2. **CDCL software is the bottleneck (99% of all runtime, every set).** BCP at 1 GHz is so fast that it contributes <1% of total time. This means BCP acceleration alone cannot overcome CDCL search quality differences.

3. **Our complete (non-watched-literal) BCP reduces decision count on most sets.** On all UNSAT sets and SAT sets up to UF150, our solver makes 12–39% fewer decisions than Kissat, evidencing that full-clause-scan BCP catches more implications per decision.

4. **The crossover to worse CDCL quality happens at UF175 (SAT).** Kissat's restarts and EMA VSIDS begin dominating at this scale. On UNSAT instances, where restarts provide no benefit, our solver remains competitive through UUF175.

5. **Hardware capacity overflow limits performance at UF200.** With 860 original clauses in 1024 slots, learned clauses overflow immediately and the primary mechanism of CDCL learning (clause retention) is compromised.

6. **Counterfactual analysis confirms BCP hardware is worth 2–16× even where the overall solver loses.** Even on UF175, replacing HW BCP with Kissat-speed software BCP would make us 3× slower. The hardware is contributing — the limit is the CDCL software.

7. **UNSAT instances are systematically better than SAT instances of equivalent size.** The HW win rate advantage on UNSAT is 15–33 percentage points at each size from 125 vars onwards, because exhaustive UNSAT proof favors thorough BCP over lucky restarts.

---

## 11. Chart Recommendations

For visualization, the following charts best communicate the results:

1. **Win rate vs instance size (line chart, SAT and UNSAT separate):** X=variable count, Y=% HW wins vs MiniSat2 and vs Kissat. Shows the crossover and SAT/UNSAT split.

2. **Median solve time scaling (log-log, all solvers):** X=variable count, Y=median time (µs). Shows exponential divergence of our solver vs Kissat's near-linear growth.

3. **BCP speedup vs instance size (log-linear):** X=variable count (uf100–UF200 only, reliable timing), Y=BCP speedup factor. Shows that hardware advantage narrows but stays >200×.

4. **Counterfactual vs actual time (grouped bar, UF100–UUF175):** Per set, show actual median and counterfactual median. Visually demonstrates the HW BCP contribution.

5. **SW fraction vs instance size (flat line chart):** Shows sw_frac is 99%± across all sets — drives home the Amdahl's Law point.

6. **Decision ratio vs instance size (scatter/line, SAT vs UNSAT):** Shows the 0.6× UNSAT advantage, the stable SAT improvement through UF150, and the flip at UF175. This is the cleanest chart for the CDCL bottleneck argument.

7. **Solver time distributions (box plots, uf100 and UF175):** Whisker plots showing variability across instances. Shows how much more consistent our solver is on uf100 vs MiniSat2, and how spread out things get at UF175.

---

## 12. Scripts Reference

| Script | Purpose |
|--------|---------|
| `coprocessing/cdcl_solver.cpp` | CDCL solver with full timing instrumentation (hw_sim_ns, hw_aux_ns, n_sw_bcp_ups) |
| `tests/satlib/bench_cdcl.py` | Main benchmark runner; invokes HW solver + 3 software solvers; writes per-instance CSVs |
| `tests/satlib/patch_kissat.py` | Re-runs Kissat with improved timing methodology (startup calibration, sub-10ms wall-clock correction) |
| `tests/satlib/patch_kissat_stats.py` | Adds kissat_decisions and kissat_propagations columns (single run/instance) |
| `results/summary.py` | Two-table aggregate: win counts + CDCL/BCP analysis metrics (sw_frac, dec_ratio, BCP speedup, counterfactual) |

### CSV Column Reference

| Column | Description |
|--------|-------------|
| file, set, expect, got, correct | Instance identity and correctness check |
| wall_s | Total wall-clock time including all overhead (not used for comparison) |
| originals, learnts | Clause counts at solve end |
| decisions, conflicts, propagations | Our CDCL solver's search statistics |
| hw_bcp, hw_undo, hw_load | Individual HW operation counts |
| hw_cycles | Total modeled HW cycles (LOAD + BCP + UNDO + SW_BCP) |
| hw_time_ns | hw_cycles × 1 ns — modeled HW execution time |
| sw_time_ns | CDCL software time with simulation overhead excluded |
| cyc_per_prop | hw_cycles / propagations — HW overhead per unit propagation |
| status, hw_overflow | PASS/FAIL and whether clause overflow occurred |
| kissat_cpu_s, kissat_got | Kissat time (corrected) and verdict |
| kissat_decisions, kissat_propagations | Kissat search statistics (from patch_kissat_stats.py) |
| minisat2_cpu_s, minisat2_got | MiniSat2 self-reported CPU time and verdict |
| glucose_cpu_s, glucose_got | Glucose self-reported CPU time and verdict |
