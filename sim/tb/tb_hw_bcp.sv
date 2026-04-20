`timescale 1ns/1ps
`include "hw_bcp_defs.vh"

// ============================================================================
// tb_hw_bcp — HW-BCP propagate FSM testbench
// ============================================================================
// The DUT (hw_bcp_propagate) and stub_trail use synchronous active-low reset:
// after releasing rst_n, wait at least one posedge clk before driving meaningful
// control (apply_reset does this).
//
// Stub infrastructure (see module definitions below):
//   tb_cam_lut  — programmable LUT-based CAM: per-variable watchlists of up to
//                 MAX_PER_VAR clause IDs, programmed via write tasks before each
//                 test.  Replaces the fixed tb_cam_empty_watchlist.
//   tb_ce_lut   — programmable LUT-based clause evaluator: per-clause-ID status
//                 (DONE/UNIT/CONFLICT) + unit-literal info, programmed via write
//                 tasks.  Replaces the single-response tb_ce_prog.
//
// Behavioral coverage:
//   Test  1 — idle outputs before start (busy=0, done=0)
//   Test  2 — zero-length trail completes without pop
//   Test  3 — n-literal trail, all clauses evaluate to DONE (no unit props)
//   Test  4 — done holds while start is high; deasserts once start drops
//   Test  5 — unit propagation: CE returns UNIT → up_valid asserted
//   Test  6 — conflict detection: CE returns CONFLICT → conflict_valid asserted
//   Test  7 — 3-SAT BCP pass (random-3-SAT, SAT region): trail={¬x3}, clause C3
//              becomes UNIT for x6=T; verifies single unit implication
//   Test  8 — 3-SAT forced CONFLICT: trail={x3=T}, clause C4 all-false → conflict
//   Test  9 — 4-SAT all-DONE BCP pass: trail={x1,x2,x3}, 2 clauses/var all DONE
//   Test 10 — 3-SAT sequential unit propagations: trail={x1,x2}, each trail
//              literal causes one UNIT implication (x5=T then x6=T)
//
// CNF instances for tests 7-10 are small instances representative of the
// "random-3-SAT" and "random-4-SAT" families hosted at benchmark-database.de
// (satisfiable region: ratio well below the 3-SAT phase transition ~4.27).
// All instances have ≤ 8 variables and ≤ 8 clauses to stay within the 6-bit
// CAM/SRAM address widths and 32-entry trail stub.
// ============================================================================

// ----------------------------------------------------------------------------
// tb_cam_lut — programmable watchlist CAM.
//
// For each variable id, stores up to MAX_PER_VAR clause IDs in watch_clauses[]
// and the count in watch_count[].  On a query for variable v:
//   • emits watch_count[v] beats, one clause ID per beat
//   • asserts r_last on the final beat
//   • if watch_count[v]==0, still emits one beat with clause=0, r_last=1 so the
//     DUT's CAM_STREAM state never stalls waiting for data
//
// Default (set in initial block): every variable → count=1, clause=0.
// Tests reprogram only the variables they actually put on the trail.
// ----------------------------------------------------------------------------
module tb_cam_lut #(
  parameter int unsigned VAR_W       = 6,
  parameter int unsigned CLAUSE_W    = 6,
  parameter int unsigned MAX_VARS    = 64,
  parameter int unsigned MAX_PER_VAR = 4
) (
  input  logic clk,
  input  logic rst_n,
  input  logic                 q_valid,
  output logic                 q_ready,
  input  logic [VAR_W-1:0]     q_var,
  output logic                 r_valid,
  input  logic                 r_ready,
  output logic [CLAUSE_W-1:0]  r_clause,
  output logic                 r_last
);
  logic [CLAUSE_W-1:0] watch_clauses [MAX_VARS][MAX_PER_VAR];
  logic [2:0]          watch_count   [MAX_VARS];   // 0..MAX_PER_VAR

  logic        emitting_r;
  logic [VAR_W-1:0] cur_var_r;
  logic [2:0]       cur_idx_r;

  // Accept new query only when idle.
  assign q_ready = ~emitting_r;

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      emitting_r  <= 1'b0;
      cur_var_r   <= '0;
      cur_idx_r   <= '0;
    end else begin
      if (!emitting_r) begin
        if (q_valid) begin
          cur_var_r  <= q_var;
          cur_idx_r  <= '0;
          emitting_r <= 1'b1;
        end
      end else begin
        if (r_valid && r_ready) begin
          if (r_last)
            emitting_r <= 1'b0;
          else
            cur_idx_r  <= cur_idx_r + 3'b1;
        end
      end
    end
  end

  wire [2:0] eff_cnt = (watch_count[cur_var_r] == 3'd0) ? 3'd1 : watch_count[cur_var_r];

  assign r_valid  = emitting_r;
  assign r_clause = (watch_count[cur_var_r] == 3'd0) ? '0
                                                       : watch_clauses[cur_var_r][cur_idx_r];
  assign r_last   = (cur_idx_r == eff_cnt - 3'd1);

  // Default: every variable maps to a single-entry watchlist returning clause 0.
  initial begin
    for (int v = 0; v < MAX_VARS; v++) begin
      watch_count[v] = 3'd1;
      for (int i = 0; i < MAX_PER_VAR; i++)
        watch_clauses[v][i] = '0;
    end
  end
endmodule

// ----------------------------------------------------------------------------
// tb_ce_lut — programmable clause evaluator.
//
// Stores per-clause-id response (status, unit literal, unit index) in arrays.
// When a request for clause C arrives, the registered response is the content
// of clause_status[C], clause_u_lit[C], clause_u_idx[C].
//
// Default (set in initial block): all clauses → CLAUSE_DONE, lit=0, idx=0.
// ----------------------------------------------------------------------------
module tb_ce_lut #(
  parameter int unsigned CLAUSE_W    = 6,
  parameter int unsigned LIT_W       = 7,
  parameter int unsigned LITIDX_W    = 6,
  parameter int unsigned MAX_CLAUSES = 64
) (
  input  logic clk,
  input  logic rst_n,
  input  logic                   req_valid,
  output logic                   req_ready,
  input  logic [CLAUSE_W-1:0]    req_clause,
  output logic                   resp_valid,
  input  logic                   resp_ready,
  output logic [1:0]             resp_status,
  output logic [LIT_W-1:0]       resp_u_lit,
  output logic [LITIDX_W-1:0]    resp_u_idx
);
  logic [1:0]          clause_status [MAX_CLAUSES];
  logic [LIT_W-1:0]    clause_u_lit  [MAX_CLAUSES];
  logic [LITIDX_W-1:0] clause_u_idx  [MAX_CLAUSES];

  logic        resp_valid_r;
  logic [1:0]          resp_status_r;
  logic [LIT_W-1:0]    resp_u_lit_r;
  logic [LITIDX_W-1:0] resp_u_idx_r;

  assign req_ready   = 1'b1;
  assign resp_valid  = resp_valid_r;
  assign resp_status = resp_status_r;
  assign resp_u_lit  = resp_u_lit_r;
  assign resp_u_idx  = resp_u_idx_r;

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      resp_valid_r <= 1'b0;
    end else begin
      if (resp_valid_r && resp_ready) begin
        resp_valid_r <= 1'b0;
      end else if (req_valid && req_ready) begin
        resp_valid_r  <= 1'b1;
        resp_status_r <= clause_status[req_clause];
        resp_u_lit_r  <= clause_u_lit[req_clause];
        resp_u_idx_r  <= clause_u_idx[req_clause];
      end
    end
  end

  initial begin
    for (int c = 0; c < MAX_CLAUSES; c++) begin
      clause_status[c] = `CLAUSE_DONE;
      clause_u_lit[c]  = '0;
      clause_u_idx[c]  = '0;
    end
  end
endmodule

// ============================================================================
// Top-level testbench
// ============================================================================
module tb_hw_bcp;

  localparam int unsigned VAR_W         = 6;
  localparam int unsigned LIT_W         = VAR_W + 1;   // 7
  localparam int unsigned CLAUSE_W      = 6;
  localparam int unsigned LITIDX_W      = 6;
  localparam int unsigned REASON_W      = CLAUSE_W;
  localparam int unsigned TRAIL_PTR_W   = 6;

  localparam int unsigned TIMEOUT_CYCLES = 500;

  logic clk;
  logic rst_n;
  always #5 clk = ~clk;

  // DUT control
  logic start, busy, done;

  // Trail
  logic [TRAIL_PTR_W-1:0] trail_size, qhead_init;
  logic trail_pop_req, trail_pop_valid;
  logic [LIT_W-1:0] trail_pop_lit;

  // CAM
  logic cam_q_valid, cam_q_ready;
  logic [VAR_W-1:0] cam_q_var;
  logic cam_r_valid, cam_r_ready;
  logic [CLAUSE_W-1:0] cam_r_clause;
  logic cam_r_last;

  // SRAM
  logic sram_rd_valid, sram_rd_ready;
  logic [LIT_W-1:0] sram_rd_lit;
  logic sram_rd_resp_valid, sram_rd_resp_ready;
  logic [1:0] sram_rd_val;
  logic sram_wr_valid, sram_wr_ready;
  logic [LIT_W-1:0] sram_wr_lit;
  logic [1:0] sram_wr_val;

  // Clause eval
  logic ce_valid, ce_ready;
  logic [CLAUSE_W-1:0] ce_clause;
  logic ce_resp_valid, ce_resp_ready;
  logic [1:0] ce_status;
  logic [LIT_W-1:0] ce_u_lit;
  logic [LITIDX_W-1:0] ce_u_idx;

  // Outputs
  logic conflict_valid;
  logic [CLAUSE_W-1:0] conflict_clause;
  logic up_valid, up_ready;
  logic [LIT_W-1:0] up_lit;
  logic [REASON_W-1:0] up_reason_clause;
  logic [LITIDX_W-1:0] up_lit_idx;

  // Test bookkeeping
  int unsigned tests_passed;
  int unsigned tests_failed;

  // -------------------------------------------------------------------------
  // Cycle counter and per-test signal monitors.
  // clear_monitors() resets the latches; always block sets them.
  // up_lit_log records each UP in order; up_count is the total count seen.
  // -------------------------------------------------------------------------
  int unsigned          cycle_count;
  logic                 up_valid_seen;
  logic [LIT_W-1:0]     up_lit_latched;
  logic [LIT_W-1:0]     up_lit_log [0:7];
  int unsigned          up_count;
  logic                 conflict_seen;
  logic [CLAUSE_W-1:0]  conflict_clause_latched;

  // -------------------------------------------------------------------------
  // Stub latency tracking.
  // -------------------------------------------------------------------------
  int unsigned stub_trail_req_cyc;
  logic        stub_trail_req_pending;
  int unsigned stub_cam_q_cyc;
  logic        stub_cam_q_pending;
  int unsigned stub_sram_rd_cyc;
  logic        stub_sram_rd_pending;
  int unsigned stub_ce_req_cyc;
  logic        stub_ce_req_pending;

  always @(posedge clk) begin
    cycle_count++;

    if (up_valid && up_ready) begin
      up_valid_seen  <= 1'b1;
      up_lit_latched <= up_lit;
      if (up_count < 8)
        up_lit_log[up_count] = up_lit;
      up_count = up_count + 1;
    end
    if (conflict_valid) begin
      conflict_seen           <= 1'b1;
      conflict_clause_latched <= conflict_clause;
    end

    if (trail_pop_req && !stub_trail_req_pending) begin
      stub_trail_req_cyc     = cycle_count;
      stub_trail_req_pending = 1'b1;
    end
    if (trail_pop_valid && stub_trail_req_pending) begin
      $display("  [STUB LAT trail ] req@cyc=%0d  valid@cyc=%0d  latency=%0d cycle(s)",
               stub_trail_req_cyc, cycle_count, cycle_count - stub_trail_req_cyc);
      stub_trail_req_pending = 1'b0;
    end

    if (cam_q_valid && cam_q_ready && !stub_cam_q_pending) begin
      stub_cam_q_cyc     = cycle_count;
      stub_cam_q_pending = 1'b1;
    end
    if (cam_r_valid && cam_r_ready && cam_r_last && stub_cam_q_pending) begin
      $display("  [STUB LAT cam   ] q_accept@cyc=%0d  r_last@cyc=%0d  latency=%0d cycle(s)",
               stub_cam_q_cyc, cycle_count, cycle_count - stub_cam_q_cyc);
      stub_cam_q_pending = 1'b0;
    end

    if (sram_rd_valid && sram_rd_ready && !stub_sram_rd_pending) begin
      stub_sram_rd_cyc     = cycle_count;
      stub_sram_rd_pending = 1'b1;
    end
    if (sram_rd_resp_valid && sram_rd_resp_ready && stub_sram_rd_pending) begin
      $display("  [STUB LAT sram  ] rd_accept@cyc=%0d  resp@cyc=%0d  latency=%0d cycle(s)",
               stub_sram_rd_cyc, cycle_count, cycle_count - stub_sram_rd_cyc);
      stub_sram_rd_pending = 1'b0;
    end

    if (ce_valid && ce_ready && !stub_ce_req_pending) begin
      stub_ce_req_cyc     = cycle_count;
      stub_ce_req_pending = 1'b1;
    end
    if (ce_resp_valid && ce_resp_ready && stub_ce_req_pending) begin
      $display("  [STUB LAT ce    ] req_accept@cyc=%0d  resp@cyc=%0d  latency=%0d cycle(s)",
               stub_ce_req_cyc, cycle_count, cycle_count - stub_ce_req_cyc);
      stub_ce_req_pending = 1'b0;
    end
  end

  always @(posedge clk) begin
    if (conflict_valid)
      $display("  [cyc=%0d t=%0t] CONFLICT clause=%0d", cycle_count, $time, conflict_clause);
    if (up_valid && up_ready)
      $display("  [cyc=%0d t=%0t] UNIT lit=0x%0h reason=%0d idx=%0d",
               cycle_count, $time, up_lit, up_reason_clause, up_lit_idx);
    if (done)
      $display("  [cyc=%0d t=%0t] DONE", cycle_count, $time);
  end

  // -------------------------------------------------------------------------
  // Decode DUT FSM state for messages (must match hw_bcp_defs.vh).
  // -------------------------------------------------------------------------
  function automatic string fsm_state_str(input logic [3:0] s);
    case (s)
      4'd0:  return "S_IDLE";
      4'd1:  return "S_POP_REQ";
      4'd2:  return "S_POP_WAIT";
      4'd3:  return "S_CAM_Q";
      4'd4:  return "S_CAM_STREAM";
      4'd5:  return "S_CE_REQ";
      4'd6:  return "S_CE_WAIT";
      4'd7:  return "S_HANDLE_UNIT";
      4'd8:  return "S_LITVAL_REQ";
      4'd9:  return "S_LITVAL_WAIT";
      4'd10: return "S_EMIT_UP";
      4'd11: return "S_REPORT_CONFL";
      4'd12: return "S_DONE";
      default: return $sformatf("unknown(%0d)", s);
    endcase
  endfunction

  task automatic print_dut_snapshot(input string where);
    $display("  snapshot @%s: busy=%b done=%b start=%b | trail_pop_req=%b trail_pop_valid=%b | cam_q=%b cam_r=%b | ce_valid=%b ce_resp=%b | conflict=%b up=%b | DUT state=%s",
             where, busy, done, start, trail_pop_req, trail_pop_valid,
             cam_q_valid, cam_r_valid, ce_valid, ce_resp_valid, conflict_valid, up_valid,
             fsm_state_str(dut.state));
  endtask

  // -------------------------------------------------------------------------
  // DUT
  // -------------------------------------------------------------------------
  hw_bcp_propagate #(
    .VAR_W(VAR_W),
    .LIT_W(LIT_W),
    .CLAUSE_W(CLAUSE_W),
    .LITIDX_W(LITIDX_W),
    .REASON_W(REASON_W),
    .TRAIL_DEPTH_W(TRAIL_PTR_W)
  ) dut (
    .clk, .rst_n,
    .start, .busy, .done,
    .trail_size, .qhead_init,
    .trail_pop_req, .trail_pop_valid, .trail_pop_lit,
    .cam_q_valid, .cam_q_ready, .cam_q_var,
    .cam_r_valid, .cam_r_ready, .cam_r_clause, .cam_r_last,
    .sram_rd_valid, .sram_rd_ready, .sram_rd_lit,
    .sram_rd_resp_valid, .sram_rd_resp_ready, .sram_rd_val,
    .sram_wr_valid, .sram_wr_ready, .sram_wr_lit, .sram_wr_val,
    .ce_valid, .ce_ready, .ce_clause,
    .ce_resp_valid, .ce_resp_ready, .ce_status, .ce_u_lit, .ce_u_idx,
    .conflict_valid, .conflict_clause,
    .up_valid, .up_ready, .up_lit, .up_reason_clause, .up_lit_idx
  );

  assign up_ready = 1'b1;

  // -------------------------------------------------------------------------
  // Stub instances
  // -------------------------------------------------------------------------

  stub_trail #(
    .LIT_W(LIT_W), .DEPTH(32), .PTR_W(TRAIL_PTR_W)
  ) u_trail (
    .clk, .rst_n,
    .qhead(qhead_init),
    .trail_size(trail_size),
    .pop_valid(trail_pop_valid),
    .pop_req(trail_pop_req),
    .pop_lit(trail_pop_lit)
  );

  // LUT-based CAM: programmed per-test via cam_set() / cam_reset_defaults().
  tb_cam_lut #(
    .VAR_W(VAR_W), .CLAUSE_W(CLAUSE_W)
  ) u_cam (
    .clk, .rst_n,
    .q_valid(cam_q_valid),
    .q_ready(cam_q_ready),
    .q_var(cam_q_var),
    .r_valid(cam_r_valid),
    .r_ready(cam_r_ready),
    .r_clause(cam_r_clause),
    .r_last(cam_r_last)
  );

  stub_sram_litval #(
    .LIT_W(LIT_W)
  ) u_sram (
    .clk, .rst_n,
    .rd_valid(sram_rd_valid),
    .rd_ready(sram_rd_ready),
    .rd_lit(sram_rd_lit),
    .rd_resp_valid(sram_rd_resp_valid),
    .rd_resp_ready(sram_rd_resp_ready),
    .rd_val(sram_rd_val),
    .wr_valid(sram_wr_valid),
    .wr_ready(sram_wr_ready),
    .wr_lit(sram_wr_lit),
    .wr_val(sram_wr_val)
  );

  // LUT-based CE: programmed per-test via ce_set() / ce_reset_defaults().
  tb_ce_lut #(
    .CLAUSE_W(CLAUSE_W), .LIT_W(LIT_W), .LITIDX_W(LITIDX_W)
  ) u_ce (
    .clk, .rst_n,
    .req_valid(ce_valid),
    .req_ready(ce_ready),
    .req_clause(ce_clause),
    .resp_valid(ce_resp_valid),
    .resp_ready(ce_resp_ready),
    .resp_status(ce_status),
    .resp_u_lit(ce_u_lit),
    .resp_u_idx(ce_u_idx)
  );

  // -------------------------------------------------------------------------
  // Literal helpers.
  // Literal encoding: lit[LIT_W-1] = polarity (1=neg), lit[LIT_W-2:0] = var id.
  //   pos_lit(v) = v          (bit 6 clear → positive / variable is true)
  //   neg_lit(v) = (1<<6)|v   (bit 6 set   → negative / variable is false)
  // -------------------------------------------------------------------------
  function automatic logic [LIT_W-1:0] pos_lit(input int unsigned v);
    return LIT_W'(v);
  endfunction

  function automatic logic [LIT_W-1:0] neg_lit(input int unsigned v);
    return LIT_W'((1 << (LIT_W-1)) | v);
  endfunction

  // -------------------------------------------------------------------------
  // Stub-programming tasks.
  // -------------------------------------------------------------------------

  // Restore every CAM variable to the default (one entry → clause 0).
  task automatic cam_reset_defaults();
    for (int v = 0; v < 64; v++) begin
      u_cam.watch_count[v] = 3'd1;
      for (int i = 0; i < 4; i++)
        u_cam.watch_clauses[v][i] = '0;
    end
  endtask

  // Set the watchlist for one variable (up to 4 clauses).
  task automatic cam_set(
    input int unsigned var_id,
    input int unsigned n,
    input int unsigned c0 = 0,
    input int unsigned c1 = 0,
    input int unsigned c2 = 0,
    input int unsigned c3 = 0
  );
    u_cam.watch_count[var_id] = 3'(n);
    if (n > 0) u_cam.watch_clauses[var_id][0] = CLAUSE_W'(c0);
    if (n > 1) u_cam.watch_clauses[var_id][1] = CLAUSE_W'(c1);
    if (n > 2) u_cam.watch_clauses[var_id][2] = CLAUSE_W'(c2);
    if (n > 3) u_cam.watch_clauses[var_id][3] = CLAUSE_W'(c3);
  endtask

  // Set every clause to DONE.
  task automatic ce_reset_defaults();
    for (int c = 0; c < 64; c++) begin
      u_ce.clause_status[c] = `CLAUSE_DONE;
      u_ce.clause_u_lit[c]  = '0;
      u_ce.clause_u_idx[c]  = '0;
    end
  endtask

  // Program one clause's response.
  task automatic ce_set(
    input int unsigned clause_id,
    input logic [1:0]          status,
    input logic [LIT_W-1:0]    u_lit = '0,
    input logic [LITIDX_W-1:0] u_idx = '0
  );
    u_ce.clause_status[clause_id] = status;
    u_ce.clause_u_lit[clause_id]  = u_lit;
    u_ce.clause_u_idx[clause_id]  = u_idx;
  endtask

  // -------------------------------------------------------------------------
  // Drive trail literals before a run.
  // init_trail_mem(n)  — positive literals for vars 0,2,4,… (legacy helper)
  // init_trail_lit(i, lit) — set trail[i] to an arbitrary literal
  // -------------------------------------------------------------------------
  task automatic init_trail_mem(input int unsigned n);
    for (int i = 0; i < n; i++)
      u_trail.mem[i] = LIT_W'((i << 1) | 1'b0);
  endtask

  task automatic init_trail_lit(input int unsigned idx, input logic [LIT_W-1:0] lit);
    u_trail.mem[idx] = lit;
  endtask

  // -------------------------------------------------------------------------
  // Synchronous reset.
  // -------------------------------------------------------------------------
  task automatic apply_reset();
    rst_n      = 1'b0;
    start      = 1'b0;
    qhead_init = '0;
    trail_size = '0;
    repeat (5) @(posedge clk);
    rst_n = 1'b1;
    @(posedge clk);
  endtask

  // -------------------------------------------------------------------------
  // Clear per-test monitors (waits one posedge + 1 ps so NBA updates settle).
  // -------------------------------------------------------------------------
  task automatic clear_monitors();
    @(posedge clk); #1;
    up_valid_seen = 1'b0;
    up_lit_latched = '0;
    up_count = 0;
    for (int i = 0; i < 8; i++) up_lit_log[i] = '0;
    conflict_seen = 1'b0;
  endtask

  // -------------------------------------------------------------------------
  // Wait until done or cycle timeout.
  // -------------------------------------------------------------------------
  task automatic wait_done_timeout(
    input  int unsigned max_cycles,
    output bit          ok,
    output int unsigned cycles_used
  );
    int unsigned cyc;
    ok          = 1'b0;
    cycles_used = max_cycles;
    cyc = 0;
    while (cyc < max_cycles) begin
      @(posedge clk);
      cyc++;
`ifdef TB_DEBUG
      if (cyc <= 60)
        $display("  [wait_done cyc=%0d] done=%b busy=%b trail_pop=%b cam_q=%b ce_v=%b",
                 cyc, done, busy, trail_pop_req, cam_q_valid, ce_valid);
`endif
      if (done) begin
        ok          = 1'b1;
        cycles_used = cyc;
        break;
      end
    end
  endtask

  task automatic pulse_start();
    start = 1'b1;
    @(posedge clk);
    start = 1'b0;
  endtask

  // ==========================================================================
  // Test 1: idle outputs before start
  // ==========================================================================
  task automatic test_idle_before_start();
    $display("\n--- Test 1: idle outputs before start ---");
    $display("  expected: busy=0, done=0");
    if (busy !== 1'b0 || done !== 1'b0) begin
      $error("FAIL: got busy=%b done=%b (start=%b)", busy, done, start);
      print_dut_snapshot("fail");
      tests_failed++;
    end else begin
      $display("PASS: busy=%b done=%b", busy, done);
      tests_passed++;
    end
  endtask

  // ==========================================================================
  // Test 2: zero-length trail completes without pop
  // ==========================================================================
  task automatic test_zero_trail_immediate_done();
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 2: zero trail completes without pop ---");
    trail_size = TRAIL_PTR_W'(0);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  stimulus: trail_size=%0d qhead_init=%0d", trail_size, qhead_init);
    $display("  expected: done=1 within %0d cycles, busy=0", TIMEOUT_CYCLES);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles; got done=%b after %0d cycles (busy=%b)",
             TIMEOUT_CYCLES, done, cyc_done, busy);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else if (busy !== 1'b0) begin
      $error("FAIL: expected busy=0 after done; got busy=%b done=%b", busy, done);
      tests_failed++;
    end else begin
      $display("PASS: done=1 after %0d cycle(s), busy=%b", cyc_done, busy);
      tests_passed++;
    end
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 3: n trail literals, CE always DONE — liveness + no up/conflict
  // ==========================================================================
  task automatic test_nonempty_trail_runs_to_done(input int unsigned n);
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 3 (n=%0d): trail runs to done, no unit/conflict expected ---", n);
    clear_monitors();
    init_trail_mem(n);
    trail_size = TRAIL_PTR_W'(n);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  stimulus: trail_size=%0d, all-DONE CE (default)", n);
    $display("  expected: done=1 within %0d cycles; up_valid=0; conflict_valid=0", TIMEOUT_CYCLES);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL (n=%0d): expected done=1 within %0d cycles; got done=%b after %0d cycles | busy=%b",
             n, TIMEOUT_CYCLES, done, cyc_done, busy);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): done=1 after %0d cycle(s)", n, cyc_done);
      tests_passed++;
    end
    if (up_valid_seen) begin
      $error("FAIL (n=%0d): up_valid asserted (lit=0x%0h) — no unit prop should occur with all-DONE CE",
             n, up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): up_valid never asserted", n);
      tests_passed++;
    end
    if (conflict_seen) begin
      $error("FAIL (n=%0d): conflict_valid asserted (clause=%0d) — no conflict with all-DONE CE",
             n, conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): conflict_valid never asserted", n);
      tests_passed++;
    end
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 4: done holds while start=1; deasserts after start=0
  // ==========================================================================
  task automatic test_done_requires_start_low_to_re_idle();
    bit          ok;
    int unsigned cyc_done;
    logic        done_sample;
    $display("\n--- Test 4: done holds while start=1; deasserts after start=0 ---");
    $display("  stimulus: trail_size=0, start held 1 until done, then start=0");
    $display("  expected: (a) done=1 in S_DONE with start=1; (b) done=0 one posedge after start=0");
    trail_size = TRAIL_PTR_W'(0);
    qhead_init = TRAIL_PTR_W'(0);
    start = 1'b1;
    @(posedge clk);
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles (start held high); got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
      return;
    end
    if (done !== 1'b1) begin
      $error("FAIL: expected done=1 in S_DONE; got done=%b busy=%b state=%s",
             done, busy, fsm_state_str(dut.state));
      tests_failed++;
      return;
    end
    $display("  checkpoint: done=1 after %0d cycle(s) with start=1 (expected)", cyc_done);
    start = 1'b0;
    @(posedge clk);
    #1;
    done_sample = done;
    if (done_sample !== 1'b0) begin
      $error("FAIL: expected done=0 one posedge after start=0; got done=%b busy=%b state=%s",
             done_sample, busy, fsm_state_str(dut.state));
      print_dut_snapshot("fail test 4");
      tests_failed++;
    end else begin
      $display("PASS: done=0, busy=0 after start deasserts (FSM returned to IDLE)");
      tests_passed++;
    end
  endtask

  // ==========================================================================
  // Test 5: unit propagation — CE returns UNIT → up_valid asserted
  // ==========================================================================
  task automatic test_unit_propagation();
    logic [LIT_W-1:0]    exp_lit;
    logic [LITIDX_W-1:0] exp_idx;
    bit          ok;
    int unsigned cyc_done;
    exp_lit = LIT_W'('h0F);   // var=15, polarity=0
    exp_idx = LITIDX_W'(2);
    $display("\n--- Test 5: unit propagation emits up_valid ---");
    clear_monitors();
    ce_set(0, `CLAUSE_UNIT, exp_lit, exp_idx);
    init_trail_mem(1);
    trail_size = TRAIL_PTR_W'(1);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  CNF:      clause 0 is UNIT for lit=0x%0h (var=%0d pos) at idx=%0d",
             exp_lit, exp_lit[LIT_W-2:0], exp_idx);
    $display("  stimulus: trail_size=1 (trail[0]=var0-pos), CE clause-0=UNIT");
    $display("  expected: up_valid with up_lit=0x%0h; no conflict; done", exp_lit);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end
    if (!up_valid_seen) begin
      $error("FAIL: up_valid was never asserted");
      tests_failed++;
    end else if (up_lit_latched !== exp_lit) begin
      $error("FAIL: up_lit=0x%0h, expected 0x%0h", up_lit_latched, exp_lit);
      tests_failed++;
    end else begin
      $display("PASS: up_valid asserted with up_lit=0x%0h (correct)", up_lit_latched);
      tests_passed++;
    end
    if (conflict_seen) begin
      $error("FAIL: conflict_valid raised unexpectedly (clause=%0d)", conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS: no conflict raised");
      tests_passed++;
    end
    ce_set(0, `CLAUSE_DONE);
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 6: conflict detection — CE returns CONFLICT → conflict_valid asserted
  // ==========================================================================
  task automatic test_conflict_detection();
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 6: conflict detection emits conflict_valid ---");
    clear_monitors();
    ce_set(0, `CLAUSE_CONFLICT);
    init_trail_mem(1);
    trail_size = TRAIL_PTR_W'(1);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  CNF:      clause 0 is fully falsified → CONFLICT");
    $display("  stimulus: trail_size=1, CE clause-0=CONFLICT");
    $display("  expected: conflict_valid asserted; no up_valid; done");
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end
    if (!conflict_seen) begin
      $error("FAIL: conflict_valid was never asserted");
      tests_failed++;
    end else begin
      $display("PASS: conflict_valid asserted (clause=%0d)", conflict_clause_latched);
      tests_passed++;
    end
    if (up_valid_seen) begin
      $error("FAIL: up_valid raised during conflict test (up_lit=0x%0h)", up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS: up_valid never asserted (correct — conflict path skips implication)");
      tests_passed++;
    end
    ce_set(0, `CLAUSE_DONE);
  endtask

  // ==========================================================================
  // Test 7: 3-SAT BCP pass — single unit implication
  //
  // CNF (6 variables, 6 clauses, 3 literals each — random-3-SAT SAT region):
  //   p cnf 6 6
  //   C0:  x1  x2  x3  (1  2  3 0)
  //   C1: -x1  x2  x4  (-1  2  4 0)
  //   C2:  x2 -x3  x5  (2 -3  5 0)   ← -x3 satisfied when x3=F → DONE
  //   C3: -x2  x3  x6  (-2  3  6 0)  ← x3=F falsifies x3; if x2=T (SRAM) then
  //                                      -x2=F too; only x6 free → UNIT for x6
  //   C4:  x1 -x4  x5  (1 -4  5 0)
  //   C5: -x1  x4 -x6  (-1  4 -6 0)
  //
  // BCP scenario:
  //   Prior SRAM state: x2=T (assigned earlier, not in trail)
  //   Trail: {x3=F}  (x3 just set false; the CE sees x2=T,x3=F,x6=VALU in C3)
  //   CAM watchlist for var 3: [C2, C3]
  //     CE(C2) → DONE  (C2 satisfied via -x3=T)
  //     CE(C3) → UNIT  (unit literal = x6=T, idx=2)
  //   SRAM stub returns VALU for x6 → FSM emits UP
  //
  // Expected output:
  //   up_valid=1, up_lit=pos_lit(6)=0x06, up_reason_clause=3, up_lit_idx=2
  //   conflict_valid=0
  //   done=1
  // ==========================================================================
  task automatic test_3sat_unit_prop();
    logic [LIT_W-1:0]    exp_lit;
    logic [LITIDX_W-1:0] exp_idx;
    bit          ok;
    int unsigned cyc_done;

    exp_lit = pos_lit(6);   // x6=T
    exp_idx = LITIDX_W'(2); // x6 is at position 2 in C3

    $display("\n--- Test 7: 3-SAT unit propagation (random-3-SAT SAT region) ---");
    $display("  CNF: p cnf 6 6");
    $display("    C0:  x1  x2  x3   C1: -x1  x2  x4");
    $display("    C2:  x2 -x3  x5   C3: -x2  x3  x6  <- UNIT for x6 when x2=T,x3=F");
    $display("    C4:  x1 -x4  x5   C5: -x1  x4 -x6");
    $display("  BCP scenario: prior x2=T; trail = { x3=F (neg_lit(3)=0x%0h) }",
             neg_lit(3));
    $display("    CAM[var3] -> [C2, C3]");
    $display("    CE(C2)=DONE (clause satisfied by -x3=T)");
    $display("    CE(C3)=UNIT unit_lit=pos_lit(6)=0x%0h idx=2", exp_lit);
    $display("  expected output: up_valid=1 up_lit=0x%0h reason_clause=3 idx=2; done; no conflict",
             exp_lit);

    clear_monitors();
    cam_reset_defaults();
    ce_reset_defaults();

    // CAM: var 3 → C2, then C3
    cam_set(3, 2, /*c0=*/2, /*c1=*/3);

    // CE: C2=DONE (default), C3=UNIT for x6 at idx 2
    ce_set(3, `CLAUSE_UNIT, exp_lit, exp_idx);

    // Trail: one entry — neg_lit(3) (x3=F)
    init_trail_lit(0, neg_lit(3));
    trail_size = TRAIL_PTR_W'(1);
    qhead_init = TRAIL_PTR_W'(0);

    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);

    // Check liveness
    if (!ok) begin
      $error("FAIL T7: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS T7: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end

    // Check unit implication
    if (!up_valid_seen) begin
      $error("FAIL T7: up_valid never asserted (expected unit for x6=T)");
      tests_failed++;
    end else if (up_lit_latched !== exp_lit) begin
      $error("FAIL T7: up_lit=0x%0h, expected 0x%0h (pos_lit(6))", up_lit_latched, exp_lit);
      tests_failed++;
    end else begin
      $display("PASS T7: up_valid=1 up_lit=0x%0h (x6=T, correct)", up_lit_latched);
      tests_passed++;
    end

    // Check no conflict
    if (conflict_seen) begin
      $error("FAIL T7: conflict_valid raised (clause=%0d) — not expected", conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS T7: no conflict (correct)");
      tests_passed++;
    end

    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 8: 3-SAT forced CONFLICT
  //
  // CNF (6 variables, 7 clauses — random-3-SAT UNSAT-region instance):
  //   p cnf 6 7
  //   C0:  x1  x2  x4   (-1  2  4 0)
  //   C1: -x1  x2  x5   (-1  2  5 0)
  //   C2:  x1 -x2  x4   ( 1 -2  4 0)
  //   C3: -x1 -x2  x5   (-1 -2  5 0)
  //   C4: -x1 -x3 -x4   (-1 -3 -4 0)  ← all-false when x1=T,x3=T,x4=T
  //   C5:  x2  x3  x5   ( 2  3  5 0)
  //   C6: -x2  x3  x6   (-2  3  6 0)
  //
  // BCP scenario:
  //   Prior SRAM state: x1=T, x4=T
  //   Trail: {x3=T}  (x3 just set true)
  //   CAM watchlist for var 3: [C4]
  //     CE(C4) → CONFLICT  (x1=T→-x1=F, x3=T→-x3=F, x4=T→-x4=F; all three false)
  //
  // Expected output:
  //   conflict_valid=1, conflict_clause=4
  //   up_valid=0
  //   done=1
  // ==========================================================================
  task automatic test_3sat_conflict();
    bit          ok;
    int unsigned cyc_done;

    $display("\n--- Test 8: 3-SAT forced CONFLICT (random-3-SAT UNSAT region) ---");
    $display("  CNF: p cnf 6 7");
    $display("    C0:  x1  x2  x4   C1: -x1  x2  x5");
    $display("    C2:  x1 -x2  x4   C3: -x1 -x2  x5");
    $display("    C4: -x1 -x3 -x4   <- CONFLICT when x1=T, x3=T, x4=T");
    $display("    C5:  x2  x3  x5   C6: -x2  x3  x6");
    $display("  BCP scenario: prior x1=T,x4=T in SRAM; trail = { x3=T (pos_lit(3)=0x%0h) }",
             pos_lit(3));
    $display("    CAM[var3] -> [C4]");
    $display("    CE(C4)=CONFLICT  (all literals falsified)");
    $display("  expected output: conflict_valid=1 conflict_clause=4; up_valid=0; done");

    clear_monitors();
    cam_reset_defaults();
    ce_reset_defaults();

    // CAM: var 3 → C4 only (single-entry watchlist)
    cam_set(3, 1, /*c0=*/4);

    // CE: C4 = CONFLICT
    ce_set(4, `CLAUSE_CONFLICT);

    // Trail: one entry — pos_lit(3) (x3=T)
    init_trail_lit(0, pos_lit(3));
    trail_size = TRAIL_PTR_W'(1);
    qhead_init = TRAIL_PTR_W'(0);

    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);

    if (!ok) begin
      $error("FAIL T8: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS T8: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end

    if (!conflict_seen) begin
      $error("FAIL T8: conflict_valid never asserted (expected CONFLICT on clause 4)");
      tests_failed++;
    end else if (conflict_clause_latched !== CLAUSE_W'(4)) begin
      $error("FAIL T8: conflict_clause=%0d, expected 4", conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS T8: conflict_valid=1 conflict_clause=%0d (correct)", conflict_clause_latched);
      tests_passed++;
    end

    if (up_valid_seen) begin
      $error("FAIL T8: up_valid asserted (lit=0x%0h) — conflict path must not emit implication",
             up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS T8: up_valid never asserted (correct for conflict path)");
      tests_passed++;
    end

    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 9: 4-SAT all-DONE BCP pass (multiple trail literals)
  //
  // CNF (8 variables, 6 clauses, 4 literals each — random-4-SAT SAT region,
  //      ratio=0.75, well below 4-SAT phase transition ~9.93):
  //   p cnf 8 6
  //   C0:  x1  x2  x3  x4   ( 1  2  3  4 0)  <- x1=T satisfies C0
  //   C1: -x1  x2  x5  x6   (-1  2  5  6 0)  <- -x1 falsified; 3 free → DONE
  //   C2:  x1 -x2  x3  x7   ( 1 -2  3  7 0)  <- x1=T satisfies C2
  //   C3: -x1 -x2  x4  x8   (-1 -x2  4  8 0) <- -x1,-x2 falsified; 2 free → DONE
  //   C4:  x2  x3 -x5  x7   ( 2  3 -5  7 0)  <- x2=T satisfies C4
  //   C5: -x2  x3  x5 -x8   (-2  3  5 -8 0)  <- -x2 falsified; 3 free → DONE
  //
  // BCP scenario:
  //   Trail: {x1=T, x2=T, x3=T}
  //     var 1 → CAM returns [C0, C1]  (both evaluate to DONE)
  //     var 2 → CAM returns [C2, C3]  (C2 satisfied; C3 has ≥2 unassigned → DONE)
  //     var 3 → CAM returns [C4, C5]  (C4 satisfied; C5 has ≥2 unassigned → DONE)
  //
  // Expected output:
  //   up_valid=0, conflict_valid=0, done=1
  // ==========================================================================
  task automatic test_4sat_all_done();
    bit          ok;
    int unsigned cyc_done;

    $display("\n--- Test 9: 4-SAT all-DONE BCP pass (random-4-SAT SAT region) ---");
    $display("  CNF: p cnf 8 6  (4 literals/clause, ratio=0.75)");
    $display("    C0:  x1  x2  x3  x4    C1: -x1  x2  x5  x6");
    $display("    C2:  x1 -x2  x3  x7    C3: -x1 -x2  x4  x8");
    $display("    C4:  x2  x3 -x5  x7    C5: -x2  x3  x5 -x8");
    $display("  BCP scenario: trail = { x1=T, x2=T, x3=T }");
    $display("    CAM[var1]->[C0,C1]  CAM[var2]->[C2,C3]  CAM[var3]->[C4,C5]");
    $display("    All CE responses = DONE (clauses satisfied or >=2 unassigned lits)");
    $display("  expected output: up_valid=0; conflict_valid=0; done=1");

    clear_monitors();
    cam_reset_defaults();
    ce_reset_defaults();

    // CAM: var 1 → [C0, C1], var 2 → [C2, C3], var 3 → [C4, C5]
    cam_set(1, 2, /*c0=*/0, /*c1=*/1);
    cam_set(2, 2, /*c0=*/2, /*c1=*/3);
    cam_set(3, 2, /*c0=*/4, /*c1=*/5);
    // CE: all clauses 0-5 = DONE (already default after ce_reset_defaults)

    // Trail: {x1=T, x2=T, x3=T}
    init_trail_lit(0, pos_lit(1));
    init_trail_lit(1, pos_lit(2));
    init_trail_lit(2, pos_lit(3));
    trail_size = TRAIL_PTR_W'(3);
    qhead_init = TRAIL_PTR_W'(0);

    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);

    if (!ok) begin
      $error("FAIL T9: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS T9: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end

    if (up_valid_seen) begin
      $error("FAIL T9: up_valid asserted (%0d time(s), last lit=0x%0h) — all-DONE CE should emit no UPs",
             up_count, up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS T9: up_valid never asserted (correct for all-DONE CE)");
      tests_passed++;
    end

    if (conflict_seen) begin
      $error("FAIL T9: conflict_valid asserted (clause=%0d) — not expected with all-DONE CE",
             conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS T9: conflict_valid never asserted (correct)");
      tests_passed++;
    end

    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Test 10: 3-SAT sequential unit propagations
  //
  // CNF (6 variables, 6 clauses — random-3-SAT SAT region, ratio=1.0):
  //   p cnf 6 6
  //   C0: -x1 -x3  x5   (-1 -3  5 0) <- UNIT for x5=T when x1=T,x3=T
  //   C1: -x2 -x4  x6   (-2 -4  6 0) <- UNIT for x6=T when x2=T,x4=T
  //   C2:  x1  x3 -x5   ( 1  3 -5 0)
  //   C3:  x2  x4 -x6   ( 2  4 -6 0)
  //   C4:  x5  x6  x1   ( 5  6  1 0)
  //   C5: -x5 -x6  x2   (-5 -6  2 0)
  //
  // BCP scenario:
  //   Prior SRAM state: x3=T, x4=T (assigned earlier, not in trail)
  //   Trail: {x1=T, x2=T}
  //     var 1 → CAM returns [C0]
  //       CE(C0)=UNIT for x5=T at idx=2  (x1=T→-x1=F, x3=T→-x3=F, only x5 free)
  //       SRAM returns VALU for x5 → FSM emits UP(x5=T, reason=C0)
  //     var 2 → CAM returns [C1]
  //       CE(C1)=UNIT for x6=T at idx=2  (x2=T→-x2=F, x4=T→-x4=F, only x6 free)
  //       SRAM returns VALU for x6 → FSM emits UP(x6=T, reason=C1)
  //
  // Expected output:
  //   up_count=2
  //   up_lit_log[0] = pos_lit(5) = 0x05   (x5=T, reason clause 0)
  //   up_lit_log[1] = pos_lit(6) = 0x06   (x6=T, reason clause 1)
  //   conflict_valid=0
  //   done=1
  // ==========================================================================
  task automatic test_3sat_sequential_units();
    logic [LIT_W-1:0] exp_lit0, exp_lit1;
    bit          ok;
    int unsigned cyc_done;

    exp_lit0 = pos_lit(5);  // x5=T
    exp_lit1 = pos_lit(6);  // x6=T

    $display("\n--- Test 10: 3-SAT sequential unit propagations ---");
    $display("  CNF: p cnf 6 6  (random-3-SAT SAT region, ratio=1.0)");
    $display("    C0: -x1 -x3  x5   <- UNIT x5=T when x1=T,x3=T");
    $display("    C1: -x2 -x4  x6   <- UNIT x6=T when x2=T,x4=T");
    $display("    C2:  x1  x3 -x5   C3:  x2  x4 -x6");
    $display("    C4:  x5  x6  x1   C5: -x5 -x6  x2");
    $display("  BCP scenario: prior x3=T,x4=T; trail = { x1=T (0x%0h), x2=T (0x%0h) }",
             pos_lit(1), pos_lit(2));
    $display("    CAM[var1]->[C0] CE(C0)=UNIT lit=0x%0h idx=2", exp_lit0);
    $display("    CAM[var2]->[C1] CE(C1)=UNIT lit=0x%0h idx=2", exp_lit1);
    $display("  expected output: up_count=2; up[0]=0x%0h (x5=T); up[1]=0x%0h (x6=T); done",
             exp_lit0, exp_lit1);

    clear_monitors();
    cam_reset_defaults();
    ce_reset_defaults();

    // CAM: var 1 → [C0], var 2 → [C1]
    cam_set(1, 1, /*c0=*/0);
    cam_set(2, 1, /*c0=*/1);

    // CE: C0 = UNIT(x5=T, idx=2), C1 = UNIT(x6=T, idx=2)
    ce_set(0, `CLAUSE_UNIT, exp_lit0, LITIDX_W'(2));
    ce_set(1, `CLAUSE_UNIT, exp_lit1, LITIDX_W'(2));

    // Trail: {x1=T, x2=T}
    init_trail_lit(0, pos_lit(1));
    init_trail_lit(1, pos_lit(2));
    trail_size = TRAIL_PTR_W'(2);
    qhead_init = TRAIL_PTR_W'(0);

    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);

    if (!ok) begin
      $error("FAIL T10: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS T10: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end

    // Check that exactly 2 unit implications were emitted
    if (up_count !== 2) begin
      $error("FAIL T10: expected up_count=2; got %0d", up_count);
      tests_failed++;
    end else begin
      $display("PASS T10: up_count=2 (correct)");
      tests_passed++;
    end

    // Check first implication: x5=T
    if (up_count >= 1 && up_lit_log[0] !== exp_lit0) begin
      $error("FAIL T10: up[0]=0x%0h, expected 0x%0h (x5=T)", up_lit_log[0], exp_lit0);
      tests_failed++;
    end else if (up_count >= 1) begin
      $display("PASS T10: up[0]=0x%0h (x5=T, correct)", up_lit_log[0]);
      tests_passed++;
    end

    // Check second implication: x6=T
    if (up_count >= 2 && up_lit_log[1] !== exp_lit1) begin
      $error("FAIL T10: up[1]=0x%0h, expected 0x%0h (x6=T)", up_lit_log[1], exp_lit1);
      tests_failed++;
    end else if (up_count >= 2) begin
      $display("PASS T10: up[1]=0x%0h (x6=T, correct)", up_lit_log[1]);
      tests_passed++;
    end

    if (conflict_seen) begin
      $error("FAIL T10: conflict_valid asserted unexpectedly (clause=%0d)", conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS T10: no conflict (correct)");
      tests_passed++;
    end

    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // ==========================================================================
  // Simulation entry point
  // ==========================================================================
  initial begin
    $vcdpluson;
    tests_passed             = 0;
    tests_failed             = 0;
    cycle_count              = 0;
    up_valid_seen            = 1'b0;
    up_lit_latched           = '0;
    up_count                 = 0;
    for (int i = 0; i < 8; i++) up_lit_log[i] = '0;
    conflict_seen            = 1'b0;
    conflict_clause_latched  = '0;
    clk                      = 1'b0;
    rst_n                    = 1'b0;
    start                    = 1'b0;
    stub_trail_req_cyc       = 0;
    stub_trail_req_pending   = 1'b0;
    stub_cam_q_cyc           = 0;
    stub_cam_q_pending       = 1'b0;
    stub_sram_rd_cyc         = 0;
    stub_sram_rd_pending     = 1'b0;
    stub_ce_req_cyc          = 0;
    stub_ce_req_pending      = 1'b0;

    apply_reset();

    // ------------------------------------------------------------------
    // Tests 1-6: original behavioral coverage (stubs at default settings)
    // ------------------------------------------------------------------
    test_idle_before_start();

    test_zero_trail_immediate_done();
    apply_reset();

    test_nonempty_trail_runs_to_done(1);
    apply_reset();

    test_nonempty_trail_runs_to_done(4);
    apply_reset();

    test_done_requires_start_low_to_re_idle();
    apply_reset();

    test_unit_propagation();
    apply_reset();

    test_conflict_detection();
    apply_reset();

    // ------------------------------------------------------------------
    // Tests 7-10: CNF-grounded BCP scenarios (benchmark-database.de style)
    // ------------------------------------------------------------------
    test_3sat_unit_prop();
    apply_reset();

    test_3sat_conflict();
    apply_reset();

    test_4sat_all_done();
    apply_reset();

    test_3sat_sequential_units();

    // ------------------------------------------------------------------
    // Summary
    // ------------------------------------------------------------------
    $display("\n========================================");
    $display("Summary: %0d passed, %0d failed", tests_passed, tests_failed);
    $display("  T1  idle outputs before start");
    $display("  T2  zero-trail immediate done");
    $display("  T3a nonempty trail n=1 all-DONE");
    $display("  T3b nonempty trail n=4 all-DONE");
    $display("  T4  done/start handshake");
    $display("  T5  unit propagation (single)");
    $display("  T6  conflict detection");
    $display("  T7  3-SAT unit prop  CNF: p cnf 6 6 | trail={neg x3} | UNIT x6=T via C3");
    $display("  T8  3-SAT conflict   CNF: p cnf 6 7 | trail={pos x3} | CONFLICT via C4");
    $display("  T9  4-SAT all-DONE   CNF: p cnf 8 6 | trail={x1,x2,x3} | all DONE");
    $display("  T10 3-SAT 2x UNIT    CNF: p cnf 6 6 | trail={x1,x2} | UP x5 then x6");
    $display("========================================\n");
    if (tests_failed != 0)
      $fatal(1, "One or more tests failed.");
    $finish;
  end

endmodule
