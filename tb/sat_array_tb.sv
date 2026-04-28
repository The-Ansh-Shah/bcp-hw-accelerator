// Smoke testbench for sat_array — exercises the full hierarchy:
//   sat_array  →  sat_group_submodule  →  sat_submodule
//
// Geometry is intentionally tiny so simulation is fast:
//   NUM_GROUPS              = 2
//   NUM_SUBSAT_PER_GROUP    = 2
//   NUM_CLAUSES_PER_SUBSAT  = 4
//   LITS_PER_CLAUSE         = 4
//   → 16 clauses spread across 4 sub-SATs in 2 groups (64 rows).
//
// Override the parameters on the dut to instantiate the spec geometry
// (NUM_GROUPS=1024, NUM_SUBSAT_PER_GROUP=8, NUM_CLAUSES_PER_SUBSAT=64).
`timescale 1ns/1ps

module sat_array_tb;

    localparam int VID_WIDTH               = 18;
    localparam int NUM_GROUPS              = 2;
    localparam int NUM_SUBSAT_PER_GROUP    = 2;
    localparam int NUM_CLAUSES_PER_SUBSAT  = 4;
    localparam int LITS_PER_CLAUSE         = 4;

    localparam int SUBSAT_CID_WIDTH = $clog2(NUM_CLAUSES_PER_SUBSAT);
    localparam int SUBSAT_SEL_WIDTH = $clog2(NUM_SUBSAT_PER_GROUP);
    localparam int GROUP_CID_WIDTH  = SUBSAT_CID_WIDTH + SUBSAT_SEL_WIDTH;
    localparam int GROUP_SEL_WIDTH  = $clog2(NUM_GROUPS);
    localparam int ARRAY_CID_WIDTH  = GROUP_CID_WIDTH + GROUP_SEL_WIDTH;

    localparam int NUM_ROWS_TOTAL =
        NUM_GROUPS * NUM_SUBSAT_PER_GROUP * NUM_CLAUSES_PER_SUBSAT * LITS_PER_CLAUSE;
    localparam int ROW_ADDR_W = $clog2(NUM_ROWS_TOTAL);

    localparam logic [2:0] OP_IDLE     = 3'd0;
    localparam logic [2:0] OP_LOAD     = 3'd1;
    localparam logic [2:0] OP_BCP      = 3'd2;
    localparam logic [2:0] OP_UNDO     = 3'd3;

    localparam logic [1:0] VAL_ZERO = 2'b00;
    localparam logic [1:0] VAL_ONE  = 2'b11;
    localparam logic [1:0] VAL_U    = 2'b01;

    logic                          clk, rst_n;
    logic [2:0]                    op;
    logic                          phase;
    logic [VID_WIDTH-1:0]          vid_in;
    logic [1:0]                    val_in;
    logic                          pol_in;
    logic [ROW_ADDR_W-1:0]         row_addr;
    logic [ARRAY_CID_WIDTH-1:0]    cid_in;

    logic                          conf_out, up_out, done_out;
    logic [ARRAY_CID_WIDTH-1:0]    cid_out;
    logic [1:0]                    up_lit_pos;
    logic [VID_WIDTH-1:0]          vid_out;
    logic                          pol_out;
    logic                          valid_out;

    sat_array #(
        .VID_WIDTH              (VID_WIDTH),
        .NUM_GROUPS             (NUM_GROUPS),
        .NUM_SUBSAT_PER_GROUP   (NUM_SUBSAT_PER_GROUP),
        .NUM_CLAUSES_PER_SUBSAT (NUM_CLAUSES_PER_SUBSAT),
        .LITS_PER_CLAUSE        (LITS_PER_CLAUSE)
    ) dut (.*);

    initial clk = 0;
    always #5 clk = ~clk;

    int num_errors = 0;

    // ── Tasks ────────────────────────────────────────────────────────────

    task do_reset;
        rst_n = 0; op = OP_IDLE; vid_in = '0; val_in = '0;
        pol_in = '0; row_addr = '0; cid_in = '0; phase = '0;
        repeat(2) @(posedge clk); #1;
        rst_n = 1;
        @(posedge clk); #1;
    endtask

    // row_of(g, s, c, l): physical row index for group g, sub-SAT s,
    // clause c, literal l.  Layout is { g, s, c, l } from MSB to LSB.
    function automatic int row_of(input int g, input int s,
                                  input int c, input int l);
        return (g << (SUBSAT_SEL_WIDTH
                      + $clog2(NUM_CLAUSES_PER_SUBSAT)
                      + $clog2(LITS_PER_CLAUSE)))
             | (s << ($clog2(NUM_CLAUSES_PER_SUBSAT)
                      + $clog2(LITS_PER_CLAUSE)))
             | (c << $clog2(LITS_PER_CLAUSE))
             | l;
    endfunction

    task load_row(input int                     raddr,
                  input logic [VID_WIDTH-1:0]   vid,
                  input logic                   pol);
        op = OP_LOAD; row_addr = raddr; vid_in = vid;
        pol_in = pol; val_in = VAL_U; phase = '0; cid_in = '0;
        @(posedge clk); #1;
        op = OP_IDLE;
        @(posedge clk); #1;
    endtask

    task load_clause(input int g, input int s, input int c,
                     input logic [VID_WIDTH-1:0] v0, input logic p0,
                     input logic [VID_WIDTH-1:0] v1, input logic p1,
                     input logic [VID_WIDTH-1:0] v2, input logic p2,
                     input logic [VID_WIDTH-1:0] v3, input logic p3);
        load_row(row_of(g, s, c, 0), v0, p0);
        load_row(row_of(g, s, c, 1), v1, p1);
        load_row(row_of(g, s, c, 2), v2, p2);
        load_row(row_of(g, s, c, 3), v3, p3);
    endtask

    task bcp(input logic [VID_WIDTH-1:0] vid, input logic [1:0] val);
        op = OP_IDLE; vid_in = '0; val_in = '0;
        phase = '0; row_addr = '0; pol_in = '0; cid_in = '0;
        @(posedge clk); #1;
        op = OP_BCP; vid_in = vid; val_in = val;
        @(posedge clk); #1;
        @(posedge clk); #1;
        op = OP_IDLE;
    endtask

    task check(input string name,
               input logic exp_conf, exp_up, exp_done);
        string verdict;
        if (conf_out !== exp_conf || up_out !== exp_up || done_out !== exp_done) begin
            verdict = "FAIL"; num_errors++;
        end else begin
            verdict = "PASS";
        end
        $display("%s [%s]: actual {conf=%b up=%b done=%b}  expected {conf=%b up=%b done=%b}",
                 verdict, name,
                 conf_out, up_out, done_out,
                 exp_conf, exp_up, exp_done);
    endtask

    task check_cid(input string name,
                   input logic [ARRAY_CID_WIDTH-1:0] exp_cid);
        if (cid_out !== exp_cid) begin
            $display("FAIL [%s cid]: actual=%0d expected=%0d",
                     name, cid_out, exp_cid);
            num_errors++;
        end else begin
            $display("PASS [%s cid]: cid=%0d", name, cid_out);
        end
    endtask

    // ── Tests ────────────────────────────────────────────────────────────

    // Test A: every clause is (x1 ∨ x2 ∨ x3 ∨ x4); bcp(x1=1) makes every
    // sub-SAT's every clause T, so the whole array reaches DONE.  Verifies
    // that BCP broadcasts down through both levels and DONE ANDs back up.
    task run_test_A;
        $display("\n========== Test A: hierarchy DONE via single broadcast BCP ==========");
        $display("Geometry: %0d groups x %0d sub-SATs x %0d clauses (= %0d clauses total)",
                 NUM_GROUPS, NUM_SUBSAT_PER_GROUP, NUM_CLAUSES_PER_SUBSAT,
                 NUM_GROUPS * NUM_SUBSAT_PER_GROUP * NUM_CLAUSES_PER_SUBSAT);
        $display("Every clause = ( x1 |  x2 |  x3 |  x4 );  bcp(x1=1) -> DONE everywhere.");
        do_reset;

        for (int g = 0; g < NUM_GROUPS; g++)
            for (int s = 0; s < NUM_SUBSAT_PER_GROUP; s++)
                for (int c = 0; c < NUM_CLAUSES_PER_SUBSAT; c++)
                    load_clause(g, s, c,
                                18'd1, 1'b0, 18'd2, 1'b0,
                                18'd3, 1'b0, 18'd4, 1'b0);

        bcp(18'd1, VAL_ONE);
        check("A:DEC(x1=1)", 1'b0, 1'b0, 1'b1);
    endtask

    // Test B: same clauses except the LAST position holds (~x1 ∨ ~x1 ∨ ~x1 ∨ ~x1),
    // which becomes a CONFLICT under bcp(x1=1).  Verifies that CONF wins
    // priority across the hierarchy and that cid_out points exactly at the
    // conflicting clause's global index.
    task run_test_B;
        int gC, sC, cC;
        logic [ARRAY_CID_WIDTH-1:0] exp_cid;
        gC = NUM_GROUPS-1;
        sC = NUM_SUBSAT_PER_GROUP-1;
        cC = 0;
        // Expected global cid = { gC, sC, cC } as a packed bit vector.
        exp_cid = ARRAY_CID_WIDTH'(
                  (gC << (SUBSAT_SEL_WIDTH + SUBSAT_CID_WIDTH))
                | (sC <<  SUBSAT_CID_WIDTH)
                |  cC);

        $display("\n========== Test B: CONF wins priority across the hierarchy ==========");
        $display("All-but-one clauses = ( x1 |  x2 |  x3 |  x4 ); the conflicting clause");
        $display("at (group=%0d, sub-SAT=%0d, clause=%0d) = (~x1 | ~x1 | ~x1 | ~x1).",
                 gC, sC, cC);
        $display("Expected global cid_out = %0d (= {gC,sC,cC}).", exp_cid);
        do_reset;

        for (int g = 0; g < NUM_GROUPS; g++)
            for (int s = 0; s < NUM_SUBSAT_PER_GROUP; s++)
                for (int c = 0; c < NUM_CLAUSES_PER_SUBSAT; c++) begin
                    if (g == gC && s == sC && c == cC)
                        load_clause(g, s, c,
                                    18'd1, 1'b1, 18'd1, 1'b1,
                                    18'd1, 1'b1, 18'd1, 1'b1);
                    else
                        load_clause(g, s, c,
                                    18'd1, 1'b0, 18'd2, 1'b0,
                                    18'd3, 1'b0, 18'd4, 1'b0);
                end

        bcp(18'd1, VAL_ONE);
        check    ("B:DEC(x1=1)", 1'b1, 1'b0, 1'b0);
        check_cid("B:DEC(x1=1)", exp_cid);
    endtask

    initial begin
        $vcdplusfile("waves_array.vpd");
        $vcdpluson;

        rst_n = 0; op = OP_IDLE; vid_in = '0; val_in = '0;
        pol_in = '0; row_addr = '0; cid_in = '0; phase = '0;
        repeat(2) @(posedge clk); #1;

        run_test_A;
        run_test_B;

        $display("\n-- DONE --");
        if (num_errors == 0)
            $display("ALL TESTS PASSED");
        else
            $display("FINISHED WITH %0d ERROR(S)", num_errors);

        $vcdplusoff;
        $finish;
    end

endmodule
