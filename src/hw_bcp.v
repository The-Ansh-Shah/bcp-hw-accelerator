`define CLAUSE_DONE     2'b00
`define CLAUSE_UNIT     2'b01
`define CLAUSE_CONFLICT 2'b10
`define VAL0            2'b00
`define VALU            2'b01
`define VAL1            2'b11

// FSM state encoding
`define S_IDLE          4'd0
`define S_POP_REQ       4'd1
`define S_POP_WAIT      4'd2
`define S_CAM_Q         4'd3
`define S_CAM_STREAM    4'd4
`define S_CE_REQ        4'd5
`define S_CE_WAIT       4'd6
`define S_HANDLE_UNIT   4'd7
`define S_LITVAL_REQ    4'd8
`define S_LITVAL_WAIT   4'd9
`define S_EMIT_UP       4'd10
`define S_REPORT_CONFL  4'd11
`define S_DONE          4'd12

module hw_bcp_propagate #(
  parameter VAR_W         = 10,
  parameter LIT_W         = 11,
  parameter CLAUSE_W      = 8,
  parameter LITIDX_W      = 8,
  parameter REASON_W      = 8,
  parameter TRAIL_DEPTH_W = 10
) (
  input  wire                      clk,
  input  wire                      rst_n,
  input  wire                      start,
  output reg                       busy,
  output reg                       done,
  input  wire [TRAIL_DEPTH_W-1:0]  trail_size,
  input  wire [TRAIL_DEPTH_W-1:0]  qhead_init,
  output reg                       trail_pop_req,
  input  wire                      trail_pop_valid,
  input  wire [LIT_W-1:0]          trail_pop_lit,
  output reg                       cam_q_valid,
  input  wire                      cam_q_ready,
  output reg [VAR_W-1:0]           cam_q_var,
  input  wire                      cam_r_valid,
  output reg                       cam_r_ready,
  input  wire [CLAUSE_W-1:0]       cam_r_clause,
  input  wire                      cam_r_last,
  output reg                       sram_rd_valid,
  input  wire                      sram_rd_ready,
  output reg [LIT_W-1:0]           sram_rd_lit,
  input  wire                      sram_rd_resp_valid,
  output reg                       sram_rd_resp_ready,
  input  wire [1:0]                 sram_rd_val,
  output reg                       sram_wr_valid,
  input  wire                      sram_wr_ready,
  output reg [LIT_W-1:0]           sram_wr_lit,
  output reg [1:0]                 sram_wr_val,
  output reg                       ce_valid,
  input  wire                      ce_ready,
  output reg [CLAUSE_W-1:0]        ce_clause,
  input  wire                      ce_resp_valid,
  output reg                       ce_resp_ready,
  input  wire [1:0]                ce_status,
  input  wire [LIT_W-1:0]          ce_u_lit,
  input  wire [LITIDX_W-1:0]       ce_u_idx,
  output reg                       conflict_valid,
  output reg [CLAUSE_W-1:0]        conflict_clause,
  output reg                       up_valid,
  input  wire                      up_ready,
  output reg [LIT_W-1:0]           up_lit,
  output reg [REASON_W-1:0]        up_reason_clause,
  output reg [LITIDX_W-1:0]        up_lit_idx
);

  reg [3:0] state;
  reg [3:0] state_n;

  reg [TRAIL_DEPTH_W-1:0] qhead, qhead_n;
  reg [LIT_W-1:0]         cur_lit, cur_lit_n;
  wire [VAR_W-1:0]        cur_var;

  reg [CLAUSE_W-1:0]      cur_clause, cur_clause_n;
  reg                     cam_eos, cam_eos_n;

  reg                     have_unit, have_unit_n;
  reg [LIT_W-1:0]         unit_lit, unit_lit_n;
  reg [CLAUSE_W-1:0]      unit_reason, unit_reason_n;
  reg [LITIDX_W-1:0]      unit_idx, unit_idx_n;

  reg                     have_conflict, have_conflict_n;
  reg [CLAUSE_W-1:0]      conflict_c, conflict_c_n;

  assign cur_var = cur_lit[LIT_W-2:0];

  always @(*) begin
    state_n           = state;
    qhead_n           = qhead;
    cur_lit_n         = cur_lit;
    cur_clause_n      = cur_clause;
    cam_eos_n         = cam_eos;
    have_unit_n       = have_unit;
    unit_lit_n        = unit_lit;
    unit_reason_n     = unit_reason;
    unit_idx_n        = unit_idx;
    have_conflict_n   = have_conflict;
    conflict_c_n      = conflict_c;

    busy              = 1'b0;
    done              = 1'b0;

    trail_pop_req     = 1'b0;

    cam_q_valid       = 1'b0;
    cam_q_var         = cur_var;
    cam_r_ready       = 1'b0;

    ce_valid          = 1'b0;
    ce_clause         = cur_clause;
    ce_resp_ready     = 1'b0;

    sram_rd_valid     = 1'b0;
    sram_rd_lit       = unit_lit;
    sram_rd_resp_ready= 1'b0;

    sram_wr_valid     = 1'b0;
    sram_wr_lit       = unit_lit;
    sram_wr_val       = `VAL1;

    conflict_valid    = 1'b0;
    conflict_clause   = conflict_c;

    up_valid          = 1'b0;
    up_lit            = unit_lit;
    up_reason_clause  = unit_reason;
    up_lit_idx        = unit_idx;

    case (state)
      `S_IDLE: begin
        if (start) begin
          qhead_n         = qhead_init;
          have_unit_n     = 1'b0;
          have_conflict_n = 1'b0;
          cam_eos_n       = 1'b0;
          state_n         = `S_POP_REQ;
        end
      end

      `S_POP_REQ: begin
        busy          = 1'b1;
        have_unit_n   = 1'b0;
        cam_eos_n     = 1'b0;
        if (qhead < trail_size) begin
          trail_pop_req = 1'b1;
          state_n       = `S_POP_WAIT;
        end else begin
          state_n       = `S_DONE;
        end
      end

      `S_POP_WAIT: begin
        busy = 1'b1;
        if (trail_pop_valid) begin
          cur_lit_n = trail_pop_lit;
          qhead_n   = qhead + 1'b1;
          state_n   = `S_CAM_Q;
        end
      end

      `S_CAM_Q: begin
        busy        = 1'b1;
        cam_q_valid = 1'b1;
        if (cam_q_valid && cam_q_ready) begin
          state_n   = `S_CAM_STREAM;
        end
      end

      `S_CAM_STREAM: begin
        busy        = 1'b1;
        cam_r_ready = 1'b1;
        if (cam_r_valid) begin
          cur_clause_n = cam_r_clause;
          cam_eos_n    = cam_r_last;
          state_n      = `S_CE_REQ;
        end else if (cam_eos) begin
          state_n      = have_conflict ? `S_REPORT_CONFL :
                         (have_unit     ? `S_HANDLE_UNIT  : `S_POP_REQ);
        end
      end

      `S_CE_REQ: begin
        busy     = 1'b1;
        ce_valid = 1'b1;
        if (ce_valid && ce_ready) begin
          state_n = `S_CE_WAIT;
        end
      end

      `S_CE_WAIT: begin
        busy          = 1'b1;
        ce_resp_ready = 1'b1;
        if (ce_resp_valid) begin
          if (ce_status == `CLAUSE_CONFLICT) begin
            have_conflict_n = 1'b1;
            conflict_c_n    = cur_clause;
            state_n         = `S_REPORT_CONFL;
          end else if ((ce_status == `CLAUSE_UNIT) && !have_unit) begin
            have_unit_n     = 1'b1;
            unit_lit_n      = ce_u_lit;
            unit_reason_n   = cur_clause;
            unit_idx_n      = ce_u_idx;
            state_n         = cam_eos ? `S_HANDLE_UNIT : `S_CAM_STREAM;
          end else begin
            state_n         = cam_eos ? (have_unit ? `S_HANDLE_UNIT : `S_POP_REQ) : `S_CAM_STREAM;
          end
        end
      end

      `S_HANDLE_UNIT: begin
        busy = 1'b1;
        if (have_conflict) begin
          state_n = `S_REPORT_CONFL;
        end else if (have_unit) begin
          state_n = `S_LITVAL_REQ;
        end else begin
          state_n = `S_POP_REQ;
        end
      end

      `S_LITVAL_REQ: begin
        busy         = 1'b1;
        sram_rd_valid= 1'b1;
        if (sram_rd_valid && sram_rd_ready) begin
          state_n = `S_LITVAL_WAIT;
        end
      end

      `S_LITVAL_WAIT: begin
        busy               = 1'b1;
        sram_rd_resp_ready = 1'b1;
        if (sram_rd_resp_valid) begin
          if (sram_rd_val == `VAL0) begin
            have_conflict_n = 1'b1;
            conflict_c_n    = unit_reason;
            state_n         = `S_REPORT_CONFL;
          end else if (sram_rd_val == `VALU) begin
            state_n         = `S_EMIT_UP;
          end else begin
            state_n         = `S_POP_REQ;
          end
        end
      end

      `S_EMIT_UP: begin
        busy     = 1'b1;
        up_valid = 1'b1;
        if (up_valid && up_ready) begin
          if (sram_wr_ready) begin
            sram_wr_valid = 1'b1;
            state_n       = `S_POP_REQ;
          end else begin
            sram_wr_valid = 1'b1;
            state_n       = `S_EMIT_UP;
          end
        end
      end

      `S_REPORT_CONFL: begin
        busy           = 1'b1;
        conflict_valid = 1'b1;
        state_n        = `S_DONE;
      end

      `S_DONE: begin
        done  = 1'b1;
        busy  = 1'b0;
        if (!start) state_n = `S_IDLE;
      end

      default: state_n = `S_IDLE;
    endcase
  end

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state         <= `S_IDLE;
      qhead         <= {TRAIL_DEPTH_W{1'b0}};
      cur_lit       <= {LIT_W{1'b0}};
      cur_clause    <= {CLAUSE_W{1'b0}};
      cam_eos       <= 1'b0;
      have_unit     <= 1'b0;
      unit_lit      <= {LIT_W{1'b0}};
      unit_reason   <= {CLAUSE_W{1'b0}};
      unit_idx      <= {LITIDX_W{1'b0}};
      have_conflict <= 1'b0;
      conflict_c    <= {CLAUSE_W{1'b0}};
    end else begin
      state         <= state_n;
      qhead         <= qhead_n;
      cur_lit       <= cur_lit_n;
      cur_clause    <= cur_clause_n;
      cam_eos       <= cam_eos_n;
      have_unit     <= have_unit_n;
      unit_lit      <= unit_lit_n;
      unit_reason   <= unit_reason_n;
      unit_idx      <= unit_idx_n;
      have_conflict <= have_conflict_n;
      conflict_c    <= conflict_c_n;
    end
  end

endmodule

`ifdef HW_BCP_SIM_STUBS

module stub_trail #(
  parameter LIT_W  = 11,
  parameter DEPTH  = 32,
  parameter PTR_W  = 5
) (
  input  wire                 clk,
  input  wire                 rst_n,
  input  wire [PTR_W-1:0]     qhead,
  input  wire [PTR_W-1:0]     trail_size,
  output reg                  pop_valid,
  input  wire                 pop_req,
  output reg [LIT_W-1:0]      pop_lit
);
  reg [LIT_W-1:0] mem [0:DEPTH-1];
  reg [PTR_W-1:0] rd_ptr;

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      rd_ptr    <= {PTR_W{1'b0}};
      pop_valid <= 1'b0;
      pop_lit   <= {LIT_W{1'b0}};
    end else begin
      pop_valid <= 1'b0;
      if (pop_req && (rd_ptr < trail_size)) begin
        pop_lit   <= mem[rd_ptr];
        rd_ptr    <= rd_ptr + 1'b1;
        pop_valid <= 1'b1;
      end
    end
  end
endmodule

module stub_cam #(
  parameter VAR_W    = 10,
  parameter CLAUSE_W = 8
) (
  input  wire                 clk,
  input  wire                 rst_n,
  input  wire                 q_valid,
  output reg                  q_ready,
  input  wire [VAR_W-1:0]     q_var,
  output reg                  r_valid,
  input  wire                 r_ready,
  output reg [CLAUSE_W-1:0]   r_clause,
  output reg                  r_last
);
  always @(*) begin
    q_ready  = 1'b1;
    r_valid  = 1'b0;
    r_clause = {CLAUSE_W{1'b0}};
    r_last   = 1'b1;
  end
endmodule

module stub_sram_litval #(
  parameter LIT_W = 11
) (
  input  wire                 clk,
  input  wire                 rst_n,
  input  wire                 rd_valid,
  output reg                  rd_ready,
  input  wire [LIT_W-1:0]     rd_lit,
  output reg                  rd_resp_valid,
  input  wire                 rd_resp_ready,
  output reg [1:0]            rd_val,
  input  wire                 wr_valid,
  output reg                  wr_ready,
  input  wire [LIT_W-1:0]     wr_lit,
  input  wire [1:0]            wr_val
);
  always @(*) begin
    rd_ready       = 1'b1;
    rd_resp_valid  = rd_valid;
    rd_val         = `VALU;
    wr_ready       = 1'b1;
  end
endmodule

module stub_clause_eval #(
  parameter CLAUSE_W  = 8,
  parameter LIT_W     = 11,
  parameter LITIDX_W  = 8
) (
  input  wire                   clk,
  input  wire                   rst_n,
  input  wire                   req_valid,
  output reg                    req_ready,
  input  wire [CLAUSE_W-1:0]    req_clause,
  output reg                    resp_valid,
  input  wire                   resp_ready,
  output reg [1:0]              resp_status,
  output reg [LIT_W-1:0]        resp_u_lit,
  output reg [LITIDX_W-1:0]     resp_u_idx
);
  always @(*) begin
    req_ready    = 1'b1;
    resp_valid   = req_valid;
    resp_status  = `CLAUSE_DONE;
    resp_u_lit   = {LIT_W{1'b0}};
    resp_u_idx   = {LITIDX_W{1'b0}};
  end
endmodule

`endif
