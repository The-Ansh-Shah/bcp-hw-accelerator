// ============================================================================
// hw_bcp_propagate — Boolean Constraint Propagation (BCP) engine (FSM)
// ============================================================================
//
// This module walks the implication trail from qhead_init to trail_size,
// and for each assigned literal:
//   1) Queries the CAM for clauses watching that literal's variable.
//   2) Streams each clause to the clause evaluator (CE).
//   3) On UNIT, records the implied literal and (after the CAM stream ends)
//      checks / updates its assignment in SRAM and may emit an implication.
//   4) On CONFLICT, records the conflict clause and eventually reports it.
//
// External interfaces (handshake names mirror the ports):
//   trail_*     — pop literals from the implication queue.
//   cam_*       — content-addressable memory: clauses watching a variable.
//   sram_*      — read/write 2-bit literal values (assignment state).
//   ce_*        — clause evaluation: status + unit literal when UNIT.
//   conflict_*  — output a conflict clause id when propagation fails.
//   up_*        — output one unit implication (literal + reason + index).
//
// Typical macro step: POP → CAM_Q → CAM_STREAM ⟷ CE_REQ/CE_WAIT until the
// clause list for this trail literal is exhausted, then either next POP or
// HANDLE_UNIT → LITVAL (SRAM) → EMIT_UP (write true + emit implication).
//
`include "hw_bcp_defs.vh"

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
  output wire                      done,
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

  // Variable id is encoded in the lower bits of the literal (polarity is MSB).
  assign cur_var = cur_lit[LIT_W-2:0];
  // done follows registered state only (avoids combinational done high the same
  // timestep state_n has returned to idle when start drops).
  assign done     = (state == `S_DONE);

  // Next-state and output logic: single combinational block drives all
  // handshake defaults and state transitions.
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
      // Wait for start; snapshot queue head from qhead_init.
      `S_IDLE: begin
        if (start) begin
          qhead_n         = qhead_init;
          have_unit_n     = 1'b0;
          have_conflict_n = 1'b0;
          cam_eos_n       = 1'b0;
          state_n         = `S_POP_REQ;
        end
      end

      // Request another trail literal if qhead < trail_size, else finish.
      // Do not assert trail_pop_req here — only S_POP_WAIT may request a pop.
      // Otherwise the trail stub can consume a pop on this cycle while the FSM
      // has not yet entered S_POP_WAIT to latch pop_valid, desyncing rd_ptr vs qhead.
      `S_POP_REQ: begin
        busy          = 1'b1;
        have_unit_n   = 1'b0;
        cam_eos_n     = 1'b0;
        if (qhead < trail_size) begin
          state_n       = `S_POP_WAIT;
        end else begin
          state_n       = `S_DONE;
        end
      end

      // Latch popped literal and advance qhead when valid. Hold trail_pop_req
      // until valid so the trail memory can see a stable request (req/ready).
      `S_POP_WAIT: begin
        busy            = 1'b1;
        trail_pop_req   = 1'b1;
        if (trail_pop_valid) begin
          cur_lit_n = trail_pop_lit;
          qhead_n   = qhead + 1'b1;
          state_n   = `S_CAM_Q;
        end
      end

      // Issue CAM query for cur_var (clauses watching this variable).
      `S_CAM_Q: begin
        busy        = 1'b1;
        cam_q_valid = 1'b1;
        // cam_q_valid is driven high in this state; do not use (cam_q_valid && ...)
        // in the condition — reading the output port here can miss the update.
        if (cam_q_ready) begin
          state_n   = `S_CAM_STREAM;
        end
      end

      // Accept clause ids from CAM until r_last; between clauses go to CE.
      // If no r_valid but cam_eos (end of stream), branch to conflict report,
      // unit handling, or next POP.
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

      // Handshake clause id into the clause evaluator.
      `S_CE_REQ: begin
        busy     = 1'b1;
        ce_valid = 1'b1;
        if (ce_ready) begin
          state_n = `S_CE_WAIT;
        end
      end

      // Latch CE result: conflict stops the watch list; first UNIT wins;
      // otherwise continue streaming or return to POP.
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

      // After CAM stream: resolve conflict vs unit vs continue trail.
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

      // Read current assignment of the unit literal from SRAM.
      `S_LITVAL_REQ: begin
        busy         = 1'b1;
        sram_rd_valid= 1'b1;
        if (sram_rd_ready) begin
          state_n = `S_LITVAL_WAIT;
        end
      end

      // VAL0 → conflict (reason clause); VALU → emit implication; else skip.
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

      // Assert implication (up_*) and write literal to VAL1; retry if wr busy.
      `S_EMIT_UP: begin
        busy     = 1'b1;
        up_valid = 1'b1;
        if (up_ready) begin
          if (sram_wr_ready) begin
            sram_wr_valid = 1'b1;
            state_n       = `S_POP_REQ;
          end else begin
            sram_wr_valid = 1'b1;
            state_n       = `S_EMIT_UP;
          end
        end
      end

      // Pulse conflict output once, then done.
      `S_REPORT_CONFL: begin
        busy           = 1'b1;
        conflict_valid = 1'b1;
        state_n        = `S_DONE;
      end

      // Terminal: done = (state==S_DONE); exit to idle when start deasserts.
      `S_DONE: begin
        busy    = 1'b0;
        state_n = start ? `S_DONE : `S_IDLE;
      end

      default: state_n = `S_IDLE;
    endcase
  end

  // Synchronous active-low reset (asserted rst_n clears state on the next posedge clk).
  always @(posedge clk) begin
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
