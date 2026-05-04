// CDCL SAT solver driving the Verilated sat_submodule RTL for BCP.
//
// Architecture mirrors MiniSat (github.com/niklasso/minisat), except the
// inner BCP kernel is replaced by the RTL model: each propagation step is
// an OP_BCP pulse to the Verilated DUT, and conflict/unit-prop events are
// read back from conf_out/up_out/cid_out/vid_out/pol_out. Decision
// heuristics (VSIDS), 1-UIP conflict analysis, clause learning, and
// backtracking all run in software.
//
// Hardware limits (fixed by the RTL elaboration in this directory):
//   - 1024 clauses max (NUM_CLAUSES = 1024, covers all SATLIB instances
//     up to ~250 originals + learned clauses)
//   - up to 4 literals per clause (shorter clauses are loaded partially;
//     the behavioral RTL treats unloaded rows as always-false so no padding
//     literals are needed)

#include "Vsat_solver.h"
#include "Vsat_solver_sat_submodule.h"
#include "verilated.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef int Lit;  // DIMACS: +v = positive literal of var v; -v = negative.

// RTL op codes (sat_submodule.sv)
static const int OP_IDLE = 0;
static const int OP_LOAD = 1;
static const int OP_BCP  = 2;
static const int OP_UNDO = 3;

// RTL value encoding (matches sat_submodule.sv / hw_bcp_defs.vh):
//   VAL_ZERO = 2'b00 = 0
//   VAL_ONE  = 2'b11 = 3
//   VAL_U    = 2'b01 = 1
static const int VAL_ZERO = 0;
static const int VAL_ONE  = 3;
static const int VAL_U    = 1;

// Built RTL dimensions (must match Verilator elaboration parameters)
static const int HW_NUM_CLAUSES     = 1024;
static const int HW_LITS_PER_CLAUSE = 4;
static const int HW_NUM_ROWS        = HW_LITS_PER_CLAUSE * HW_NUM_CLAUSES;

// Three-valued truth
static const uint8_t L_FALSE = 0;
static const uint8_t L_TRUE  = 1;
static const uint8_t L_UNDEF = 2;

static inline int lit_var(Lit l) { return l > 0 ? l : -l; }
static inline int lit_pol(Lit l) { return l > 0 ? 0 : 1; }

// Reason encoding:
//   r == -1         : decision (no antecedent)
//   r >= 0          : index into `originals`
//   r <= -2         : learnt index = -2 - r
static inline bool reason_is_learnt(int r) { return r <= -2; }
static inline int  learnt_of(int r)        { return -2 - r; }
static inline int  encode_learnt(int li)   { return -2 - li; }

static uint64_t main_time = 0;
double sc_time_stamp() { return static_cast<double>(main_time); }

struct Clause {
    vector<Lit> lits;
};

class Solver {
public:
    Solver() : dut_(new Vsat_solver()) { reset_hw_(); }
    ~Solver() { dut_->final(); delete dut_; }

    // ── Public state ─────────────────────────────────────────────
    int num_vars = 0;
    vector<Clause>  originals;
    vector<Clause>  learnts;
    vector<uint8_t> learnt_in_hw;  // 1 if learnt clause is loaded faithfully into HW
    vector<uint8_t> assigns;      // size num_vars+1
    vector<int>     level;
    vector<int>     reason_;
    vector<double>  activity;
    vector<Lit>     trail;
    vector<int>     trail_lim;
    int             qhead = 0;
    double          var_inc = 1.0;
    double          var_decay = 0.95;

    // n_orig_ is frozen after postLoad_(): distinguishes original CIDs from
    // learnt CIDs when decoding cid_out returned by the HW.
    int             n_orig_ = 0;

    uint64_t n_decisions = 0;
    uint64_t n_conflicts = 0;
    uint64_t n_propagations = 0;
    uint64_t n_hw_bcp = 0;
    uint64_t n_hw_undo = 0;
    uint64_t n_hw_load = 0;
    bool     unsat_at_load = false;

    // ── Public API ───────────────────────────────────────────────
    void setNumVars(int n) {
        num_vars = n;
        assigns.assign(n + 1, L_UNDEF);
        level.assign(n + 1, -1);
        reason_.assign(n + 1, -1);
        activity.assign(n + 1, 0.0);
    }

    int decisionLevel() const { return static_cast<int>(trail_lim.size()); }

    bool lit_true_(Lit l) const {
        uint8_t a = assigns[lit_var(l)];
        if (a == L_UNDEF) return false;
        return (l > 0) ? (a == L_TRUE) : (a == L_FALSE);
    }
    bool lit_false_(Lit l) const {
        uint8_t a = assigns[lit_var(l)];
        if (a == L_UNDEF) return false;
        return (l > 0) ? (a == L_FALSE) : (a == L_TRUE);
    }
    bool lit_undef_(Lit l) const { return assigns[lit_var(l)] == L_UNDEF; }

    // ── Hardware driving ─────────────────────────────────────────
    Vsat_solver* dut_;

    void tick_() {
        dut_->clk = 0; dut_->eval(); main_time += 5;
        dut_->clk = 1; dut_->eval(); main_time += 5;
    }
    void idle_inputs_() {
        dut_->op = OP_IDLE;
        dut_->vid_in = 0; dut_->val_in = 0; dut_->pol_in = 0;
        dut_->row_addr = 0; dut_->cid_in = 0; dut_->phase = 0;
    }
    void reset_hw_() {
        dut_->rst_n = 0;
        idle_inputs_();
        dut_->eval();
        tick_(); tick_();
        dut_->rst_n = 1;
        tick_();
    }
    void load_hw_(int row_addr, int vid, int pol) {
        n_hw_load++;
        dut_->op = OP_LOAD;
        dut_->row_addr = static_cast<uint16_t>(row_addr);
        dut_->vid_in = vid;
        dut_->pol_in = pol;
        dut_->val_in = VAL_U;
        tick_();  // S_IDLE -> S_LOAD
        tick_();  // S_LOAD -> S_IDLE; posedge latches vid/pol/val into storage
        idle_inputs_();
        if (std::getenv("SAT_DEBUG")) {
            std::fprintf(stderr, "  load row=%d vid=%d pol=%d -> pol_store[%d]=%d val_store[%d]=%d\n",
                row_addr, vid, pol,
                row_addr, dut_->sat_submodule->pol_store[row_addr],
                row_addr, dut_->sat_submodule->val_store[row_addr]);
        }
    }

    struct HWBCP {
        bool conf, up, done;
        int  cid;
        int  up_lit_pos;
        int  up_vid;
        int  up_pol;
    };
    HWBCP bcp_hw_(int vid, int val) {
        n_hw_bcp++;
        dut_->op = OP_BCP;
        dut_->vid_in = vid;
        dut_->val_in = val;
        tick_();  // S_IDLE -> S_BCP1
        tick_();  // S_BCP1 -> S_BCP2 (outputs latched)
        HWBCP r;
        r.conf       = dut_->conf_out;
        r.up         = dut_->up_out;
        r.done       = dut_->done_out;
        r.cid        = dut_->cid_out;
        r.up_lit_pos = dut_->up_lit_pos;
        r.up_vid     = dut_->vid_out;
        r.up_pol     = dut_->pol_out;
        if (std::getenv("SAT_DEBUG")) {
            std::fprintf(stderr,
                "  bcp(vid=%d,val=%d) -> conf=%d up=%d done=%d cid=%d ulp=%d up_vid=%d up_pol=%d\n",
                vid, val, r.conf, r.up, r.done, r.cid, r.up_lit_pos, r.up_vid, r.up_pol);
            std::fprintf(stderr, "    val_store:");
            for (int i = 0; i < HW_NUM_ROWS; i++) std::fprintf(stderr, " %d", dut_->sat_submodule->val_store[i]);
            std::fprintf(stderr, "\n    ml_q:     ");
            for (int i = 0; i < HW_NUM_ROWS; i++) std::fprintf(stderr, " %d", dut_->sat_submodule->ml_q[i]);
            std::fprintf(stderr, "\n    proc_conf:");
            for (int i = 0; i < HW_NUM_CLAUSES; i++) std::fprintf(stderr, " %d", dut_->sat_submodule->proc_conf[i]);
            std::fprintf(stderr, "\n    proc_up:  ");
            for (int i = 0; i < HW_NUM_CLAUSES; i++) std::fprintf(stderr, " %d", dut_->sat_submodule->proc_up[i]);
            std::fprintf(stderr, "\n    proc_done:");
            for (int i = 0; i < HW_NUM_CLAUSES; i++) std::fprintf(stderr, " %d", dut_->sat_submodule->proc_done[i]);
            std::fprintf(stderr, "\n");
        }
        idle_inputs_();
        tick_();  // S_BCP2 -> S_IDLE
        return r;
    }
    // Pre-set val_store for all rows of vid so the HW combinational logic
    // sees the variable as assigned. Used to "consume" a reported UP before
    // re-issuing bcp_hw_ for the same (vid,val) to find additional UPs.
    void setValInHW_(int vid, int val_code) {
        for (int row = 0; row < HW_NUM_ROWS; row++) {
            if (dut_->sat_submodule->vid_store[row] == static_cast<uint32_t>(vid))
                dut_->sat_submodule->val_store[row] = static_cast<uint8_t>(val_code);
        }
    }

    void undo_hw_(int vid) {
        n_hw_undo++;
        dut_->op = OP_UNDO;
        dut_->vid_in = vid;
        dut_->val_in = VAL_U;
        tick_();  // S_IDLE -> S_UNDO
        tick_();  // S_UNDO -> S_IDLE; posedge writes VAL_U to matched rows
        idle_inputs_();
    }

    // ── Clause DB ────────────────────────────────────────────────
    bool addOriginalClause(vector<Lit> c) {
        std::sort(c.begin(), c.end(),
                  [](Lit a, Lit b) { return lit_var(a) < lit_var(b); });
        vector<Lit> out;
        for (size_t i = 0; i < c.size(); i++) {
            if (i > 0 && c[i] == c[i - 1]) continue;
            if (i > 0 && c[i] == -c[i - 1]) return true;  // tautology, skip
            out.push_back(c[i]);
        }
        if (out.empty()) { unsat_at_load = true; return true; }
        int ci = static_cast<int>(originals.size());
        Clause cl; cl.lits = out;
        originals.push_back(cl);

        if (ci < HW_NUM_CLAUSES) {
            if (out.size() > static_cast<size_t>(HW_LITS_PER_CLAUSE)) {
                std::cerr << "ERROR: clause has " << out.size()
                          << " lits; RTL supports at most " << HW_LITS_PER_CLAUSE << "\n";
                return false;
            }
            // Load only real literals. Behavioral RTL treats unloaded rows
            // (row_valid=0) as always-false, so no padding literals are needed.
            for (int li = 0; li < static_cast<int>(out.size()); li++) {
                Lit l = out[li];
                load_hw_(HW_LITS_PER_CLAUSE * ci + li, lit_var(l), lit_pol(l));
            }
        }
        return true;
    }

    // Load a learnt clause into HW at slot (n_orig_ + li).
    void loadLearntHW_(int li, const vector<Lit>& lits) {
        if (li >= static_cast<int>(learnt_in_hw.size())) return;
        learnt_in_hw[li] = 0;
        int hw_ci = n_orig_ + li;
        // Only load learnt clauses the RTL can represent exactly. Silently
        // truncating a wider clause would strengthen it and break soundness.
        if (hw_ci >= HW_NUM_CLAUSES) {
            std::fprintf(stderr, "c WARNING: learnt clause %d overflows HW (%d slots, %d originals)\n",
                         li, HW_NUM_CLAUSES, n_orig_);
            return;
        }
        if (lits.size() > static_cast<size_t>(HW_LITS_PER_CLAUSE)) return;
        for (int i = 0; i < static_cast<int>(lits.size()); i++) {
            Lit l = lits[i];
            int v   = lit_var(l);
            int row = HW_LITS_PER_CLAUSE * hw_ci + i;
            load_hw_(row, v, lit_pol(l));
            // Body literals are already assigned (false) at the backtrack level but
            // their new row starts as VAL_U from load_hw_. They won't be updated by
            // HW BCP until re-propagated, causing missed UPs. Patch val_store directly.
            if (assigns[v] != L_UNDEF) {
                dut_->sat_submodule->val_store[row] =
                    (assigns[v] == L_TRUE) ? static_cast<uint8_t>(VAL_ONE)
                                           : static_cast<uint8_t>(VAL_ZERO);
            }
        }
        learnt_in_hw[li] = 1;
    }

    // Called once after every original clause is loaded.
    void postLoad_() {
        n_orig_ = static_cast<int>(originals.size());
        // Enqueue any unit clauses at decision level 0.
        for (size_t ci = 0; ci < originals.size(); ci++) {
            if (originals[ci].lits.size() == 1) {
                Lit l = originals[ci].lits[0];
                if (lit_false_(l)) { unsat_at_load = true; return; }
                if (lit_undef_(l) && !enqueue(l, static_cast<int>(ci))) {
                    unsat_at_load = true; return;
                }
            }
        }
    }

    // ── Trail / assignment ───────────────────────────────────────
    bool enqueue(Lit l, int r) {
        int v = lit_var(l);
        if (assigns[v] != L_UNDEF) return lit_true_(l);
        assigns[v] = (l > 0) ? L_TRUE : L_FALSE;
        level[v]   = decisionLevel();
        reason_[v] = r;
        trail.push_back(l);
        return true;
    }

    void newDecisionLevel() { trail_lim.push_back(static_cast<int>(trail.size())); }

    void cancelUntil(int lvl) {
        if (decisionLevel() <= lvl) return;
        for (int i = static_cast<int>(trail.size()) - 1;
             i >= trail_lim[lvl]; i--) {
            int v = lit_var(trail[i]);
            undo_hw_(v);
            assigns[v] = L_UNDEF;
            level[v]   = -1;
            reason_[v] = -1;
        }
        trail.resize(trail_lim[lvl]);
        qhead = static_cast<int>(trail.size());
        trail_lim.resize(lvl);
    }

    // ── Propagation ──────────────────────────────────────────────
    // Returns the reason code of a conflict, or -1 for no conflict.
    // All clauses (originals + learnts that fit) live in HW.
    // cid_out in [0, n_orig_) = original; [n_orig_, 1024) = learnt.
    int propagate() {
        while (qhead < static_cast<int>(trail.size())) {
            Lit l = trail[qhead++];
            n_propagations++;
            int v   = lit_var(l);
            int val = (l > 0) ? VAL_ONE : VAL_ZERO;

            // Loop until the HW reports no more events for this assignment.
            // One bcp_hw_ call returns at most one UP; multiple clauses may
            // become unit simultaneously, so we consume each UP by writing
            // its value into val_store (making that clause proc_done=1) and
            // re-issue BCP to find the next unit clause.
            for (;;) {
                HWBCP r = bcp_hw_(v, val);
                int reason_code = (r.cid >= n_orig_)
                                    ? encode_learnt(r.cid - n_orig_)
                                    : r.cid;
                if (r.conf) return reason_code;
                if (r.up) {
                    int up_val = (r.up_pol == 0) ? VAL_ONE : VAL_ZERO;
                    setValInHW_(r.up_vid, up_val);
                    Lit impl = (r.up_pol == 0) ? static_cast<Lit>(r.up_vid)
                                               : -static_cast<Lit>(r.up_vid);
                    if (!enqueue(impl, reason_code)) return reason_code;
                } else {
                    break;
                }
            }

            int lc = propagateLearntsSW_();
            if (lc != -1) return encode_learnt(lc);
        }
        return -1;
    }

    // SW BCP for learnts not represented in HW: either they overflow the
    // learnt-clause slots or are wider than the RTL can encode faithfully.
    int propagateLearntsSW_() {
        for (int li = 0; li < static_cast<int>(learnts.size()); li++) {
            if (li < static_cast<int>(learnt_in_hw.size()) && learnt_in_hw[li]) continue;
            const vector<Lit>& c = learnts[li].lits;
            int num_u = 0;
            Lit u_lit = 0;
            bool sat = false;
            for (Lit lt : c) {
                if (lit_true_(lt)) { sat = true; break; }
                if (lit_undef_(lt)) { num_u++; u_lit = lt; }
            }
            if (sat) continue;
            if (num_u == 0) return li;
            if (num_u == 1) {
                if (!enqueue(u_lit, encode_learnt(li))) return li;
            }
        }
        return -1;
    }

    // ── 1-UIP conflict analysis (MiniSat-style) ──────────────────
    void analyze(int confl, vector<Lit>& out_learnt, int& out_btlevel) {
        out_learnt.clear();
        out_learnt.push_back(0);  // slot 0 reserved for 1-UIP
        vector<uint8_t> seen(num_vars + 1, 0);
        int counter = 0;
        Lit p = 0;
        int p_reason = confl;
        int index = static_cast<int>(trail.size()) - 1;
        out_btlevel = 0;

        for (;;) {
            const vector<Lit>& rlits =
                reason_is_learnt(p_reason)
                    ? learnts[learnt_of(p_reason)].lits
                    : originals[p_reason].lits;
            for (Lit q : rlits) {
                if (q == p) continue;
                int v = lit_var(q);
                if (!seen[v] && level[v] > 0) {
                    bumpVarActivity(v);
                    seen[v] = 1;
                    if (level[v] >= decisionLevel()) {
                        counter++;
                    } else {
                        out_learnt.push_back(q);
                        if (level[v] > out_btlevel) out_btlevel = level[v];
                    }
                }
            }
            while (index >= 0 && !seen[lit_var(trail[index])]) index--;
            if (index < 0) break;
            p = trail[index];
            p_reason = reason_[lit_var(p)];
            seen[lit_var(p)] = 0;
            index--;
            counter--;
            if (counter <= 0) break;
        }
        assert(p != 0);  // degenerate trail — propagation invariant violated
        out_learnt[0] = -p;
    }

    void bumpVarActivity(int v) {
        activity[v] += var_inc;
        if (activity[v] > 1e100) {
            for (int i = 1; i <= num_vars; i++) activity[i] *= 1e-100;
            var_inc *= 1e-100;
        }
    }
    void decayVarActivity() { var_inc *= (1.0 / var_decay); }

    int pickBranch() {
        int best = -1;
        double best_act = -1.0;
        for (int v = 1; v <= num_vars; v++) {
            if (assigns[v] == L_UNDEF && activity[v] >= best_act) {
                best_act = activity[v];
                best = v;
            }
        }
        return best;
    }

    // ── Main CDCL loop ───────────────────────────────────────────
    // Returns 1 on SAT, 0 on UNSAT.
    int solve() {
        postLoad_();
        if (unsat_at_load) return 0;
        for (;;) {
            int confl = propagate();
            if (confl != -1) {
                n_conflicts++;
                if (decisionLevel() == 0) return 0;
                vector<Lit> learnt;
                int btlevel;
                analyze(confl, learnt, btlevel);
                cancelUntil(btlevel);
                int li = static_cast<int>(learnts.size());
                Clause lc; lc.lits = learnt;
                learnts.push_back(lc);
                learnt_in_hw.push_back(0);
                loadLearntHW_(li, learnt);
                enqueue(learnt[0], encode_learnt(li));
                decayVarActivity();
            } else {
                if (static_cast<int>(trail.size()) == num_vars) return 1;
                int v = pickBranch();
                if (v < 0) return 1;
                n_decisions++;
                newDecisionLevel();
                enqueue(v, -1);  // default polarity: true
            }
        }
    }

    void printModel(std::ostream& os) const {
        os << "v ";
        for (int v = 1; v <= num_vars; v++) {
            if (assigns[v] == L_TRUE)       os << v << " ";
            else if (assigns[v] == L_FALSE) os << -v << " ";
        }
        os << "0\n";
    }
};

static bool parseDIMACS(const string& path, Solver& S) {
    std::ifstream fin(path);
    if (!fin) { std::cerr << "Cannot open " << path << "\n"; return false; }
    string line;
    int nvars = 0, nclauses = 0;
    bool have_header = false;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == 'c') continue;
        if (line[0] == 'p') {
            std::istringstream is(line);
            string p, cnf;
            is >> p >> cnf >> nvars >> nclauses;
            S.setNumVars(nvars);
            have_header = true;
            break;
        }
    }
    if (!have_header) { std::cerr << "No DIMACS header\n"; return false; }
    vector<Lit> cur;
    int lit;
    while (fin >> lit) {
        if (lit == 0) {
            if (!S.addOriginalClause(cur)) return false;
            cur.clear();
        } else {
            cur.push_back(lit);
        }
    }
    (void)nclauses;
    return true;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cnf_file>\n";
        return 1;
    }
    Solver S;
    if (!parseDIMACS(argv[1], S)) return 1;
    auto t0 = std::chrono::steady_clock::now();
    int r = S.solve();
    auto t1 = std::chrono::steady_clock::now();
    double sw_time_ns = std::chrono::duration<double, std::nano>(t1 - t0).count();
    if (r == 1) {
        std::cout << "s SATISFIABLE\n";
        S.printModel(std::cout);
    } else {
        std::cout << "s UNSATISFIABLE\n";
    }
    uint64_t hw_bcp_cycles = S.n_hw_bcp;          // 1 true HW cycle per BCP
    double   bcp_time_ns   = hw_bcp_cycles / 1.0; // @1GHz: 1 cycle = 1ns
    std::cerr << "c originals="      << S.originals.size()
              << " learnts="         << S.learnts.size()
              << " decisions="       << S.n_decisions
              << " conflicts="       << S.n_conflicts
              << " propagations="    << S.n_propagations
              << " hw_bcp="          << S.n_hw_bcp
              << " hw_undo="         << S.n_hw_undo
              << " hw_load="         << S.n_hw_load
              << "\n";
    std::cerr << "c hw_bcp_cycles="  << hw_bcp_cycles
              << " (sim_cycles="     << S.n_hw_bcp * 3 << "/3)"
              << " @1GHz bcp_time="  << bcp_time_ns    << "ns"
              << " ("                << bcp_time_ns / 1e3 << "us)\n";
    std::cerr << "c sw_cdcl_time="   << sw_time_ns     << "ns"
              << " ("                << sw_time_ns / 1e3 << "us)\n";
    return 0;
}
