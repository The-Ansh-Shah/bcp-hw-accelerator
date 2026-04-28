#!/usr/bin/env python3
"""
gen_tb.py — Generate a sat_submodule testbench from a CNF formula.

Usage:
    python3 gen_tb.py formula.cnf            # DIMACS CNF file
    python3 gen_tb.py formula.cnf -o tb/my_tb.sv
    python3 gen_tb.py                        # uses INLINE_CNF below

The script:
  1. Parses the CNF formula.
  2. Runs the Python DPLL solver (dpll.py) to produce a full solve trace,
     matching the hardware's BCP semantics exactly.
  3. Emits a SystemVerilog testbench with:
       - LOAD phase (one load_row() per array row)
       - SOLVE phase (bcp()/undo() calls from the DPLL trace)
       - CHECK after each operation (expected conf/up/done from the solver)
       - Cycle counting per BCP and UNDO operation

DIMACS format reminder:
    c comment lines start with c
    p cnf <num_vars> <num_clauses>
    1 -3 5 0       <- clause: x1 OR (NOT x3) OR x5, terminated by 0
    Variables are 1-indexed; this script converts to 0-indexed VIDs.

Polarity encoding (matches sat_submodule):
    pol=0  ->  positive literal  xi
    pol=1  ->  negative literal  ~xi

Value encoding (matches sat_submodule.sv / processing_logic.sv / hw_bcp_defs.vh):
    2'b00 = 0 (logic zero)
    2'b11 = 1 (logic one)
    2'b01 = U (unassigned, initial state)

VID convention:
    DIMACS variable i  ->  VID i  (1-indexed, mirroring coprocessing/cdcl_solver.cpp).
    VID 0 is reserved for padding when a clause has fewer than LITS_PER_CLAUSE
    literals; the generated TB pins x0=0 once after LOAD via bcp(0, VAL_ZERO).
"""

import argparse
import sys
import textwrap
from pathlib import Path

# Import the hardware-faithful DPLL solver
import dpll as _dpll
VAL_ZERO = _dpll.VAL_ZERO
VAL_ONE  = _dpll.VAL_ONE
VAL_U    = _dpll.VAL_U


# ── Inline CNF (alternative to passing a file) ─────────────────────────────
# Edit this list to embed a formula directly.
# Variables are 1-indexed (DIMACS-style); VID 0 is reserved for padding.
INLINE_CNF = [
    [ 1,  2, -6],   # x1 ∨ x2 ∨ ~x6
    [ 2, -3,  6],   # x2 ∨ ~x3 ∨ x6
    [-1,  3,  4],   # ~x1 ∨ x3 ∨ x4
    [-2, -4,  5],   # ~x2 ∨ ~x4 ∨ x5
]
# ────────────────────────────────────────────────────────────────────────────


def parse_dimacs(path):
    """Return (num_vars, clauses) from a DIMACS CNF file.
    clauses is a list of lists of (vid:int, pol:int) pairs (0-indexed vids).
    """
    clauses  = []
    num_vars = 0
    current  = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('c'):
                continue
            if line.startswith('p'):
                parts = line.split()
                num_vars = int(parts[2])
                continue
            for tok in line.split():
                lit = int(tok)
                if lit == 0:
                    if current:
                        clauses.append(current)
                    current = []
                else:
                    vid = abs(lit)       # keep DIMACS 1-indexing; VID 0 reserved for padding
                    pol = 1 if lit < 0 else 0
                    current.append((vid, pol))
    if current:
        clauses.append(current)
    return num_vars, clauses


def parse_inline(raw):
    """Convert INLINE_CNF (0-indexed signed ints) to list of (vid, pol) pairs."""
    clauses = []
    for clause in raw:
        lits = []
        for lit in clause:
            vid = abs(lit)
            pol = 1 if lit < 0 else 0
            lits.append((vid, pol))
        clauses.append(lits)
    return clauses


def next_power_of_two(n):
    p = 1
    while p < n:
        p <<= 1
    return p


def pad_clause(lits, width=3):
    """Pad a clause to exactly `width` literals.  Extra slots get vid=1, pol=0,
    but are marked invalid (no valid bit written, so CAM never matches them).
    Actually — since we load all rows, unused slots get vid=0,pol=0.
    Returns list of (vid, pol, valid) triples.
    """
    result = [(vid, pol, True) for vid, pol in lits[:width]]
    while len(result) < width:
        result.append((0, 0, False))
    return result


def _sv_val(val, vid_width):
    """Return SV literal for a value constant."""
    if val == VAL_ZERO:
        return "VAL_ZERO"
    elif val == VAL_ONE:
        return "VAL_ONE"
    else:
        return "VAL_U"


def generate_tb(clauses, num_vars, out_path, vid_width=20,
                lits_per_clause=4, solve=True, max_decisions=100000):
    num_clauses_raw = len(clauses)
    num_clauses_hw  = next_power_of_two(num_clauses_raw)   # must be power-of-2
    num_rows        = lits_per_clause * num_clauses_hw
    row_addr_w      = max(1, num_rows.bit_length())

    # ── CNF echo + LOAD block ───────────────────────────────────────────────
    load_lines = []
    load_lines.append(
        '        $display("\\n-- CNF ({} clauses, {} literals each) --");'.format(
            num_clauses_hw, lits_per_clause))
    for ci in range(num_clauses_hw):
        clause = clauses[ci] if ci < num_clauses_raw else []
        padded = pad_clause(clause, lits_per_clause)
        lit_strs = []
        for vid, pol, valid in padded:
            lit_strs.append("{}x{}".format("~" if pol else " ", vid))
        note = " (padding)" if ci >= num_clauses_raw else ""
        load_lines.append(
            '        $display("  C{}{}: ( {} )");'.format(
                ci, note, " | ".join(lit_strs)))
    load_lines.append("")
    load_lines.append('        $display("\\n-- LOAD --");')
    for ci in range(num_clauses_hw):
        clause  = clauses[ci] if ci < num_clauses_raw else []
        padded  = pad_clause(clause, lits_per_clause)
        load_lines.append("        // Clause {}{}".format(
            ci, " (padding)" if ci >= num_clauses_raw else ""))
        for li, (vid, pol, valid) in enumerate(padded):
            row = ci * lits_per_clause + li
            if valid:
                comment = "{}x{}  ({})".format(
                    '~' if pol else '', vid, 'neg' if pol else 'pos')
            else:
                comment = "(unused slot — VID 0 padding)"
            load_lines.append(
                "        load_row({:3d}, {}'d{}, 1'b{});  // C{} L{}: {}".format(
                    row, vid_width, vid, pol, ci, li, comment))
        load_lines.append("")
    load_block = "\n".join(load_lines)

    # ── SOLVE block from DPLL trace ─────────────────────────────────────────
    sat_result = None
    solve_lines = []
    if solve:
        sat, assignment, trace, hw = _dpll.solve_cnf(
            clauses, num_clauses_hw=num_clauses_hw,
            lits_per_clause=lits_per_clause,
            vid_width=vid_width, max_decisions=max_decisions)

        sat_result = sat
        result_str = 'SAT' if sat else ('UNSAT' if sat is False else 'TIMEOUT')
        solve_lines.append(
            '        $display("\\n-- SOLVE (Python DPLL result: {}) --");'.format(result_str))

        for t in trace:
            vs       = "VAL_ONE" if t.val == VAL_ONE else "VAL_ZERO"
            exp_c    = "1'b1" if t.conf else "1'b0"
            exp_u    = "1'b1" if t.up   else "1'b0"
            exp_d    = "1'b1" if t.done else "1'b0"
            label_sv = t.label.replace('"', '\\"')

            if t.op == 'bcp':
                solve_lines.append(
                    "        bcp({}'d{}, {});".format(vid_width, t.vid, vs))
                solve_lines.append(
                    '        check("{}", {}, {}, {});'.format(
                        label_sv, exp_c, exp_u, exp_d))
                solve_lines.append("")
            else:  # undo
                solve_lines.append(
                    "        undo({}'d{});".format(vid_width, t.vid))
                solve_lines.append(
                    '        check_undo("{}", {}, {}, {});'.format(
                        label_sv, exp_c, exp_u, exp_d))
                solve_lines.append("")
    else:
        solve_lines.append("        // ── ADD YOUR BCP / CHECK CALLS BELOW ──")
        solve_lines.append(
            "        // bcp({}'d1, VAL_ONE);".format(vid_width))
        solve_lines.append(
            '        // check("BCP(x1=1)", 1\'b0, 1\'b0, 1\'b0);')

    solve_block = "\n".join(solve_lines)

    # ── Testbench template ───────────────────────────────────────────────────
    tb = textwrap.dedent("""\
        // Auto-generated testbench — do not edit by hand.
        // Generated by gen_tb.py
        //
        // Formula: {num_clauses_raw} clauses, {num_vars} variables
        // HW array: {num_clauses_hw} clauses (padded to power-of-2)
        // DPLL result: {result_str}
        `timescale 1ns/1ps

        module sat_submodule_tb;

            localparam VID_WIDTH       = {vid_width};
            localparam NUM_CLAUSES     = {num_clauses_hw};
            localparam LITS_PER_CLAUSE = {lits_per_clause};
            localparam CID_WIDTH       = $clog2(NUM_CLAUSES);
            localparam NUM_ROWS        = LITS_PER_CLAUSE * NUM_CLAUSES;
            localparam ROW_ADDR_W      = $clog2(NUM_ROWS);

            localparam OP_IDLE     = 3'd0;
            localparam OP_LOAD     = 3'd1;
            localparam OP_BCP      = 3'd2;
            localparam OP_UNDO     = 3'd3;
            localparam OP_CIT_READ = 3'd4;

            // Match the RTL encoding (sat_submodule.sv / processing_logic.sv)
            localparam [1:0] VAL_ZERO = 2'b00;
            localparam [1:0] VAL_ONE  = 2'b11;
            localparam [1:0] VAL_U    = 2'b01;

            logic                    clk, rst_n;
            logic [2:0]              op;
            logic                    phase;
            logic [VID_WIDTH-1:0]    vid_in;
            logic [1:0]              val_in;
            logic                    pol_in;
            logic [ROW_ADDR_W-1:0]   row_addr;
            logic [CID_WIDTH-1:0]    cid_in;

            logic                    conf_out, up_out, done_out;
            logic [CID_WIDTH-1:0]    cid_out;
            logic [1:0]              up_lit_pos;
            logic [VID_WIDTH-1:0]    vid_out;
            logic                    pol_out;
            logic                    valid_out;

            sat_submodule #(
                .VID_WIDTH       (VID_WIDTH),
                .NUM_CLAUSES     (NUM_CLAUSES),
                .LITS_PER_CLAUSE (LITS_PER_CLAUSE),
                .CID_WIDTH       (CID_WIDTH)
            ) dut (.*);

            initial clk = 0;
            always #5 clk = ~clk;

            // ── Cycle counters ───────────────────────────────────────────
            longint bcp_call_count   = 0;
            longint bcp_total_cycles = 0;
            longint undo_call_count  = 0;
            longint undo_total_cycles = 0;
            int     num_errors = 0;

            // ── Tasks ────────────────────────────────────────────────────
            task idle_cycle;
                op = OP_IDLE; vid_in = '0; val_in = '0; pol_in = '0;
                row_addr = '0; cid_in = '0; phase = '0;
                @(posedge clk); #1;
            endtask

            task load_row(input int                     raddr,
                          input logic [VID_WIDTH-1:0]   vid,
                          input logic                   pol);
                op = OP_LOAD; row_addr = raddr; vid_in = vid;
                pol_in = pol; val_in = VAL_U; phase = '0; cid_in = '0;
                @(posedge clk); #1;
                op = OP_IDLE;
                @(posedge clk); #1;
            endtask

            // FSM: IDLE -> BCP1 -> BCP2 -> IDLE (one BCP takes 3 cycles).
            // Each call drains a previous S_BCP2 to S_IDLE first via one
            // OP_IDLE cycle, runs BCP1/BCP2, and returns with state==S_BCP2 so
            // the caller's check() can sample valid conf/up/done outputs.
            task bcp(input logic [VID_WIDTH-1:0] vid,
                     input logic [1:0]            val);
                longint t_start;
                t_start = $time / 10;
                op = OP_IDLE; vid_in = '0; val_in = '0;
                phase = '0; row_addr = '0; pol_in = '0; cid_in = '0;
                @(posedge clk); #1;
                op = OP_BCP; vid_in = vid; val_in = val;
                @(posedge clk); #1;
                @(posedge clk); #1;
                op = OP_IDLE;
                bcp_total_cycles += ($time / 10) - t_start;
                bcp_call_count++;
            endtask

            task undo(input logic [VID_WIDTH-1:0] vid);
                longint t_start;
                t_start = $time / 10;
                op = OP_IDLE; vid_in = '0; val_in = '0;
                phase = '0; row_addr = '0; pol_in = '0; cid_in = '0;
                @(posedge clk); #1;
                op = OP_UNDO; vid_in = vid; val_in = VAL_U;
                @(posedge clk); #1;
                op = OP_IDLE;
                @(posedge clk); #1;
                undo_total_cycles += ($time / 10) - t_start;
                undo_call_count++;
            endtask

            task check(input string name,
                       input logic exp_conf, exp_up, exp_done);
                string verdict;
                if (conf_out !== exp_conf || up_out !== exp_up || done_out !== exp_done) begin
                    verdict = "FAIL";
                    num_errors++;
                end else begin
                    verdict = "PASS";
                end
                $display("%s [%s]: actual {{conf=%b up=%b done=%b}}  expected {{conf=%b up=%b done=%b}}",
                         verdict, name,
                         conf_out, up_out, done_out,
                         exp_conf, exp_up, exp_done);
            endtask

            // check() variant for UNDO (same logic, separate name for clarity)
            task check_undo(input string name,
                            input logic exp_conf, exp_up, exp_done);
                check(name, exp_conf, exp_up, exp_done);
            endtask

            initial begin
                $vcdplusfile("waves.vpd");
                $vcdpluson;

                // Reset
                rst_n = 0; op = OP_IDLE; vid_in = '0; val_in = '0;
                pol_in = '0; row_addr = '0; cid_in = '0; phase = '0;
                repeat(2) @(posedge clk); #1;
                rst_n = 1;
                @(posedge clk); #1;

                // ── CNF echo + LOAD ─────────────────────────────────────
        {load_block}
        {solve_block}
                // ── STATS ────────────────────────────────────────────────
                $display("\\n-- DONE --");
                $display("BCP_STATS: bcp_calls=%0d bcp_cycles=%0d undo_calls=%0d undo_cycles=%0d",
                         bcp_call_count, bcp_total_cycles,
                         undo_call_count, undo_total_cycles);
                if (num_errors == 0)
                    $display("ALL TESTS PASSED");
                else
                    $display("FINISHED WITH %0d ERROR(S)", num_errors);

                $vcdplusoff;
                $finish;
            end

        endmodule
        """).format(
            num_clauses_raw=num_clauses_raw,
            num_vars=num_vars,
            num_clauses_hw=num_clauses_hw,
            lits_per_clause=lits_per_clause,
            result_str=('SAT' if sat_result else ('UNSAT' if sat_result is False else 'NO_SOLVE')),
            vid_width=vid_width,
            load_block=load_block,
            solve_block=solve_block,
        )

    if out_path == '-':
        print(tb)
    else:
        Path(out_path).parent.mkdir(parents=True, exist_ok=True)
        Path(out_path).write_text(tb)
        print("Written to {}".format(out_path))


def main():
    parser = argparse.ArgumentParser(
        description="Generate a sat_submodule testbench from a CNF formula.")
    parser.add_argument("cnf", nargs='?',
                        help="DIMACS CNF file (omit to use INLINE_CNF)")
    parser.add_argument("-o", "--out", default="tb/sat_submodule_tb.sv",
                        help="Output SV file (default: tb/sat_submodule_tb.sv, use - for stdout)")
    parser.add_argument("--vid-width", type=int, default=20,
                        help="VID_WIDTH parameter (default: 20)")
    parser.add_argument("--no-solve", action="store_true",
                        help="Skip DPLL solve; emit empty solve block for manual editing")
    parser.add_argument("--max-decisions", type=int, default=100000,
                        help="DPLL decision limit (default: 100000)")
    args = parser.parse_args()

    if args.cnf:
        num_vars, clauses = parse_dimacs(args.cnf)
        print("Loaded {} clauses, {} variables from {}".format(
            len(clauses), num_vars, args.cnf), file=sys.stderr)
    else:
        clauses  = parse_inline(INLINE_CNF)
        num_vars = max(vid for clause in clauses for vid, _ in clause) + 1
        print("Using INLINE_CNF: {} clauses, {} variables".format(
            len(clauses), num_vars), file=sys.stderr)

    generate_tb(clauses, num_vars, args.out,
                vid_width=args.vid_width,
                solve=not args.no_solve,
                max_decisions=args.max_decisions)


if __name__ == "__main__":
    main()
