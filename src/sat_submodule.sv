// SAT Submodule — Top-level integration of CAM/SRAM array, processing logic,
// combining logic, and FSM controller for hardware BCP/UNDO/CIT operations.
//
// Default geometry: 16 clauses × 4 literals/clause = 64 rows.
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
    input  logic [2:0]                                           op,       // see OP_* localparams below
    input  logic                                                 phase,    // 0=Phase1, 1=Phase2

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

    // Operation encodings
    localparam OP_IDLE     = 3'd0;
    localparam OP_LOAD     = 3'd1;
    localparam OP_BCP      = 3'd2;
    localparam OP_UNDO     = 3'd3;
    localparam OP_CIT_READ = 3'd4;

    // Value encoding (aligned with hw_bcp_defs.vh and processing_logic.sv)
    localparam VAL_ZERO = 2'b00;
    localparam VAL_ONE  = 2'b11;
    localparam VAL_U    = 2'b01;

    localparam NUM_ROWS    = LITS_PER_CLAUSE * NUM_CLAUSES;
    localparam ROW_ADDR_W  = $clog2(NUM_ROWS);
    localparam LIT_IDX_W   = $clog2(LITS_PER_CLAUSE);

    // --- Array control signals ---
    logic                    search_en;
    logic                    cam_act_wr;
    logic                    addr_wr_en;
    logic                    addr_rd_en;
    logic [ROW_ADDR_W-1:0]   arr_row_addr;
    logic [VID_WIDTH-1:0]    arr_vid_in;
    logic                    arr_pol_in;
    logic [1:0]              arr_val_in;

    // --- Array outputs ---
    logic [VID_WIDTH-1:0]    arr_vid_out;
    logic                    arr_pol_out;
    logic [1:0]              arr_val_out;
    logic                    pol_stored [NUM_ROWS];
    logic [1:0]              val_stored [NUM_ROWS];
    logic                    matchlines   [NUM_ROWS];
    // Registered matchlines: latched at end of BCP1 for use in BCP2.
    // The CAM search is only active in BCP1 (search_en=0 in BCP2), so we must
    // hold the BCP1 matchlines to correctly gate UP detection in BCP2.
    logic                    matchlines_q [NUM_ROWS];

    cam_sram_array #(
        .VID_WIDTH       (VID_WIDTH),
        .NUM_CLAUSES     (NUM_CLAUSES),
        .LITS_PER_CLAUSE (LITS_PER_CLAUSE),
        .NUM_ROWS        (NUM_ROWS)
    ) u_array (
        .clk        (clk),
        .rst_n      (rst_n),
        .search_en  (search_en),
        .cam_act_wr (cam_act_wr),
        .addr_wr_en (addr_wr_en),
        .addr_rd_en (addr_rd_en),
        .row_addr   (arr_row_addr),
        .vid_in     (arr_vid_in),
        .pol_in     (arr_pol_in),
        .val_in     (arr_val_in),
        .vid_out    (arr_vid_out),
        .pol_out    (arr_pol_out),
        .val_out    (arr_val_out),
        .pol_stored (pol_stored),
        .val_stored (val_stored),
        .matchlines (matchlines)
    );

    // --- Processing logic: one per clause, 4 literals each ---
    logic              proc_conf   [NUM_CLAUSES];
    logic              proc_up     [NUM_CLAUSES];
    logic              proc_done   [NUM_CLAUSES];
    logic [1:0]        proc_ulp    [NUM_CLAUSES];

    genvar c;
    generate
        for (c = 0; c < NUM_CLAUSES; c++) begin : proc
            logic [1:0] pval [4];
            logic       ppol [4];
            logic       pml  [4];

            assign pval[0] = val_stored[LITS_PER_CLAUSE*c + 0];
            assign pval[1] = val_stored[LITS_PER_CLAUSE*c + 1];
            assign pval[2] = val_stored[LITS_PER_CLAUSE*c + 2];
            assign pval[3] = val_stored[LITS_PER_CLAUSE*c + 3];
            assign ppol[0] = pol_stored[LITS_PER_CLAUSE*c + 0];
            assign ppol[1] = pol_stored[LITS_PER_CLAUSE*c + 1];
            assign ppol[2] = pol_stored[LITS_PER_CLAUSE*c + 2];
            assign ppol[3] = pol_stored[LITS_PER_CLAUSE*c + 3];
            assign pml[0]  = matchlines_q[LITS_PER_CLAUSE*c + 0];
            assign pml[1]  = matchlines_q[LITS_PER_CLAUSE*c + 1];
            assign pml[2]  = matchlines_q[LITS_PER_CLAUSE*c + 2];
            assign pml[3]  = matchlines_q[LITS_PER_CLAUSE*c + 3];

            processing_logic u_proc (
                .val        (pval),
                .pol        (ppol),
                .ml         (pml),
                .conf       (proc_conf[c]),
                .up         (proc_up[c]),
                .done       (proc_done[c]),
                .up_lit_pos (proc_ulp[c])
            );
        end
    endgenerate

    // --- Combining logic ---
    logic              tree_conf;
    logic              tree_up;
    logic              tree_done;
    logic [CID_WIDTH-1:0] tree_cid;
    logic [1:0]        tree_ulp;

    combining_logic #(
        .NUM_CLAUSES(NUM_CLAUSES),
        .CID_WIDTH  (CID_WIDTH)
    ) u_combine (
        .conf_in       (proc_conf),
        .up_in         (proc_up),
        .done_in       (proc_done),
        .up_lit_pos_in (proc_ulp),
        .conf_out      (tree_conf),
        .up_out        (tree_up),
        .done_out      (tree_done),
        .cid_out       (tree_cid),
        .up_lit_pos_out(tree_ulp)
    );

    // --- FSM ---
    typedef enum logic [2:0] {
        S_IDLE,
        S_LOAD,
        S_BCP1,   // BCP Phase 1: CAM search + CAM-activated SRAM write
        S_BCP2,   // BCP Phase 2: read back VID/pol for UP literal
        S_UNDO,
        S_CIT
    } state_t;

    state_t state;

    // CIT read: LITS_PER_CLAUSE sequential row reads per clause
    logic [LIT_IDX_W-1:0] cit_lit_idx;

    // Latch matchlines at posedge while in BCP1 so they're stable during BCP2.
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (int r = 0; r < NUM_ROWS; r++) matchlines_q[r] <= 1'b0;
        end else if (state == S_BCP1) begin
            for (int r = 0; r < NUM_ROWS; r++) matchlines_q[r] <= matchlines[r];
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state       <= S_IDLE;
            cit_lit_idx <= '0;
        end else begin
            case (state)
                S_IDLE: begin
                    case (op)
                        OP_LOAD:     state <= S_LOAD;
                        OP_BCP:      state <= S_BCP1;
                        OP_UNDO:     state <= S_UNDO;
                        OP_CIT_READ: begin state <= S_CIT; cit_lit_idx <= '0; end
                        default:     state <= S_IDLE;
                    endcase
                end
                S_LOAD:  state <= S_IDLE;
                S_BCP1:  state <= S_BCP2;
                S_BCP2:  state <= S_IDLE;
                S_UNDO:  state <= S_IDLE;
                S_CIT: begin
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

    // --- Combinational control signal generation ---
    always_comb begin
        // Defaults
        search_en    = 1'b0;
        cam_act_wr   = 1'b0;
        addr_wr_en   = 1'b0;
        addr_rd_en   = 1'b0;
        arr_row_addr = row_addr;
        arr_vid_in   = vid_in;
        arr_pol_in   = pol_in;
        arr_val_in   = val_in;
        valid_out    = 1'b0;

        case (state)
            S_LOAD: begin
                // Write VID and polarity to addressed row; initialize value to U
                addr_wr_en = 1'b1;
                arr_val_in = VAL_U;
                valid_out  = 1'b1;
            end
            S_BCP1: begin
                // Search all rows for vid_in; matchlines activate matched rows
                search_en  = 1'b1;
                // CAM-activated SRAM write: update value in all matched rows
                cam_act_wr = 1'b1;
                arr_val_in = val_in;
            end
            S_BCP2: begin
                // Report combining logic results; optionally read UP literal row
                valid_out  = 1'b1;
                if (tree_up) begin
                    // Read back VID and polarity of the UP literal
                    addr_rd_en   = 1'b1;
                    arr_row_addr = ROW_ADDR_W'(tree_cid * LITS_PER_CLAUSE + 32'(tree_ulp));
                end
            end
            S_UNDO: begin
                // Same as BCP search + CAM-activated write, but write U
                search_en  = 1'b1;
                cam_act_wr = 1'b1;
                arr_val_in = VAL_U;
                valid_out  = 1'b1;
            end
            S_CIT: begin
                // Read VID and polarity from each row of clause cid_in
                addr_rd_en   = 1'b1;
                arr_row_addr = ROW_ADDR_W'(cid_in * LITS_PER_CLAUSE + 32'(cit_lit_idx));
                valid_out    = 1'b1;
            end
            default: ;
        endcase
    end

    // --- Output assignments ---
    assign conf_out   = (state == S_BCP2) ? tree_conf : 1'b0;
    assign up_out     = (state == S_BCP2) ? tree_up   : 1'b0;
    assign done_out   = (state == S_BCP2) ? tree_done : 1'b0;
    assign cid_out    = tree_cid;
    assign up_lit_pos = tree_ulp;
    assign vid_out    = arr_vid_out;
    assign pol_out    = arr_pol_out;

endmodule
