// SAT Submodule — Flat behavioral model of CAM/SRAM array + processing +
// combining logic + FSM controller for hardware BCP/UNDO/CIT operations.
//
// All sub-module hierarchy from the structural version is inlined here so
// elaboration at NUM_CLAUSES=1024 works without generating per-row C++ classes.
//
// Value encoding (matches hw_bcp_defs.vh):
//   VAL_ZERO = 2'b00
//   VAL_ONE  = 2'b11
//   VAL_U    = 2'b01  (unassigned)
module sat_submodule #(
    parameter VID_WIDTH       = 20,
    parameter NUM_CLAUSES     = 16,
    parameter LITS_PER_CLAUSE = 4,
    parameter CID_WIDTH       = $clog2(NUM_CLAUSES)
)(
    input  logic                                                 clk,
    input  logic                                                 rst_n,

    // Operation control
    input  logic [2:0]                                           op,
    input  logic                                                 phase,

    // Data inputs
    input  logic [VID_WIDTH-1:0]                                 vid_in,
    input  logic [1:0]                                           val_in,
    input  logic                                                 pol_in,
    input  logic [$clog2(LITS_PER_CLAUSE*NUM_CLAUSES)-1:0]       row_addr,
    input  logic [CID_WIDTH-1:0]                                 cid_in,

    // Outputs
    output logic                                                 conf_out,
    output logic                                                 up_out,
    output logic                                                 done_out,
    output logic [CID_WIDTH-1:0]                                 cid_out,
    output logic [1:0]                                           up_lit_pos,
    output logic [VID_WIDTH-1:0]                                 vid_out,
    output logic                                                 pol_out,
    output logic                                                 valid_out
);

    localparam OP_IDLE     = 3'd0;
    localparam OP_LOAD     = 3'd1;
    localparam OP_BCP      = 3'd2;
    localparam OP_UNDO     = 3'd3;
    localparam OP_CIT_READ = 3'd4;

    localparam VAL_ZERO = 2'b00;
    localparam VAL_ONE  = 2'b11;
    localparam VAL_U    = 2'b01;

    localparam NUM_ROWS   = LITS_PER_CLAUSE * NUM_CLAUSES;
    localparam ROW_ADDR_W = $clog2(NUM_ROWS);
    localparam LIT_IDX_W  = $clog2(LITS_PER_CLAUSE);

    // ── Storage arrays ──────────────────────────────────────────────────────
    logic [VID_WIDTH-1:0] vid_store [NUM_ROWS] /*verilator public*/;
    logic                 pol_store [NUM_ROWS] /*verilator public*/;
    logic [1:0]           val_store [NUM_ROWS] /*verilator public*/;
    logic                 row_valid [NUM_ROWS];
    logic                 ml_q      [NUM_ROWS] /*verilator public*/;

    // ── Per-clause processing outputs (combinational) ────────────────────────
    logic       proc_conf [NUM_CLAUSES] /*verilator public*/;
    logic       proc_up   [NUM_CLAUSES] /*verilator public*/;
    logic       proc_done [NUM_CLAUSES] /*verilator public*/;
    logic [1:0] proc_ulp  [NUM_CLAUSES];

    // Intermediate per-clause packed truth-value bits
    logic [LITS_PER_CLAUSE-1:0] c_lit_u  [NUM_CLAUSES];
    logic [LITS_PER_CLAUSE-1:0] c_lit_t  [NUM_CLAUSES];
    logic [LITS_PER_CLAUSE-1:0] c_lit_f  [NUM_CLAUSES];
    logic                       c_ml_any [NUM_CLAUSES];

    // ── Combining outputs (combinational) ────────────────────────────────────
    logic              tree_conf;
    logic              tree_up;
    logic              tree_done;
    logic [CID_WIDTH-1:0] tree_cid;
    logic [1:0]        tree_ulp;

    // ── FSM ─────────────────────────────────────────────────────────────────
    typedef enum logic [2:0] {
        S_IDLE, S_LOAD, S_BCP1, S_BCP2, S_UNDO, S_CIT
    } state_t;
    state_t state;

    logic [LIT_IDX_W-1:0] cit_lit_idx;

    // ── Processing logic — one entry per clause, purely combinational ────────
    always_comb begin
        for (int c = 0; c < NUM_CLAUSES; c++) begin
            c_ml_any[c] = 1'b0;
            for (int i = 0; i < LITS_PER_CLAUSE; i++) begin
                automatic int r = LITS_PER_CLAUSE * c + i;
                c_lit_u[c][i] = row_valid[r] & (val_store[r] == VAL_U);
                // lit true:  not unassigned AND val[0] == ~pol (pol=0→need ONE; pol=1→need ZERO)
                c_lit_t[c][i] = row_valid[r] & ~c_lit_u[c][i]
                                & (val_store[r][0] == ~pol_store[r]);
                // lit false: not unassigned AND val[0] ==  pol
                c_lit_f[c][i] = row_valid[r] & ~c_lit_u[c][i]
                                & (val_store[r][0] ==  pol_store[r]);
                c_ml_any[c]  |= ml_q[r];
            end

            proc_done[c] = |c_lit_t[c];
            proc_conf[c] = &c_lit_f[c];

            // UP: exactly one unassigned, all others false, a matchline was hit
            proc_up[c] = (c_lit_u[c] == LITS_PER_CLAUSE'(1) ||
                          c_lit_u[c] == LITS_PER_CLAUSE'(2) ||
                          c_lit_u[c] == LITS_PER_CLAUSE'(4) ||
                          c_lit_u[c] == LITS_PER_CLAUSE'(8))
                        & (&(c_lit_f[c] | c_lit_u[c]))
                        & ~proc_done[c]
                        & c_ml_any[c];

            // Priority-encode the unassigned literal position
            if      (c_lit_u[c][0]) proc_ulp[c] = 2'd0;
            else if (c_lit_u[c][1]) proc_ulp[c] = 2'd1;
            else if (c_lit_u[c][2]) proc_ulp[c] = 2'd2;
            else                    proc_ulp[c] = 2'd3;
        end
    end

    // ── Combining logic — priority: CONF > UP > DONE ─────────────────────────
    always_comb begin
        tree_conf = 1'b0;
        tree_up   = 1'b0;
        tree_done = 1'b0;
        tree_cid  = '0;
        tree_ulp  = 2'd0;

        // CONF: lowest-index conflicting clause wins (iterate high→low, last write wins)
        for (int i = NUM_CLAUSES-1; i >= 0; i--) begin
            if (proc_conf[i]) begin
                tree_conf = 1'b1;
                tree_cid  = CID_WIDTH'(i);
            end
        end

        if (!tree_conf) begin
            // UP: lowest-index unit clause wins
            for (int i = NUM_CLAUSES-1; i >= 0; i--) begin
                if (proc_up[i]) begin
                    tree_up  = 1'b1;
                    tree_cid = CID_WIDTH'(i);
                    tree_ulp = proc_ulp[i];
                end
            end

            if (!tree_up) begin
                // DONE: all clauses satisfied
                tree_done = 1'b1;
                for (int i = 0; i < NUM_CLAUSES; i++) begin
                    if (!proc_done[i]) tree_done = 1'b0;
                end
            end
        end
    end

    // ── Sequential: storage + FSM ─────────────────────────────────────────────
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state       <= S_IDLE;
            cit_lit_idx <= '0;
            for (int r = 0; r < NUM_ROWS; r++) begin
                vid_store[r] <= '0;
                pol_store[r] <= 1'b0;
                val_store[r] <= VAL_U;
                row_valid[r] <= 1'b0;
                ml_q[r]      <= 1'b0;
            end
        end else begin
            case (state)
                S_IDLE: begin
                    case (op)
                        OP_LOAD:     state <= S_LOAD;
                        OP_BCP:      state <= S_BCP1;
                        OP_UNDO:     state <= S_UNDO;
                        OP_CIT_READ: begin
                            state       <= S_CIT;
                            cit_lit_idx <= '0;
                        end
                        default:     state <= S_IDLE;
                    endcase
                end

                S_LOAD: begin
                    // Write addressed row; value initialised to unassigned
                    vid_store[row_addr] <= vid_in;
                    pol_store[row_addr] <= pol_in;
                    val_store[row_addr] <= VAL_U;
                    row_valid[row_addr] <= 1'b1;
                    state               <= S_IDLE;
                end

                S_BCP1: begin
                    // CAM search: write val_in to every row whose VID matches,
                    // latch matchlines for use by processing logic in BCP2.
                    for (int r = 0; r < NUM_ROWS; r++) begin
                        if (row_valid[r] && vid_store[r] == vid_in) begin
                            val_store[r] <= val_in;
                            ml_q[r]      <= 1'b1;
                        end else begin
                            ml_q[r]      <= 1'b0;
                        end
                    end
                    state <= S_BCP2;
                end

                S_BCP2: begin
                    // Processing/combining logic is purely combinational from
                    // val_store/pol_store/ml_q — outputs already stable.
                    state <= S_IDLE;
                end

                S_UNDO: begin
                    // CAM search: restore VAL_U to every row whose VID matches.
                    for (int r = 0; r < NUM_ROWS; r++) begin
                        if (row_valid[r] && vid_store[r] == vid_in) begin
                            val_store[r] <= VAL_U;
                        end
                    end
                    state <= S_IDLE;
                end

                S_CIT: begin
                    // Sequential read of each literal of clause cid_in
                    if (cit_lit_idx == LIT_IDX_W'(LITS_PER_CLAUSE - 1)) begin
                        state       <= S_IDLE;
                        cit_lit_idx <= '0;
                    end else begin
                        cit_lit_idx <= cit_lit_idx + 1'b1;
                    end
                end

                default: state <= S_IDLE;
            endcase
        end
    end

    // ── Combinational output: addressed row read for BCP2 / CIT ─────────────
    logic [ROW_ADDR_W-1:0] rd_row;
    always_comb begin
        rd_row = '0;
        if (state == S_BCP2 && tree_up) begin
            rd_row = ROW_ADDR_W'(LITS_PER_CLAUSE * tree_cid + tree_ulp);
        end else if (state == S_CIT) begin
            rd_row = ROW_ADDR_W'(LITS_PER_CLAUSE * cid_in + cit_lit_idx);
        end
    end

    // ── Output assignments ───────────────────────────────────────────────────
    assign conf_out   = (state == S_BCP2) ? tree_conf : 1'b0;
    assign up_out     = (state == S_BCP2) ? tree_up   : 1'b0;
    assign done_out   = (state == S_BCP2) ? tree_done : 1'b0;
    assign cid_out    = tree_cid;
    assign up_lit_pos = tree_ulp;
    assign vid_out    = vid_store[rd_row];
    assign pol_out    = pol_store[rd_row];
    assign valid_out  = (state == S_LOAD) | (state == S_BCP2) |
                        (state == S_UNDO) | (state == S_CIT);

endmodule
