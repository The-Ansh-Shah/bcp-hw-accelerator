// SAT Array — top-level wrapper that aggregates NUM_GROUPS sat_group_submodules
// and combines their CONF/UP/DONE outputs into a single array-level result.
//
// Spec geometry (per architecture diagram):
//   NUM_GROUPS = 32 × 32 = 1024  (logical count; the diagram's H-tree is a
//                                 physical-design choice and is not modeled here)
//   Each group holds 8 sub-SATs × 64 clauses × 4 lits = 2048 rows.
//   Total array clause count = NUM_GROUPS × 8 × 64 = 524288 = 2^19.
//   Combining levels = 6 (sub-SAT) + 3 (group) + 10 (array) = 19.
//
// The default elaboration is the full spec; small-scale instances are
// permitted via parameter overrides for simulation.
module sat_array #(
    parameter int VID_WIDTH              = 18,
    parameter int NUM_GROUPS             = 1024,            // 32 × 32 logical
    parameter int NUM_SUBSAT_PER_GROUP   = 8,
    parameter int NUM_CLAUSES_PER_SUBSAT = 64,
    parameter int LITS_PER_CLAUSE        = 4,

    parameter int SUBSAT_CID_WIDTH = $clog2(NUM_CLAUSES_PER_SUBSAT),
    parameter int SUBSAT_SEL_WIDTH = $clog2(NUM_SUBSAT_PER_GROUP),
    parameter int GROUP_CID_WIDTH  = SUBSAT_CID_WIDTH + SUBSAT_SEL_WIDTH,
    parameter int GROUP_SEL_WIDTH  = $clog2(NUM_GROUPS),
    parameter int ARRAY_CID_WIDTH  = GROUP_CID_WIDTH + GROUP_SEL_WIDTH,

    parameter int NUM_ROWS_PER_SUBSAT  = LITS_PER_CLAUSE * NUM_CLAUSES_PER_SUBSAT,
    parameter int NUM_ROWS_PER_GROUP   = NUM_SUBSAT_PER_GROUP * NUM_ROWS_PER_SUBSAT,
    parameter int NUM_ROWS_TOTAL       = NUM_GROUPS * NUM_ROWS_PER_GROUP,
    parameter int ROW_ADDR_W           = $clog2(NUM_ROWS_TOTAL),
    parameter int GROUP_ROW_ADDR_W     = $clog2(NUM_ROWS_PER_GROUP)
)(
    input  logic                          clk,
    input  logic                          rst_n,

    input  logic [2:0]                    op,
    input  logic                          phase,
    input  logic [VID_WIDTH-1:0]          vid_in,
    input  logic [1:0]                    val_in,
    input  logic                          pol_in,
    input  logic [ROW_ADDR_W-1:0]         row_addr,
    input  logic [ARRAY_CID_WIDTH-1:0]    cid_in,

    output logic                          conf_out,
    output logic                          up_out,
    output logic                          done_out,
    output logic [ARRAY_CID_WIDTH-1:0]    cid_out,
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
    logic [GROUP_SEL_WIDTH-1:0]    sel_load;
    logic [GROUP_SEL_WIDTH-1:0]    sel_cit;
    logic [GROUP_ROW_ADDR_W-1:0]   group_row;
    logic [GROUP_CID_WIDTH-1:0]    group_cid;

    assign sel_load  = row_addr[ROW_ADDR_W-1 -: GROUP_SEL_WIDTH];
    assign group_row = row_addr[GROUP_ROW_ADDR_W-1:0];
    assign sel_cit   = cid_in[ARRAY_CID_WIDTH-1 -: GROUP_SEL_WIDTH];
    assign group_cid = cid_in[GROUP_CID_WIDTH-1:0];

    // ── Per-group signals ────────────────────────────────────────────────
    logic [2:0]                        group_op    [NUM_GROUPS];
    logic                              g_conf      [NUM_GROUPS];
    logic                              g_up        [NUM_GROUPS];
    logic                              g_done      [NUM_GROUPS];
    logic [GROUP_CID_WIDTH-1:0]        g_cid       [NUM_GROUPS];
    logic [1:0]                        g_ulp       [NUM_GROUPS];
    logic [VID_WIDTH-1:0]              g_vid       [NUM_GROUPS];
    logic                              g_pol       [NUM_GROUPS];
    logic                              g_valid     [NUM_GROUPS];

    // ── Operation routing ────────────────────────────────────────────────
    genvar g;
    generate
        for (g = 0; g < NUM_GROUPS; g++) begin : op_route
            always_comb begin
                case (op)
                    OP_LOAD:     group_op[g] = (sel_load == GROUP_SEL_WIDTH'(g)) ? OP_LOAD     : OP_IDLE;
                    OP_CIT_READ: group_op[g] = (sel_cit  == GROUP_SEL_WIDTH'(g)) ? OP_CIT_READ : OP_IDLE;
                    default:     group_op[g] = op;
                endcase
            end
        end
    endgenerate

    // ── Group instances ──────────────────────────────────────────────────
    generate
        for (g = 0; g < NUM_GROUPS; g++) begin : grp_inst
            sat_group_submodule #(
                .VID_WIDTH              (VID_WIDTH),
                .NUM_SUBSAT             (NUM_SUBSAT_PER_GROUP),
                .NUM_CLAUSES_PER_SUBSAT (NUM_CLAUSES_PER_SUBSAT),
                .LITS_PER_CLAUSE        (LITS_PER_CLAUSE)
            ) u_grp (
                .clk        (clk),
                .rst_n      (rst_n),
                .op         (group_op[g]),
                .phase      (phase),
                .vid_in     (vid_in),
                .val_in     (val_in),
                .pol_in     (pol_in),
                .row_addr   (group_row),
                .cid_in     (group_cid),

                .conf_out   (g_conf[g]),
                .up_out     (g_up[g]),
                .done_out   (g_done[g]),
                .cid_out    (g_cid[g]),
                .up_lit_pos (g_ulp[g]),
                .vid_out    (g_vid[g]),
                .pol_out    (g_pol[g]),
                .valid_out  (g_valid[g])
            );
        end
    endgenerate

    // ── Priority combine across groups ───────────────────────────────────
    logic [GROUP_SEL_WIDTH-1:0]   win_sel;
    logic [GROUP_CID_WIDTH-1:0]   win_cid;
    logic [1:0]                   win_ulp;

    always_comb begin
        win_sel  = '0;
        win_cid  = '0;
        win_ulp  = '0;
        conf_out = 1'b0;
        up_out   = 1'b0;

        for (int i = NUM_GROUPS-1; i >= 0; i--) begin
            if (g_conf[i]) begin
                conf_out = 1'b1;
                win_sel  = GROUP_SEL_WIDTH'(i);
                win_cid  = g_cid[i];
            end
        end

        if (!conf_out) begin
            for (int i = NUM_GROUPS-1; i >= 0; i--) begin
                if (g_up[i]) begin
                    up_out  = 1'b1;
                    win_sel = GROUP_SEL_WIDTH'(i);
                    win_cid = g_cid[i];
                    win_ulp = g_ulp[i];
                end
            end
        end
    end

    assign up_lit_pos = win_ulp;
    assign cid_out    = {win_sel, win_cid};

    always_comb begin
        done_out = 1'b1;
        for (int i = 0; i < NUM_GROUPS; i++) begin
            if (!g_done[i]) done_out = 1'b0;
        end
    end

    always_comb begin
        valid_out = 1'b0;
        for (int i = 0; i < NUM_GROUPS; i++) begin
            if (g_valid[i]) valid_out = 1'b1;
        end
    end

    logic [GROUP_SEL_WIDTH-1:0] read_sel;
    assign read_sel = (op == OP_CIT_READ) ? sel_cit : win_sel;

    always_comb begin
        vid_out = g_vid[read_sel];
        pol_out = g_pol[read_sel];
    end

endmodule
