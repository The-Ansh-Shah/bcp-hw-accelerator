// SAT Group Submodule — wraps NUM_SUBSAT sub-SAT submodules and combines
// their CONF/UP/DONE outputs into a single group-level result.
//
// Spec geometry (per architecture diagram):
//   NUM_SUBSAT = 8 sub-SATs per group  -> 3 levels of binary sub-combining.
//
// Operation routing:
//   - LOAD / CIT_READ go ONLY to the addressed sub-SAT (decoded from the
//     upper bits of row_addr / cid_in respectively); all other sub-SATs
//     receive OP_IDLE.
//   - BCP / UNDO / IDLE broadcast to every sub-SAT in lockstep.
//
// Combining (CONF > UP > DONE; lowest-index sub-SAT wins ties — equivalent
// to a balanced binary tree of 2-input priority sub-combiners, with output
// CID extended by SUBSAT_SEL_WIDTH bits to identify the winning sub-SAT).
module sat_group_submodule #(
    parameter int VID_WIDTH              = 18,
    parameter int NUM_SUBSAT             = 8,
    parameter int NUM_CLAUSES_PER_SUBSAT = 64,
    parameter int LITS_PER_CLAUSE        = 4,

    parameter int SUBSAT_CID_WIDTH = $clog2(NUM_CLAUSES_PER_SUBSAT),
    parameter int SUBSAT_SEL_WIDTH = $clog2(NUM_SUBSAT),
    parameter int GROUP_CID_WIDTH  = SUBSAT_CID_WIDTH + SUBSAT_SEL_WIDTH,

    parameter int NUM_ROWS_PER_SUBSAT = LITS_PER_CLAUSE * NUM_CLAUSES_PER_SUBSAT,
    parameter int NUM_ROWS_TOTAL      = NUM_SUBSAT * NUM_ROWS_PER_SUBSAT,
    parameter int ROW_ADDR_W          = $clog2(NUM_ROWS_TOTAL),
    parameter int SUBSAT_ROW_ADDR_W   = $clog2(NUM_ROWS_PER_SUBSAT)
)(
    input  logic                          clk,
    input  logic                          rst_n,

    input  logic [2:0]                    op,
    input  logic                          phase,
    input  logic [VID_WIDTH-1:0]          vid_in,
    input  logic [1:0]                    val_in,
    input  logic                          pol_in,
    input  logic [ROW_ADDR_W-1:0]         row_addr,
    input  logic [GROUP_CID_WIDTH-1:0]    cid_in,

    output logic                          conf_out,
    output logic                          up_out,
    output logic                          done_out,
    output logic [GROUP_CID_WIDTH-1:0]    cid_out,
    output logic [1:0]                    up_lit_pos,
    output logic [VID_WIDTH-1:0]          vid_out,
    output logic                          pol_out,
    output logic                          valid_out
);

    localparam logic [2:0] OP_IDLE     = 3'd0;
    localparam logic [2:0] OP_LOAD     = 3'd1;
    localparam logic [2:0] OP_BCP      = 3'd2;
    localparam logic [2:0] OP_UNDO     = 3'd3;
    localparam logic [2:0] OP_CIT_READ = 3'd4;

    // ── Address decode ───────────────────────────────────────────────────
    logic [SUBSAT_SEL_WIDTH-1:0]    sel_load;
    logic [SUBSAT_SEL_WIDTH-1:0]    sel_cit;
    logic [SUBSAT_ROW_ADDR_W-1:0]   subsat_row;
    logic [SUBSAT_CID_WIDTH-1:0]    subsat_cid;

    assign sel_load   = row_addr[ROW_ADDR_W-1 -: SUBSAT_SEL_WIDTH];
    assign subsat_row = row_addr[SUBSAT_ROW_ADDR_W-1:0];
    assign sel_cit    = cid_in[GROUP_CID_WIDTH-1 -: SUBSAT_SEL_WIDTH];
    assign subsat_cid = cid_in[SUBSAT_CID_WIDTH-1:0];

    // ── Per-sub-SAT signals ──────────────────────────────────────────────
    logic [2:0]                       subsat_op   [NUM_SUBSAT];
    logic                             ss_conf     [NUM_SUBSAT];
    logic                             ss_up       [NUM_SUBSAT];
    logic                             ss_done     [NUM_SUBSAT];
    logic [SUBSAT_CID_WIDTH-1:0]      ss_cid      [NUM_SUBSAT];
    logic [1:0]                       ss_ulp      [NUM_SUBSAT];
    logic [VID_WIDTH-1:0]             ss_vid      [NUM_SUBSAT];
    logic                             ss_pol      [NUM_SUBSAT];
    logic                             ss_valid    [NUM_SUBSAT];

    // ── Operation routing ────────────────────────────────────────────────
    genvar s;
    generate
        for (s = 0; s < NUM_SUBSAT; s++) begin : op_route
            always_comb begin
                case (op)
                    OP_LOAD:     subsat_op[s] = (sel_load == SUBSAT_SEL_WIDTH'(s)) ? OP_LOAD     : OP_IDLE;
                    OP_CIT_READ: subsat_op[s] = (sel_cit  == SUBSAT_SEL_WIDTH'(s)) ? OP_CIT_READ : OP_IDLE;
                    default:     subsat_op[s] = op;   // IDLE / BCP / UNDO broadcast to all
                endcase
            end
        end
    endgenerate

    // ── Sub-SAT instances ────────────────────────────────────────────────
    generate
        for (s = 0; s < NUM_SUBSAT; s++) begin : ss_inst
            sat_submodule #(
                .VID_WIDTH       (VID_WIDTH),
                .NUM_CLAUSES     (NUM_CLAUSES_PER_SUBSAT),
                .LITS_PER_CLAUSE (LITS_PER_CLAUSE),
                .CID_WIDTH       (SUBSAT_CID_WIDTH)
            ) u_ss (
                .clk        (clk),
                .rst_n      (rst_n),
                .op         (subsat_op[s]),
                .phase      (phase),
                .vid_in     (vid_in),
                .val_in     (val_in),
                .pol_in     (pol_in),
                .row_addr   (subsat_row),
                .cid_in     (subsat_cid),

                .conf_out   (ss_conf[s]),
                .up_out     (ss_up[s]),
                .done_out   (ss_done[s]),
                .cid_out    (ss_cid[s]),
                .up_lit_pos (ss_ulp[s]),
                .vid_out    (ss_vid[s]),
                .pol_out    (ss_pol[s]),
                .valid_out  (ss_valid[s])
            );
        end
    endgenerate

    // ── Priority combine across sub-SATs ─────────────────────────────────
    // CONF > UP > DONE; lowest-index sub-SAT wins ties (mirrors the
    // bottom-up tree of 2-input sub-combiners shown in the diagram).
    logic [SUBSAT_SEL_WIDTH-1:0]  win_sel;
    logic [SUBSAT_CID_WIDTH-1:0]  win_cid;
    logic [1:0]                   win_ulp;

    always_comb begin
        win_sel    = '0;
        win_cid    = '0;
        win_ulp    = '0;
        conf_out   = 1'b0;
        up_out     = 1'b0;

        // CONF priority — iterate high→low so lowest-index wins (last-write-wins)
        for (int i = NUM_SUBSAT-1; i >= 0; i--) begin
            if (ss_conf[i]) begin
                conf_out = 1'b1;
                win_sel  = SUBSAT_SEL_WIDTH'(i);
                win_cid  = ss_cid[i];
            end
        end

        // UP priority (only if no CONF)
        if (!conf_out) begin
            for (int i = NUM_SUBSAT-1; i >= 0; i--) begin
                if (ss_up[i]) begin
                    up_out  = 1'b1;
                    win_sel = SUBSAT_SEL_WIDTH'(i);
                    win_cid = ss_cid[i];
                    win_ulp = ss_ulp[i];
                end
            end
        end
    end

    assign up_lit_pos = win_ulp;
    assign cid_out    = {win_sel, win_cid};

    // DONE: every sub-SAT must report DONE.
    always_comb begin
        done_out = 1'b1;
        for (int i = 0; i < NUM_SUBSAT; i++) begin
            if (!ss_done[i]) done_out = 1'b0;
        end
    end

    // valid_out: any sub-SAT actively driving outputs.
    always_comb begin
        valid_out = 1'b0;
        for (int i = 0; i < NUM_SUBSAT; i++) begin
            if (ss_valid[i]) valid_out = 1'b1;
        end
    end

    // vid_out / pol_out muxing:
    //   - During CIT_READ, route from the addressed sub-SAT.
    //   - Otherwise (BCP2 UP read), route from the priority winner.
    logic [SUBSAT_SEL_WIDTH-1:0] read_sel;
    assign read_sel = (op == OP_CIT_READ) ? sel_cit : win_sel;

    always_comb begin
        vid_out = ss_vid[read_sel];
        pol_out = ss_pol[read_sel];
    end

endmodule
