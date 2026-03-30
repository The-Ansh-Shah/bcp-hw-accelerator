// Barebones HW-BCP skeleton based on "Current HW-BCP Architecture".
// - Focus: control FSM + explicit CAM/SRAM placeholder interfaces for a test environment.
// - Replace the placeholder CAM/SRAM/eval logic with your real implementations/macros.

package hw_bcp_pkg;
  typedef enum logic [1:0] {
    CLAUSE_DONE     = 2'b00,
    CLAUSE_UNIT     = 2'b01,
    CLAUSE_CONFLICT = 2'b10
  } clause_status_e;

  typedef enum logic [1:0] {
    VAL0  = 2'b00, // false
    VALU  = 2'b01, // undef
    // 2'b10 unused in the diagram
    VAL1  = 2'b11  // true
  } lit_val_e;
endpackage

module hw_bcp_propagate #(
  parameter int unsigned VAR_W          = 10,  // width of var id
  parameter int unsigned LIT_W          = VAR_W + 1, // {sign,var}
  parameter int unsigned CLAUSE_W       = 8,   // width of clause id
  parameter int unsigned LITIDX_W       = 8,   // width of literal index within a clause
  parameter int unsigned REASON_W       = CLAUSE_W,
  parameter int unsigned TRAIL_DEPTH_W  = 10   // trail pointer width
) (
  input  logic                    clk,
  input  logic                    rst_n,

  // Control
  input  logic                    start,
  output logic                    busy,
  output logic                    done,

  // Trail queue interface (pop stream)
  // Assumption: external "trail" holds literals; BCP pops in order qhead..(trail_size-1).
  input  logic [TRAIL_DEPTH_W-1:0] trail_size,
  input  logic [TRAIL_DEPTH_W-1:0] qhead_init,
  // trail_pop_req/valid is a simple decoupled handshake for the test environment
  output logic                    trail_pop_req,
  input  logic                    trail_pop_valid,
  input  logic [LIT_W-1:0]        trail_pop_lit,

  // CAM query interface (placeholder)
  // Assumption: query by var id returns a stream of clause IDs containing that var.
  // - cam_q_valid + cam_q_var starts a query (one per popped literal)
  // - cam_q_ready indicates CAM can accept query
  // - cam_r_valid provides each matching clause id; cam_r_last marks end of stream
  output logic                    cam_q_valid,
  input  logic                    cam_q_ready,
  output logic [VAR_W-1:0]        cam_q_var,
  input  logic                    cam_r_valid,
  output logic                    cam_r_ready,
  input  logic [CLAUSE_W-1:0]    cam_r_clause,
  input  logic                    cam_r_last,

  // SRAM interface (placeholder)
  // The diagram shows an SRAM "value array" and "pol array".
  // For this barebones skeleton, we model a generic literal-value memory:
  // - read value(lit) used when checking if up.lit is FALSE/UNDEF
  // - write assignment when enqueuing a unit propagation (optional for your system)
  output logic                    sram_rd_valid,
  input  logic                    sram_rd_ready,
  output logic [LIT_W-1:0]        sram_rd_lit,
  input  logic                    sram_rd_resp_valid,
  output logic                    sram_rd_resp_ready,
  input  logic [1:0]              sram_rd_val,

  output logic                    sram_wr_valid,
  input  logic                    sram_wr_ready,
  output logic [LIT_W-1:0]        sram_wr_lit,
  output logic [1:0]              sram_wr_val,

  // Clause evaluation interface (placeholder)
  // In the architecture, each scanned clause is "eval_clause(C)" which returns:
  // - status ∈ {CONFLICT, UNIT, DONE}
  // - u_lit (the implied unit literal)
  // - u_idx (literal index in clause) [optional metadata]
  //
  // For a test environment, you can drive these from a simple model; for real HW,
  // plug in the module that walks clause literals using SRAM value/pol arrays.
  output logic                    ce_valid,
  input  logic                    ce_ready,
  output logic [CLAUSE_W-1:0]    ce_clause,
  input  logic                    ce_resp_valid,
  output logic                    ce_resp_ready,
  input  logic [1:0]              ce_status,
  input  logic [LIT_W-1:0]        ce_u_lit,
  input  logic [LITIDX_W-1:0]     ce_u_idx,

  // Outputs
  output logic                    conflict_valid,
  output logic [CLAUSE_W-1:0]    conflict_clause,

  // Unit-propagation candidate (one at a time; consumer can enqueue into trail)
  output logic                    up_valid,
  input  logic                    up_ready,
  output logic [LIT_W-1:0]        up_lit,
  output logic [REASON_W-1:0]     up_reason_clause,
  output logic [LITIDX_W-1:0]     up_lit_idx
);
  import hw_bcp_pkg::*;

  typedef enum logic [3:0] {
    S_IDLE          = 4'd0,
    S_POP_REQ       = 4'd1,
    S_POP_WAIT      = 4'd2,
    S_CAM_Q         = 4'd3,
    S_CAM_STREAM    = 4'd4,
    S_CE_REQ        = 4'd5,
    S_CE_WAIT       = 4'd6,
    S_HANDLE_UNIT   = 4'd7,
    S_LITVAL_REQ    = 4'd8,
    S_LITVAL_WAIT   = 4'd9,
    S_EMIT_UP       = 4'd10,
    S_REPORT_CONFL  = 4'd11,
    S_DONE          = 4'd12
  } state_e;

  state_e state, state_n;

  logic [TRAIL_DEPTH_W-1:0] qhead, qhead_n;
  logic [LIT_W-1:0]         cur_lit, cur_lit_n;
  logic [VAR_W-1:0]         cur_var;

  logic [CLAUSE_W-1:0]      cur_clause, cur_clause_n;
  logic                     cam_eos, cam_eos_n;

  // Track best candidate from this popped literal's CAM scan:
  // - Priority: CONFLICT > UNIT > DONE (DONE means nothing found)
  logic                     have_unit, have_unit_n;
  logic [LIT_W-1:0]         unit_lit, unit_lit_n;
  logic [CLAUSE_W-1:0]      unit_reason, unit_reason_n;
  logic [LITIDX_W-1:0]      unit_idx, unit_idx_n;

  // Conflict latch
  logic                     have_conflict, have_conflict_n;
  logic [CLAUSE_W-1:0]      conflict_c, conflict_c_n;

  // Decoding var from lit: assume MSB is sign, rest is var id.
  assign cur_var = cur_lit[LIT_W-2:0];

  // Defaults
  always_comb begin
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
    sram_rd_lit       = unit_lit; // check candidate lit value
    sram_rd_resp_ready= 1'b0;

    sram_wr_valid     = 1'b0;
    sram_wr_lit       = unit_lit;
    sram_wr_val       = VAL1; // by convention, assignment "true"; external may interpret using sign

    conflict_valid    = 1'b0;
    conflict_clause   = conflict_c;

    up_valid          = 1'b0;
    up_lit            = unit_lit;
    up_reason_clause  = unit_reason;
    up_lit_idx        = unit_idx;

    unique case (state)
      S_IDLE: begin
        if (start) begin
          qhead_n         = qhead_init;
          have_unit_n     = 1'b0;
          have_conflict_n = 1'b0;
          cam_eos_n       = 1'b0;
          state_n         = S_POP_REQ;
        end
      end

      // while qhead < trail_size: pop literal p = trail[qhead++]
      S_POP_REQ: begin
        busy          = 1'b1;
        have_unit_n   = 1'b0;
        cam_eos_n     = 1'b0;
        if (qhead < trail_size) begin
          trail_pop_req = 1'b1;
          state_n       = S_POP_WAIT;
        end else begin
          state_n       = S_DONE;
        end
      end

      S_POP_WAIT: begin
        busy = 1'b1;
        if (trail_pop_valid) begin
          cur_lit_n = trail_pop_lit;
          qhead_n   = qhead + 1'b1;
          state_n   = S_CAM_Q;
        end
      end

      // CAM scan on clauses containing var(cur_lit)
      S_CAM_Q: begin
        busy        = 1'b1;
        cam_q_valid = 1'b1;
        if (cam_q_valid && cam_q_ready) begin
          state_n   = S_CAM_STREAM;
        end
      end

      // For each clause id from CAM, issue eval request
      S_CAM_STREAM: begin
        busy        = 1'b1;
        cam_r_ready = 1'b1;
        if (cam_r_valid) begin
          cur_clause_n = cam_r_clause;
          cam_eos_n    = cam_r_last;
          state_n      = S_CE_REQ;
        end else if (cam_eos) begin
          // Defensive: if your CAM signals EOS without a final valid beat.
          // Prefer to use cam_r_last alongside cam_r_valid in real implementations.
          state_n      = (have_conflict ? S_REPORT_CONFL :
                         (have_unit     ? S_HANDLE_UNIT  : S_POP_REQ));
        end
      end

      S_CE_REQ: begin
        busy     = 1'b1;
        ce_valid = 1'b1;
        if (ce_valid && ce_ready) begin
          state_n = S_CE_WAIT;
        end
      end

      S_CE_WAIT: begin
        busy          = 1'b1;
        ce_resp_ready = 1'b1;
        if (ce_resp_valid) begin
          // Priority: if conflict seen, stop scanning immediately.
          if (ce_status == CLAUSE_CONFLICT) begin
            have_conflict_n = 1'b1;
            conflict_c_n    = cur_clause;
            state_n         = S_REPORT_CONFL;
          end else if ((ce_status == CLAUSE_UNIT) && !have_unit) begin
            have_unit_n     = 1'b1;
            unit_lit_n      = ce_u_lit;
            unit_reason_n   = cur_clause;
            unit_idx_n      = ce_u_idx;
            // Keep scanning for conflicts; otherwise we'll check unit after scan ends.
            state_n         = (cam_eos ? S_HANDLE_UNIT : S_CAM_STREAM);
          end else begin
            // DONE or UNIT but we already captured a candidate unit.
            state_n         = (cam_eos ? (have_unit ? S_HANDLE_UNIT : S_POP_REQ) : S_CAM_STREAM);
          end
        end
      end

      // After scan: if unit candidate exists, check value(unit_lit) in SRAM
      S_HANDLE_UNIT: begin
        busy = 1'b1;
        if (have_conflict) begin
          state_n = S_REPORT_CONFL;
        end else if (have_unit) begin
          state_n = S_LITVAL_REQ;
        end else begin
          state_n = S_POP_REQ;
        end
      end

      S_LITVAL_REQ: begin
        busy         = 1'b1;
        sram_rd_valid= 1'b1;
        if (sram_rd_valid && sram_rd_ready) begin
          state_n = S_LITVAL_WAIT;
        end
      end

      S_LITVAL_WAIT: begin
        busy               = 1'b1;
        sram_rd_resp_ready = 1'b1;
        if (sram_rd_resp_valid) begin
          // Diagram logic:
          // if value(up.lit) == FALSE: return up.reason_clause (treated as conflict)
          // if value(up.lit) == UNDEF: enqueue(up.lit, reason=up.reason_clause)
          if (sram_rd_val == VAL0) begin
            have_conflict_n = 1'b1;
            conflict_c_n    = unit_reason;
            state_n         = S_REPORT_CONFL;
          end else if (sram_rd_val == VALU) begin
            state_n         = S_EMIT_UP;
          end else begin
            // Already assigned TRUE (or otherwise non-undef): nothing to do.
            state_n         = S_POP_REQ;
          end
        end
      end

      // Emit unit-prop to consumer, and (optionally) write SRAM assignment.
      // In many SAT engines, enqueuing affects both trail and assignment memory.
      S_EMIT_UP: begin
        busy     = 1'b1;
        up_valid = 1'b1;
        if (up_valid && up_ready) begin
          // Optional: write assignment for unit literal (placeholder).
          // If your system handles assignments elsewhere, you can ignore this port.
          if (sram_wr_ready) begin
            sram_wr_valid = 1'b1;
            state_n       = S_POP_REQ;
          end else begin
            // Wait until SRAM can accept the write.
            sram_wr_valid = 1'b1;
            state_n       = S_EMIT_UP;
          end
        end
      end

      S_REPORT_CONFL: begin
        busy           = 1'b1;
        conflict_valid = 1'b1;
        // For barebones: single-cycle pulse then stop.
        state_n        = S_DONE;
      end

      S_DONE: begin
        done  = 1'b1;
        busy  = 1'b0;
        // Wait for start to deassert to avoid immediate retrigger.
        if (!start) state_n = S_IDLE;
      end

      default: state_n = S_IDLE;
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state         <= S_IDLE;
      qhead         <= '0;
      cur_lit       <= '0;
      cur_clause    <= '0;
      cam_eos       <= 1'b0;
      have_unit     <= 1'b0;
      unit_lit      <= '0;
      unit_reason   <= '0;
      unit_idx      <= '0;
      have_conflict <= 1'b0;
      conflict_c    <= '0;
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

// -----------------------------------------------------------------------------
// Optional sim-only stubs to make the skeleton runnable in a basic testbench.
// Define HW_BCP_SIM_STUBS to use these.
// -----------------------------------------------------------------------------
`ifdef HW_BCP_SIM_STUBS

module stub_trail #(
  parameter int unsigned LIT_W         = 11,
  parameter int unsigned DEPTH         = 32,
  parameter int unsigned PTR_W         = $clog2(DEPTH)
) (
  input  logic                 clk,
  input  logic                 rst_n,
  input  logic [PTR_W-1:0]     qhead,     // not used (BCP provides qhead_init + internal)
  input  logic [PTR_W-1:0]     trail_size,
  output logic                 pop_valid,
  input  logic                 pop_req,
  output logic [LIT_W-1:0]     pop_lit
);
  logic [LIT_W-1:0] mem [0:DEPTH-1];
  logic [PTR_W-1:0] rd_ptr;
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      rd_ptr    <= '0;
      pop_valid <= 1'b0;
      pop_lit   <= '0;
    end else begin
      pop_valid <= 1'b0;
      if (pop_req && (rd_ptr < trail_size)) begin
        pop_lit   <= mem[rd_ptr];
        rd_ptr    <= rd_ptr + 1'b1;
        pop_valid <= 1'b1;
      end
    end
  end
  // You can initialize mem[] from the testbench with hierarchical references.
endmodule

module stub_cam #(
  parameter int unsigned VAR_W    = 10,
  parameter int unsigned CLAUSE_W = 8
) (
  input  logic                 clk,
  input  logic                 rst_n,
  input  logic                 q_valid,
  output logic                 q_ready,
  input  logic [VAR_W-1:0]     q_var,
  output logic                 r_valid,
  input  logic                 r_ready,
  output logic [CLAUSE_W-1:0]  r_clause,
  output logic                 r_last
);
  // Placeholder behavior: return an empty stream (no clauses) by default.
  // In your testbench, replace with a CAM model that streams matching clause IDs.
  always_comb begin
    q_ready  = 1'b1;
    r_valid  = 1'b0;
    r_clause = '0;
    r_last   = 1'b1;
  end
endmodule

module stub_sram_litval #(
  parameter int unsigned LIT_W = 11
) (
  input  logic            clk,
  input  logic            rst_n,
  input  logic            rd_valid,
  output logic            rd_ready,
  input  logic [LIT_W-1:0] rd_lit,
  output logic            rd_resp_valid,
  input  logic            rd_resp_ready,
  output logic [1:0]      rd_val,
  input  logic            wr_valid,
  output logic            wr_ready,
  input  logic [LIT_W-1:0] wr_lit,
  input  logic [1:0]      wr_val
);
  import hw_bcp_pkg::*;
  // Placeholder behavior: always return UNDEF; accept all writes.
  always_comb begin
    rd_ready       = 1'b1;
    rd_resp_valid  = rd_valid;
    rd_val         = VALU;
    wr_ready       = 1'b1;
  end
endmodule

module stub_clause_eval #(
  parameter int unsigned CLAUSE_W = 8,
  parameter int unsigned LIT_W    = 11,
  parameter int unsigned LITIDX_W = 8
) (
  input  logic                   clk,
  input  logic                   rst_n,
  input  logic                   req_valid,
  output logic                   req_ready,
  input  logic [CLAUSE_W-1:0]    req_clause,
  output logic                   resp_valid,
  input  logic                   resp_ready,
  output logic [1:0]             resp_status,
  output logic [LIT_W-1:0]       resp_u_lit,
  output logic [LITIDX_W-1:0]    resp_u_idx
);
  import hw_bcp_pkg::*;
  // Placeholder: every clause is DONE (no unit/conflict).
  always_comb begin
    req_ready    = 1'b1;
    resp_valid   = req_valid;
    resp_status  = CLAUSE_DONE;
    resp_u_lit   = '0;
    resp_u_idx   = '0;
  end
endmodule

`endif

