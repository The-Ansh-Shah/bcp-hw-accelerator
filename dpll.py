#!/usr/bin/env python3
"""
dpll.py — Python model of the hardware SAT submodule + DPLL solver.

Models the hardware exactly:
  - CAM/SRAM array: vid, pol, val per row
  - BCP: matchline-activated simultaneous update + clause evaluation
  - processing_logic.sv: CONF/UP/DONE per clause, matchline-gated UP
  - combining_logic.sv:  CONF > UP > DONE priority, LOWEST-index clause wins
  - DPLL: decisions + BCP propagation loop + backtrack (UNDO)

Key hardware fidelity notes:
  - UP is only reported for clauses whose rows have an active matchline
    (ml[i] = stored_vid == search_vid).  Matches the matchline-gated UP
    condition in processing_logic.sv.
  - combining_logic.sv iterates high→low with last-writer-wins, so the
    LOWEST-index CONF/UP clause is reported.  _combine() mirrors this.
  - val encoding: 0 = logic-0,  1 = logic-1,  2 = unassigned (U).
  - pol encoding: 0 = positive literal (xi),  1 = negative literal (~xi).
"""

# ── Value constants ────────────────────────────────────────────────────────
VAL_ZERO = 0   # 2'b00
VAL_ONE  = 1   # 2'b01
VAL_U    = 2   # 2'b10  (unassigned)


# ── Data structures ────────────────────────────────────────────────────────

class Row(object):
    __slots__ = ('vid', 'pol', 'val', 'valid')
    def __init__(self):
        self.vid   = 0
        self.pol   = 0
        self.val   = VAL_U
        self.valid = False


class ClauseResult(object):
    __slots__ = ('conf', 'up', 'done', 'up_lit_pos')
    def __init__(self, conf=False, up=False, done=False, up_lit_pos=0):
        self.conf       = conf
        self.up         = up
        self.done       = done
        self.up_lit_pos = up_lit_pos


class TraceEntry(object):
    __slots__ = ('op', 'vid', 'val', 'conf', 'up', 'done', 'label')
    def __init__(self, op, vid, val, conf, up, done, label):
        self.op    = op     # 'bcp' or 'undo'
        self.vid   = vid
        self.val   = val    # VAL_ZERO/ONE for bcp; VAL_U for undo
        self.conf  = conf
        self.up    = up
        self.done  = done
        self.label = label  # human-readable name for check()


# ── Clause evaluation ──────────────────────────────────────────────────────

def _lit_effective(val, pol):
    """Return 'T', 'F', or 'U' for a single literal.

    Mirrors processing_logic.sv:
        lit_true[i]  = ~unassigned & (val[0] == ~pol)
        lit_false[i] = ~unassigned & (val[0] ==  pol)
    pol=0 (positive xi):   TRUE when val=1, FALSE when val=0
    pol=1 (negative ~xi):  TRUE when val=0, FALSE when val=1
    """
    if val == VAL_U:
        return 'U'
    if (val & 1) == (pol ^ 1):   # val[0] == ~pol → TRUE
        return 'T'
    return 'F'


def _eval_clause(vals, pols, mls):
    """Evaluate one 3-literal clause.  Mirrors processing_logic.sv exactly."""
    effs = [_lit_effective(vals[i], pols[i]) for i in range(3)]

    done  = any(e == 'T' for e in effs)
    conf  = all(e == 'F' for e in effs) and not done
    any_ml = any(mls)

    num_u = sum(1 for e in effs if e == 'U')
    up = (
        num_u == 1 and
        all(e in ('F', 'U') for e in effs) and
        not done and
        any_ml
    )

    up_lit_pos = 0
    if up:
        for i, e in enumerate(effs):
            if e == 'U':
                up_lit_pos = i
                break

    return ClauseResult(conf=conf, up=up, done=done, up_lit_pos=up_lit_pos)


def _combine(clause_results):
    """Priority reduction matching combining_logic.sv.

    Hardware iterates high→low, last-writer-wins → LOWEST-index clause wins.
    Priority: CONF > UP > DONE.
    Returns (conf, up, done, cid, up_lit_pos).
    """
    conf = False
    up   = False
    done = False
    cid  = 0
    ulp  = 0

    # Iterate high→low so that the lowest-index matching clause is the
    # last write and therefore wins (mirrors the hardware for-loop).
    for i in range(len(clause_results) - 1, -1, -1):
        r = clause_results[i]
        if r.conf:
            conf = True
            up   = False
            done = False
            cid  = i

    if not conf:
        for i in range(len(clause_results) - 1, -1, -1):
            r = clause_results[i]
            if r.up:
                up   = True
                done = False
                cid  = i
                ulp  = r.up_lit_pos

        if not up:
            done = all(r.done for r in clause_results)

    return conf, up, done, cid, ulp


# ── Hardware model ─────────────────────────────────────────────────────────

class HardwareModel(object):
    """Behavioural model of the sat_submodule CAM/SRAM array + evaluation."""

    def __init__(self, num_clauses, vid_width=20):
        self.num_clauses = num_clauses
        self.num_rows    = 3 * num_clauses
        self.vid_width   = vid_width
        self.rows        = [Row() for _ in range(self.num_rows)]

    def _matchlines(self, search_vid):
        return [r.valid and (r.vid == search_vid) for r in self.rows]

    def _evaluate_all(self, mls):
        results = []
        for c in range(self.num_clauses):
            base = 3 * c
            vals      = [self.rows[base + i].val for i in range(3)]
            pols      = [self.rows[base + i].pol for i in range(3)]
            clause_ml = [mls[base + i]           for i in range(3)]
            results.append(_eval_clause(vals, pols, clause_ml))
        return results

    def load(self, row_addr, vid, pol):
        """LOAD: write VID+polarity, initialise value to U."""
        r       = self.rows[row_addr]
        r.vid   = vid
        r.pol   = pol
        r.val   = VAL_U
        r.valid = True

    def bcp(self, vid, val):
        """BCP: search CAM, matchline-activated SRAM write, evaluate clauses."""
        mls = self._matchlines(vid)
        for r, ml in zip(self.rows, mls):
            if ml:
                r.val = val
        return _combine(self._evaluate_all(mls))

    def undo(self, vid):
        """UNDO: reset all rows with this VID to unassigned."""
        mls = self._matchlines(vid)
        for r, ml in zip(self.rows, mls):
            if ml:
                r.val = VAL_U
        null_mls = [False] * self.num_rows
        return _combine(self._evaluate_all(null_mls))

    def eval_idle(self):
        """Evaluate current state with no matchlines active (IDLE)."""
        null_mls = [False] * self.num_rows
        return _combine(self._evaluate_all(null_mls))


# ── DPLL solver ────────────────────────────────────────────────────────────

def dpll_solve(hw, max_decisions=100000):
    """Run DPLL on the already-loaded hardware model.

    Returns (sat, assignment, trace):
        sat        — True (SAT), False (UNSAT), or None (timeout)
        assignment — {vid: val} for SAT, empty otherwise
        trace      — list of TraceEntry for testbench generation

    Decision stack entries:
        [vid, val_tried, [vars_assigned_at_this_level], flipped]
        flipped = True means both values have been tried → backtrack further.
    """
    trace      = []
    assignment = {}

    # [vid, val_tried, [assigned_vars_this_level], flipped]
    decision_stack = []

    all_vids = sorted(set(r.vid for r in hw.rows if r.valid))

    def pick_unassigned():
        for vid in all_vids:
            if vid not in assignment:
                return vid
        return None

    def record_bcp(vid, val, label):
        conf, up, done, cid, ulp = hw.bcp(vid, val)
        assignment[vid] = val
        if decision_stack:
            decision_stack[-1][2].append(vid)
        trace.append(TraceEntry('bcp', vid, val, conf, up, done, label))
        return conf, up, done, cid, ulp

    def record_undo(vid, label):
        conf, up, done, cid, ulp = hw.undo(vid)
        assignment.pop(vid, None)
        trace.append(TraceEntry('undo', vid, VAL_U, conf, up, done, label))
        return conf, up, done, cid, ulp

    def do_backtrack():
        """Unwind the decision stack until we find an unflipped level to flip,
        or determine UNSAT.  Returns True if a flip was made, False if UNSAT."""
        while decision_stack:
            dec_vid, dec_val, level_vars, flipped = decision_stack[-1]

            # Undo all vars assigned at this level
            for vid in reversed(level_vars):
                record_undo(vid, "UNDO(x{})".format(vid))
            decision_stack.pop()

            if not flipped:
                # Try the other value for this decision variable
                other_val = VAL_ONE if dec_val == VAL_ZERO else VAL_ZERO
                vs        = '1' if other_val == VAL_ONE else '0'
                decision_stack.append([dec_vid, other_val, [dec_vid], True])
                record_bcp(dec_vid, other_val, "DEC(x{}={})".format(dec_vid, vs))
                return True   # flipped; caller re-enters propagation loop

        return False   # exhausted all decision levels → UNSAT

    decisions = 0

    while decisions < max_decisions:

        # ── Propagation loop: drain all unit propagations ────────────────────
        conflict = False
        while True:
            conf, up, done, cid, ulp = hw.eval_idle()

            if done:
                return True, dict(assignment), trace

            if conf:
                conflict = True
                break

            if up:
                up_row = 3 * cid + ulp
                up_vid = hw.rows[up_row].vid
                up_pol = hw.rows[up_row].pol
                up_val = VAL_ONE if up_pol == 0 else VAL_ZERO
                vs     = '1' if up_val == VAL_ONE else '0'

                conf2, up2, done2, _, _ = record_bcp(
                    up_vid, up_val, "UP(x{}={})".format(up_vid, vs))

                if done2:
                    return True, dict(assignment), trace
                if conf2:
                    conflict = True
                    break
                # Loop again: another UP may be available
                continue

            break   # Open, no UP/CONF/DONE → need decision

        # ── Conflict: backtrack ──────────────────────────────────────────────
        if conflict:
            if not do_backtrack():
                return False, {}, trace   # UNSAT
            continue   # re-enter propagation loop after flip

        # ── Decision ────────────────────────────────────────────────────────
        vid = pick_unassigned()
        if vid is None:
            return True, dict(assignment), trace   # all assigned, no conflict

        decisions += 1
        decision_stack.append([vid, VAL_ONE, [vid], False])
        conf2, up2, done2, _, _ = record_bcp(vid, VAL_ONE, "DEC(x{}=1)".format(vid))

        if done2:
            return True, dict(assignment), trace

    return None, {}, trace   # timeout


# ── Convenience wrapper ────────────────────────────────────────────────────

def solve_cnf(clauses, num_clauses_hw, vid_width=20, max_decisions=100000):
    """Load clauses into a fresh HardwareModel and run DPLL.

    clauses:        list of clauses; each clause is a list of (vid, pol) pairs.
    num_clauses_hw: hardware array size (>= len(clauses), power of 2).

    Returns (sat, assignment, trace, hw).
    """
    hw = HardwareModel(num_clauses=num_clauses_hw, vid_width=vid_width)
    for ci, clause in enumerate(clauses):
        for li, (vid, pol) in enumerate(clause[:3]):
            hw.load(3 * ci + li, vid, pol)
    sat, assignment, trace = dpll_solve(hw, max_decisions=max_decisions)
    return sat, assignment, trace, hw


# ── Self-test ──────────────────────────────────────────────────────────────

if __name__ == '__main__':
    def run(name, clauses, hw_size, expect):
        sat, asgn, trace, _ = solve_cnf(clauses, num_clauses_hw=hw_size)
        result = 'SAT' if sat else ('UNSAT' if sat is False else 'TIMEOUT')
        ok = (result == expect)
        print("{} (expect {}): {}  [{}]  ops={}".format(
            name, expect, result, 'OK' if ok else 'FAIL', len(trace)))
        if not ok or sat:
            for t in trace:
                s = 'CONF' if t.conf else 'UP' if t.up else 'DONE' if t.done else 'OPEN'
                print("  {:4s}  {:<30s} -> {}".format(t.op, t.label, s))
        return ok

    all_ok = True

    # Test 1: simple SAT
    all_ok &= run("T1 simple SAT", [
        [(1, 0), (2, 1), (3, 0)],
        [(1, 1), (2, 0), (3, 1)],
        [(1, 0), (2, 0), (4, 0)],
        [(2, 0), (3, 1), (4, 1)],
    ], hw_size=4, expect='SAT')

    # Test 2: unit propagation chain → SAT without decisions
    # (~x1), so x1=0 is forced; then (x1∨x2) forces x2=1; etc.
    # Using 3-literal clauses padded:
    # (~x1 ∨ ~x2 ∨ ~x3)  x1=0 forced by (x1∨x2∨x3)? No, let's do:
    # (x1∨x2∨x3) ∧ (~x1∨x2∨x3) ∧ (~x1∨~x2∨x3) ∧ (~x1∨~x2∨~x3)
    # If x1=1: clause 1=T, clause 2 has ~x1=F → UP on x2,x3 etc.
    all_ok &= run("T2 backtrack SAT", [
        [(1, 0), (2, 0), (3, 0)],
        [(1, 1), (2, 0), (3, 0)],
        [(1, 1), (2, 1), (3, 0)],
        [(1, 1), (2, 1), (3, 1)],
    ], hw_size=4, expect='SAT')

    # Test 3: UNSAT — all 8 sign combinations of 3 vars, unsatisfiable
    all_ok &= run("T3 UNSAT (all signs)", [
        [(1, 0), (2, 0), (3, 0)],
        [(1, 1), (2, 1), (3, 1)],
        [(1, 0), (2, 1), (3, 1)],
        [(1, 1), (2, 0), (3, 0)],
        [(1, 0), (2, 0), (3, 1)],
        [(1, 1), (2, 1), (3, 0)],
        [(1, 0), (2, 1), (3, 0)],
        [(1, 1), (2, 0), (3, 1)],
    ], hw_size=8, expect='UNSAT')

    print("\n{}.".format("ALL TESTS PASSED" if all_ok else "SOME TESTS FAILED"))
