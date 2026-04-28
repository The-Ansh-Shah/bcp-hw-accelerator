// Hand-written testbench for sat_submodule with 4 literals per clause (4-SAT).
//
// Geometry: NUM_CLAUSES=4, LITS_PER_CLAUSE=4 -> NUM_ROWS=16
// Row layout: clause c, literal l -> row 4*c + l
// Value encoding (matches sat_submodule.sv / processing_logic.sv):
//   VAL_ZERO = 2'b00, VAL_ONE = 2'b11, VAL_U = 2'b01
//
// Three independent 4-SAT test cases, each preceded by a fresh reset:
//   Test A: SAT via unit-propagation chain ending in DONE
//   Test B: Conflict detection (CONF)
//   Test C: Conflict, UNDO, then re-BCP to SAT
`timescale 1ns/1ps

module sat_submodule_tb;

    localparam VID_WIDTH       = 20;
    localparam NUM_CLAUSES     = 4;
    localparam LITS_PER_CLAUSE = 4;
    localparam CID_WIDTH       = $clog2(NUM_CLAUSES);
    localparam NUM_ROWS        = LITS_PER_CLAUSE * NUM_CLAUSES;
    localparam ROW_ADDR_W      = $clog2(NUM_ROWS);

    localparam OP_IDLE     = 3'd0;
    localparam OP_LOAD     = 3'd1;
    localparam OP_BCP      = 3'd2;
    localparam OP_UNDO     = 3'd3;
    localparam OP_CIT_READ = 3'd4;

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

    longint bcp_call_count    = 0;
    longint bcp_total_cycles  = 0;
    longint undo_call_count   = 0;
    longint undo_total_cycles = 0;
    int     num_errors        = 0;

    task idle_cycle;
        op = OP_IDLE; vid_in = '0; val_in = '0; pol_in = '0;
        row_addr = '0; cid_in = '0; phase = '0;
        @(posedge clk); #1;
    endtask

    task do_reset;
        rst_n = 0; op = OP_IDLE; vid_in = '0; val_in = '0;
        pol_in = '0; row_addr = '0; cid_in = '0; phase = '0;
        repeat(2) @(posedge clk); #1;
        rst_n = 1;
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

    // Load all 4 literals of one clause in one call.
    task load_clause(input int ci,
                     input logic [VID_WIDTH-1:0] v0, input logic p0,
                     input logic [VID_WIDTH-1:0] v1, input logic p1,
                     input logic [VID_WIDTH-1:0] v2, input logic p2,
                     input logic [VID_WIDTH-1:0] v3, input logic p3);
        load_row(LITS_PER_CLAUSE*ci + 0, v0, p0);
        load_row(LITS_PER_CLAUSE*ci + 1, v1, p1);
        load_row(LITS_PER_CLAUSE*ci + 2, v2, p2);
        load_row(LITS_PER_CLAUSE*ci + 3, v3, p3);
    endtask

    // FSM: IDLE→BCP1→BCP2→IDLE.  After a previous BCP, state is left at
    // S_BCP2 (so check() can read outputs).  Each call drains S_BCP2→IDLE
    // first via a single OP_IDLE cycle, then runs BCP1/BCP2 and returns
    // with state==S_BCP2 so the caller's check() can sample valid outputs.
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
        if (conf_out !== exp_conf || up_out !== exp_up || done_out !== exp_done) begin
            $display("FAIL [%s]: conf=%b up=%b done=%b  expected conf=%b up=%b done=%b",
                     name, conf_out, up_out, done_out,
                     exp_conf, exp_up, exp_done);
            num_errors++;
        end else begin
            $display("PASS [%s]: conf=%b up=%b done=%b",
                     name, conf_out, up_out, done_out);
        end
    endtask

    task check_undo(input string name,
                    input logic exp_conf, exp_up, exp_done);
        check(name, exp_conf, exp_up, exp_done);
    endtask

    // ──────────────────────────────────────────────────────────────────
    // Test A: SAT via UP chain
    //   C0:  x1 ∨  x2 ∨  x3 ∨ x4
    //   C1: ~x1 ∨  x2 ∨  x3 ∨ x4
    //   C2:  x1 ∨ ~x2 ∨  x3 ∨ x4
    //   C3: ~x1 ∨ ~x2 ∨ ~x3 ∨ x4
    //   Decisions x1=0, x2=0, x3=0 ⇒ UP forces x4=1 ⇒ DONE.
    // ──────────────────────────────────────────────────────────────────
    task run_test_A;
        $display("\n========== Test A: 4-SAT, UP-chain to DONE ==========");
        do_reset;

        load_clause(0, 20'd1, 1'b0, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(1, 20'd1, 1'b1, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(2, 20'd1, 1'b0, 20'd2, 1'b1, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(3, 20'd1, 1'b1, 20'd2, 1'b1, 20'd3, 1'b1, 20'd4, 1'b0);

        bcp(20'd1, VAL_ZERO);
        check("A:DEC(x1=0)", 1'b0, 1'b0, 1'b0);

        bcp(20'd2, VAL_ZERO);
        check("A:DEC(x2=0)", 1'b0, 1'b0, 1'b0);

        bcp(20'd3, VAL_ZERO);
        check("A:DEC(x3=0)", 1'b0, 1'b1, 1'b0);   // C0 has 3F+1U → UP

        bcp(20'd4, VAL_ONE);
        check("A:UP(x4=1)",  1'b0, 1'b0, 1'b1);   // all clauses satisfied
    endtask

    // ──────────────────────────────────────────────────────────────────
    // Test B: 4-SAT conflict
    //   C0:  x1 ∨  x2 ∨  x3 ∨  x4
    //   C1: ~x1 ∨  x2 ∨  x3 ∨  x4
    //   C2: ~x1 ∨ ~x2 ∨  x3 ∨  x4
    //   C3: ~x1 ∨ ~x2 ∨ ~x3 ∨ ~x4
    //   Decisions x1=1, x2=1, x3=1, x4=1 ⇒ C3 all false ⇒ CONF.
    // ──────────────────────────────────────────────────────────────────
    task run_test_B;
        $display("\n========== Test B: 4-SAT, CONFLICT ==========");
        do_reset;

        load_clause(0, 20'd1, 1'b0, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(1, 20'd1, 1'b1, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(2, 20'd1, 1'b1, 20'd2, 1'b1, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(3, 20'd1, 1'b1, 20'd2, 1'b1, 20'd3, 1'b1, 20'd4, 1'b1);

        bcp(20'd1, VAL_ONE);
        check("B:DEC(x1=1)", 1'b0, 1'b0, 1'b0);

        bcp(20'd2, VAL_ONE);
        check("B:DEC(x2=1)", 1'b0, 1'b0, 1'b0);

        bcp(20'd3, VAL_ONE);
        check("B:DEC(x3=1)", 1'b0, 1'b1, 1'b0);   // C3 has 3F+1U → UP

        bcp(20'd4, VAL_ONE);
        check("B:DEC(x4=1)", 1'b1, 1'b0, 1'b0);   // C3 all F → CONFLICT
    endtask

    // ──────────────────────────────────────────────────────────────────
    // Test C: 4-SAT conflict, UNDO, flip → SAT
    //   Same clauses as Test B.  Decisions x1=1, x2=1, x3=1, x4=1 ⇒ CONF.
    //   Then undo(x4) and bcp(x4=0) ⇒ all clauses satisfied ⇒ DONE.
    // ──────────────────────────────────────────────────────────────────
    task run_test_C;
        $display("\n========== Test C: 4-SAT, CONFLICT → UNDO → SAT ==========");
        do_reset;

        load_clause(0, 20'd1, 1'b0, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(1, 20'd1, 1'b1, 20'd2, 1'b0, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(2, 20'd1, 1'b1, 20'd2, 1'b1, 20'd3, 1'b0, 20'd4, 1'b0);
        load_clause(3, 20'd1, 1'b1, 20'd2, 1'b1, 20'd3, 1'b1, 20'd4, 1'b1);

        bcp(20'd1, VAL_ONE);
        check("C:DEC(x1=1)", 1'b0, 1'b0, 1'b0);

        bcp(20'd2, VAL_ONE);
        check("C:DEC(x2=1)", 1'b0, 1'b0, 1'b0);

        bcp(20'd3, VAL_ONE);
        check("C:DEC(x3=1)", 1'b0, 1'b1, 1'b0);

        bcp(20'd4, VAL_ONE);
        check("C:DEC(x4=1)", 1'b1, 1'b0, 1'b0);   // CONFLICT

        // Backtrack on x4 and try the other polarity
        undo(20'd4);
        check_undo("C:UNDO(x4)", 1'b0, 1'b0, 1'b0);

        bcp(20'd4, VAL_ZERO);
        check("C:FLIP(x4=0)", 1'b0, 1'b0, 1'b1);   // SAT
    endtask

    initial begin
        // Optional sub-test filter via +TEST=A|B|C (default: run all).
        string sel;
        bit    run_a, run_b, run_c;

        if ($value$plusargs("TEST=%s", sel)) begin
            run_a = (sel == "A" || sel == "a");
            run_b = (sel == "B" || sel == "b");
            run_c = (sel == "C" || sel == "c");
            if (!(run_a || run_b || run_c)) begin
                $display("ERROR: unknown +TEST=%s (expected A, B, or C)", sel);
                $finish;
            end
            $display("-- Filter: +TEST=%s --", sel);
        end else begin
            run_a = 1'b1; run_b = 1'b1; run_c = 1'b1;
        end

        $vcdplusfile("waves_4sat.vpd");
        $vcdpluson;

        rst_n = 0; op = OP_IDLE; vid_in = '0; val_in = '0;
        pol_in = '0; row_addr = '0; cid_in = '0; phase = '0;
        repeat(2) @(posedge clk); #1;

        if (run_a) run_test_A;
        if (run_b) run_test_B;
        if (run_c) run_test_C;

        $display("\n-- DONE --");
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
