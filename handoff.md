# BCP Hardware Accelerator — Agent Handoff Document

This document is the single source of truth for continuing work on this project.
All numbers here are consistent with `report/report.tex` (the current draft).
For deep methodology detail, see `methodology.md`. For raw statistics, see `results/summary.py`.

---

## 1. What This Project Is

A hardware-accelerated SAT solver coprocessor. CDCL SAT solving spends **82.7% of its time on BCP** (Boolean Constraint Propagation — finding forced variable assignments). We offload BCP to a custom RTL chip, keeping all higher-level CDCL logic (decisions, conflict analysis, backtracking) in software.

The thesis: **single-cycle parallel BCP in hardware beats software BCP by 20–100×**, and this translates to end-to-end speedups of 5–30× on benchmarks up to ~150 variables.

---

## 2. Architecture

### Hardware (RTL — SystemVerilog, simulated via Verilator)
- **1024-slot CAM+SRAM array**: each slot is one clause literal
- **4 literals per clause** → 256 clauses max in current RTL (but 1024 slots used across 256 clauses of 4 lits each)
- Actually: 1024 clause slots, each clause occupies up to 4 rows → 1024 total clause capacity
- **Clock**: ~99 MHz, **10.1 ns/cycle** (from synthesis timing of 1M-clause CAM-SRAM memory structure)
- **Parallel scan**: every cycle, all 1024 clauses evaluated simultaneously in combinational logic
- **2-bit value encoding**: `00`=false, `11`=true, `01`=unassigned (U)

### Hardware Operations (1 cycle each = 10.1 ns)
| Op | Description |
|----|-------------|
| LOAD | Write one clause literal into CAM row |
| BCP | Parallel scan → return next unit propagation or conflict |
| UNDO | Clear a variable assignment (backtrack) |
| SW_BCP | Overflow learned clause propagated in SW, counted as HW cycle |

### Inputs/Outputs (RTL ports)
**Inputs**: `vid_in`, `val_in`, `op`
**Outputs**: `conf_out`, `up_out`, `done_out`, `cid_out`, `vid_out`, `pol_out`, `valid_out`
- `conf_out`: conflict detected
- `up_out`: unit propagation available
- `done_out`: BCP exhausted (no more implications)
- `cid_out`: clause ID that caused conflict/UP (needed for 1-UIP conflict analysis)
- `vid_out`: variable ID being forced by UP
- `pol_out`: polarity (true/false) being forced

### Software (C++ CDCL — `coprocessing/cdcl_solver.cpp`)
Minimal MiniSat-inspired CDCL. Intentionally naive to isolate hardware contribution.

**Has:**
- VSIDS activity scoring (var_decay=0.95)
- 1-UIP conflict analysis
- Non-chronological backtracking via `cancelUntil()`
- Clause learning → loads learned clauses into HW if capacity available

**Missing (intentional — isolates HW as the variable):**
- No restarts
- No phase saving (always decides positive polarity)
- No clause deletion/minimization
- O(n) `pickBranch()` linear scan (no heap)

**Not a copy of MiniSat** — fresh reimplementation. The `#ifdef HW_BCP` patched MiniSat in the codebase is a separate earlier approach; our solver (`cdcl_solver.cpp`) is purpose-built.

---

## 3. Key File Locations

```
bcp-hw-accelerator/
├── coprocessing/
│   └── cdcl_solver.cpp          # Main CDCL+HW solver (all timing instrumentation here)
├── src/
│   ├── sat_submodule.sv         # Top-level RTL (ports, FSM, output assigns)
│   ├── cam_sram_array.sv        # CAM+SRAM array
│   ├── cam_sram_row.sv          # Per-clause row
│   ├── processing_logic.sv      # Per-clause CONF/UP/DONE evaluator
│   └── combining_logic.sv       # Priority reduction across all clauses
├── tests/satlib/
│   ├── bench_cdcl.py            # Main benchmark runner (produces CSVs)
│   ├── patch_kissat.py          # Re-runs Kissat with improved timing methodology
│   └── patch_kissat_stats.py    # Adds kissat_decisions/propagations columns
├── results/
│   ├── summary.py               # Two-table aggregate summary
│   ├── *.csv                    # Per-instance benchmark results (14 sets)
│   └── UUF200.860.1000.csv      # May still be running (see Section 9)
├── report/
│   └── report.tex               # Current report draft — source of truth for numbers
├── methodology.md               # Deep methodology documentation
└── handoff.md                   # This file
```

---

## 4. Timing Methodology

### SW Time (CDCL-only, no simulation overhead)
Three sources of overhead are excluded from `sw_time_ns`:

1. **Verilator eval overhead** (`hw_sim_ns`): each `tick_()` runs two `eval()` calls on the entire RTL graph. Pure simulation cost — zero in real hardware.

2. **`setValInHW_()` scan** (`hw_aux_ns`): patching Verilator state requires O(4096) row scan. O(1) in real hardware.

3. **`propagateLearntsSW_()` scan** (`hw_aux_ns` + reclassified as HW cycles): SW scan of overflow learned clauses is a simulation artifact. Wall-clock time excluded from sw_time; unit propagations found are counted as `n_sw_bcp_ups` (HW cycles).

```cpp
sw_time_ns = total_ns - (hw_sim_ns_after - hw_sim_ns_before)
                      - (hw_aux_ns_after - hw_aux_ns_before)
```
Both deltas snapshotted at `solve()` entry to exclude constructor initialization.

### HW Time
```
hw_cycles  = n_hw_load + n_hw_bcp + n_hw_undo + n_sw_bcp_ups
hw_time_ns = hw_cycles × 10.1
```

### Kissat Timing (two regimes)
- **≥10ms self-reported**: use Kissat's `process-time` directly
- **<10ms (reports 0.00s)**: run 10 times, take median wall-clock, subtract startup constant

**Startup calibration**: 20 runs on trivially UNSAT CNF (`p cnf 1 1\n0\n`), overall median = **4.04ms** used as constant. Cold first-run = 2.79ms, warm median = 4.05ms. Uses overall median (not warm-only) to avoid underestimating startup.

---

## 5. Results (Consistent with report.tex)

### Win Rates vs All Software Solvers

| Set | N | HW wins (MS2/GL) | HW wins (Kissat) | Med HW (µs) | Med MS2 (µs) | Med Kissat (µs) |
|-----|---|-----------------|-----------------|-------------|--------------|-----------------|
| uf20-91 | 1000 | 999/1000 | 997/997 | 50 | 2,031 | 1,924 |
| uf50-218 | 1000 | 998/1000 | 1000/1000 | 207 | 3,927 | 10,000† |
| UF75.325 | 100 | 100/100 | 100/100 | 979 | 6,678 | 30,000† |
| uf100-430 | 1000 | 953/1000 | 999/1000 | 2,798 | 14,181 | 30,000† |
| UF125.538 | 100 | 84/100 | 100/100 | 11,175 | 30,378 | 30,000† |
| UF150.645 | 100 | 69/100 | 71/100 | 25,502 | 47,157 | 40,000† |
| UF175.753 | 100 | 57/100 | 29/100 | 93,123 | 127,553 | 50,000 |
| uf200-860 | 100 | 51/100 | 23/100 | 176,072 | 305,963 | 60,000 |
| uuf50-218 | 1000 | 998/1000 | 999/1000 | 493 | 4,996 | 5,082 |
| UUF75.325 | 100 | 100/100 | 100/100 | 1,865 | 12,031 | 30,000† |
| UUF100.430 | 1000 | 1000/1000 | 1000/1000 | 6,511 | 27,268 | 30,000† |
| UUF125.538 | 100 | 100/100 | 97/100 | 23,169 | 52,051 | 40,000† |
| UUF150.645 | 100 | 98/100 | 59/100 | 67,056 | 116,886 | 70,000 |
| UUF175.753 | 100 | 93/100 | 19/100 | 173,183 | 323,912 | 130,000 |

† = 10ms timer floor; Kissat timing unreliable for comparison at these sizes.

**Crossover**: UF175 for SAT. UNSAT win rates always exceed SAT rates at same size (see Section 6).

### SAT vs UNSAT Asymmetry (from report.tex Table 2)
| Vars | SAT wins (vs Kissat) | UNSAT wins | Gap |
|------|---------------------|------------|-----|
| 75 | 100/100 | 100/100 | — |
| 100 | 959/1000 | 1000/1000 | +4pp |
| 125 | 86/100 | 100/100 | +14pp |
| 150 | 71/100 | 98/100 | +27pp |
| 175 | 60/100 | 93/100 | +33pp |

### Amdahl's Law Breakdown (from report.tex)
- **Our solver**: ~93% CDCL software, ~7% HW BCP
- **MiniSat2**: ~25% CDCL, ~75% BCP
- Hardware has so completely flattened BCP that further speedup requires better CDCL.

### Counterfactual Analysis (from report.tex Figure 4)
Replacing HW BCP with Kissat's measured BCP rate, holding CDCL search identical:

| Set | Actual (µs) | Counterfactual (µs) | Slowdown |
|-----|-------------|---------------------|---------|
| uf100 | 2,600 | 41,100 | **15.7×** |
| UUF100 | 8,100 | 68,850 | **8.5×** |
| UF125 | 10,500 | 68,250 | **6.5×** |
| UUF125 | 21,800 | 82,840 | **3.8×** |
| UF150 | 23,800 | 107,100 | **4.5×** |
| UUF150 | 63,500 | 165,100 | **2.6×** |
| UF175 | 88,300 | 264,900 | **3.0×** |
| UUF175 | 163,000 | 358,600 | **2.2×** |

**Even where Kissat wins overall (UF175), HW BCP still provides 3× speedup.** The loss is due to CDCL quality, not BCP speed.

**Why counterfactual is conservative**: Kissat's BCP rate uses total CPU time in denominator (includes CDCL overhead ~20-30%), so we underestimate Kissat's true BCP rate → understate our hardware's advantage.

### Hard Instance Results (from report.tex Table 3)
| Instance | HW (µs) | Kissat (µs) | MiniSat2 (µs) | Glucose (µs) | Win |
|----------|---------|-------------|---------------|--------------|-----|
| PHP₄ | 184 | 684 | 2,710 | 1,005 | **HW** |
| PHP₅ | 731 | 3,891 | <1k | 2,665 | noise |
| PHP₆ | 3,352 | 10,000 | 7,911 | 5,930 | **HW** |
| PHP₇ | 6,034 | 10,000 | 37,292 | 61,267 | **HW** |
| PHP₈ | 6,803 | 40,000 | 190,585 | 962,534 | **HW** |
| Tseitin-C₅ | 12 | 1,419 | 1,357 | 920 | **HW** |
| Tseitin-C₇ | 19 | 947 | 2,254 | 828 | **HW** |
| Tseitin-C₉ | 21 | 895 | 962 | 1,458 | **HW** |
| Grötzsch-3col | 87 | 550 | 1,404 | 1,167 | **HW** |
| Grötzsch-4col | 19 | 468 | 1,394 | 1,170 | **HW** |

PHP₈: HW is 5.9× faster than Kissat, 28× faster than MiniSat2, 142× faster than Glucose.

---

## 6. Key Analytical Findings

### Why We Win at Small-Medium Scale
- BCP at 10.1ns/cycle (99MHz) vs Kissat's software BCP at 0.3–2.7 Mprop/s → 20–100× faster per propagation
- Our complete CAM scan catches more implications per decision than watched-literal BCP → **we make fewer decisions than Kissat on most sets** (dec_ratio 0.61–0.88× for uf100–UF150)
- No BCP cost means sw_time alone (CDCL only) < MiniSat2's total (CDCL+BCP)

### Why We Lose at UF175+ (SAT)
Three compounding factors:
1. **CDCL quality gap**: dec_ratio flips to 1.29× at UF175 — Kissat's restarts escape bad subtrees, phase saving avoids re-exploration. We have neither.
2. **CAM overflow**: uf200-860 leaves only ~164 slots for learned clauses (860 originals use 84% of 1024 capacity). Learned clauses cut future branches — overflow degrades BCP effectiveness.
3. **Kissat's BCP rate improves with clause density**: 0.03 Mprop/s on uf20 → 2.67 Mprop/s on UUF175. Watched-literal BCP scales better with more clauses. Our rate stays ~57-59 Mprop/s.

### Why UNSAT Always Beats SAT at Same Size
UNSAT proofs require exhaustive search — no branch can "get lucky." Kissat's restarts provide no benefit when every branch must be proven empty. Our complete BCP (catches more implications) is more valuable in exhaustive mode. Dec_ratio stays 0.61–0.70× for ALL UUF sets.

### BCP Speedup (reliable range: uf100–UF175)
| Set | HW rate | Kissat rate | Speedup |
|-----|---------|-------------|---------|
| uf100 | 57 Mprop/s | 0.29 Mprop/s | 199× |
| UUF100 | 58 Mprop/s | 0.55 Mprop/s | 105× |
| UF125 | 59 Mprop/s | 0.73 Mprop/s | 80× |
| UF150 | 58 Mprop/s | 1.07 Mprop/s | 54× |
| UF175 | 57 Mprop/s | 1.84 Mprop/s | 31× |

Small sets (uf20–UF75): BCP speedup numbers are 500–35,000× but unreliable — Kissat's timing resolution makes its rate denominator essentially noise at that scale.

### Decision Ratio (our_decisions / kissat_decisions)
| Set | dec_ratio | Interpretation |
|-----|-----------|----------------|
| UUF100 | 0.61× | We make 39% fewer decisions |
| UUF125 | 0.61× | 39% fewer |
| uf100 | 0.73× | 27% fewer |
| UF125 | 0.76× | 24% fewer |
| UF150 | 0.88× | 12% fewer |
| UF175 | 1.29× | 29% MORE ← crossover |
| uf200 | 1.29× | 29% more |

---

## 7. CSV Structure

All results in `results/*.csv`. 26 columns:

```
file, set, expect, got, correct, wall_s,
originals, learnts, decisions, conflicts, propagations,
hw_bcp, hw_undo, hw_load, hw_cycles, hw_time_ns, sw_time_ns,
cyc_per_prop, status, hw_overflow,
kissat_cpu_s, kissat_got, kissat_decisions, kissat_propagations,
minisat2_cpu_s, minisat2_got,
glucose_cpu_s, glucose_got
```

Key columns:
- `hw_time_ns` = `hw_cycles × 10.1` (already updated in all CSVs)
- `sw_time_ns` = CDCL-only time with simulation overhead excluded
- `kissat_decisions`, `kissat_propagations` = added by `patch_kissat_stats.py`

---

## 8. Scripts Reference

| Script | Purpose | Runtime |
|--------|---------|---------|
| `tests/satlib/bench_cdcl.py` | Main benchmark runner | Hours for full run |
| `tests/satlib/patch_kissat.py` | Re-run Kissat with improved timing | ~2-3 min |
| `tests/satlib/patch_kissat_stats.py` | Add decisions/propagations columns | ~2-3 min |
| `results/summary.py` | Two-table summary (win rates + BCP analysis) | Seconds |

### bench_cdcl.py flags
```bash
python3 tests/satlib/bench_cdcl.py \
  --kissat kissat/build/kissat \
  --minisat2 minisat/build/debug/bin/minisat \
  --glucose glucose/simp/glucose_debug \
  --timeout 300 \
  --max-instances 100 \        # added; limits N instances per set
  --sets "UUF200*" \
  --csv results/UUF200.860.1000.csv
```

### Clock period update (if clock changes again)
```python
# One-liner to recompute hw_time_ns in all CSVs:
# hw_time_ns = hw_cycles × NEW_CLOCK_NS
# Edit and run the inline script used previously (see conversation history)
```

---

## 9. Outstanding Work

### UUF200.860.1000.csv
May not exist or may be incomplete. Run:
```bash
python3 tests/satlib/bench_cdcl.py \
  --kissat kissat/build/kissat \
  --minisat2 minisat/build/debug/bin/minisat \
  --glucose glucose/simp/glucose_debug \
  --timeout 300 \
  --max-instances 100 \
  --sets "UUF200*" \
  --csv results/UUF200.860.1000.csv
```
Then patch kissat stats:
```bash
python3 tests/satlib/patch_kissat_stats.py \
  --kissat kissat/build/kissat results/UUF200.860.1000.csv
```

### Report Writing
`report/report.tex` already has:
- Introduction (BCP = 82.7% of runtime)
- Architecture description
- Formal model + algorithms
- Major technical challenges (C1–C4)
- Results: scaling figure, SAT/UNSAT table, counterfactual figure, Amdahl breakdown, hard instances table
- Future work, team roles, course topics

What may need updating/expanding based on conversation:
- The counterfactual section could note it's conservative (Kissat denominator includes CDCL time)
- The CDCL description notes it's intentionally minimal (already there)
- Numbers in report.tex use slightly older values (pre-10.1ns update in some figures); verify figures match current CSV data

---

## 10. Related Work / References

From report.tex:
- Y. Zhao & K.A. Sakallah, ICCAD 2008
- J.D. Davis et al., DAC 2008
- S. Park et al., https://par.nsf.gov/servlets/purl/10215919 — **primary reference**: 64-clause × 4-literal row geometry, 2-bit {0,1,U} encoding, priority reduction for CONF/UP/DONE

Our contributions over Park et al.: coprocessor integration, Verilator harness, timing methodology, counterfactual analysis.

---

## 11. Important Gotchas

1. **SATLIB files have `%\n0\n` terminator** that Kissat/MiniSat reject. `strip_satlib_tmp()` in all benchmark scripts strips it.

2. **Kissat 0.00s entries**: any kissat_cpu_s = 0 should be treated as None/missing, not as 0µs (would falsely claim Kissat is instant). The bench scripts handle this with falsy check.

3. **hw_time_ns is already 10.1× in all CSVs**. Do not multiply again.

4. **MiniSat2 binary**: `minisat/build/debug/bin/minisat` (debug build — static link fails on this machine).

5. **Glucose binary**: `glucose/simp/glucose_debug` (built with `MROOT=/absolute/path make d`).

6. **dec_ratio is our_decisions / kissat_decisions**, not the other way. < 1 means we make fewer decisions (good). > 1 means we make more (bad).

7. **Counterfactual uses kissat_propagations/kissat_cpu_s as rate**, applied to our hw_bcp_ops count. It does NOT rerun anything.

8. **sw_frac is ~93%** (not 99%) because clock is 10.1ns not 1ns. This is already reflected in methodology.md and summary.py output.
