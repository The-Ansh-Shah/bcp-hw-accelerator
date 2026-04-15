`timescale 1ns/1ps
`include "hw_bcp_defs.vh"

// ============================================================================
// tb_hw_bcp — HW-BCP propagate FSM testbench
// ============================================================================
// The DUT (hw_bcp_propagate) and stub_trail use synchronous active-low reset:
// after releasing rst_n, wait at least one posedge clk before driving meaningful
// control (apply_reset does this).
//
// The stock stub_cam never asserts r_valid, so the FSM would stick in CAM_STREAM.
// This file uses tb_cam_empty_watchlist instead: after each CAM query handshake it
// pulses one clause read with r_last=1 (one-clause watch list per variable), which
// lets the FSM exit CAM_STREAM and evaluate exactly one clause per trail literal.
//
// Behavioral coverage:
//   Test 1 — idle outputs before start (busy=0, done=0)
//   Test 2 — zero-length trail completes without pop
//   Test 3 — n-literal trail, all clauses evaluate to DONE (no unit props emitted)
//   Test 4 — done holds while start is high; deasserts once start drops
//   Test 5 — unit propagation: CE returns UNIT → up_valid asserted with correct lit
//   Test 6 — conflict detection: CE returns CONFLICT → conflict_valid asserted
//
// Cycle count is reported for every test completion.  Module-level monitors
// latch up_valid and conflict_valid between clear_monitors() calls so each test
// can assert the correct absence or presence of those outputs.
// ============================================================================

// ----------------------------------------------------------------------------
// Emits a single (r_valid, r_last) beat after each CAM query accept so the
// DUT can leave CAM_STREAM and return to the trail pop loop.
// ----------------------------------------------------------------------------
module tb_cam_empty_watchlist #(
  parameter int unsigned VAR_W    = 6,
  parameter int unsigned CLAUSE_W = 6
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
  typedef enum logic { ST_IDLE, ST_EMIT } state_t;
  state_t state;

  assign q_ready = 1'b1;

  always_ff @(posedge clk) begin
    if (!rst_n)
      state <= ST_IDLE;
    else begin
      case (state)
        ST_IDLE: if (q_valid && q_ready) state <= ST_EMIT;
        ST_EMIT: if (r_valid && r_ready) state <= ST_IDLE;
        default: state <= ST_IDLE;
      endcase
    end
  end

  assign r_valid  = (state == ST_EMIT);
  assign r_clause = '0;
  assign r_last   = 1'b1;
endmodule

// ----------------------------------------------------------------------------
// tb_ce_prog — programmable clause-evaluator stub.
//
// Returns prog_status (and associated unit info) for every accepted CE request.
// Response is registered so it remains stable after req_valid drops in the
// DUT's S_CE_WAIT state (split-transaction CE interface, same as stub_clause_eval).
//
// Usage in tests:
//   prog_status = `CLAUSE_DONE     → no action (default)
//   prog_status = `CLAUSE_UNIT     → FSM emits unit implication (up_valid)
//   prog_status = `CLAUSE_CONFLICT → FSM reports conflict (conflict_valid)
// ----------------------------------------------------------------------------
module tb_ce_prog #(
  parameter int unsigned CLAUSE_W  = 6,
  parameter int unsigned LIT_W     = 7,
  parameter int unsigned LITIDX_W  = 6
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
  output logic [LITIDX_W-1:0]    resp_u_idx,
  // Programming interface: configure what the next response will return
  input  logic [1:0]             prog_status,
  input  logic [LIT_W-1:0]       prog_u_lit,
  input  logic [LITIDX_W-1:0]    prog_u_idx
);
  // Always ready to accept a request (same behaviour as stub_clause_eval).
  assign req_ready = 1'b1;

  logic        resp_valid_r;
  logic [1:0]          resp_status_r;
  logic [LIT_W-1:0]    resp_u_lit_r;
  logic [LITIDX_W-1:0] resp_u_idx_r;

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      resp_valid_r  <= 1'b0;
      resp_status_r <= `CLAUSE_DONE;
      resp_u_lit_r  <= '0;
      resp_u_idx_r  <= '0;
    end else begin
      if (resp_valid_r && resp_ready) begin
        resp_valid_r <= 1'b0;
      end else if (req_valid && req_ready) begin
        resp_valid_r  <= 1'b1;
        resp_status_r <= prog_status;
        resp_u_lit_r  <= prog_u_lit;
        resp_u_idx_r  <= prog_u_idx;
      end
    end
  end

  assign resp_valid  = resp_valid_r;
  assign resp_status = resp_status_r;
  assign resp_u_lit  = resp_u_lit_r;
  assign resp_u_idx  = resp_u_idx_r;
endmodule

// ============================================================================
// Top-level testbench
// ============================================================================
module tb_hw_bcp;

  localparam int unsigned VAR_W         = 6;
  localparam int unsigned LIT_W         = VAR_W + 1;
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

  // CE programming inputs (drive tb_ce_prog)
  logic [1:0]          ce_prog_status;
  logic [LIT_W-1:0]    ce_prog_u_lit;
  logic [LITIDX_W-1:0] ce_prog_u_idx;

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
  // -------------------------------------------------------------------------
  int unsigned          cycle_count;
  logic                 up_valid_seen;
  logic [LIT_W-1:0]     up_lit_latched;
  logic                 conflict_seen;
  logic [CLAUSE_W-1:0]  conflict_clause_latched;

  always @(posedge clk) begin
    cycle_count++;
    if (up_valid && up_ready) begin
      up_valid_seen  <= 1'b1;
      up_lit_latched <= up_lit;
    end
    if (conflict_valid) begin
      conflict_seen           <= 1'b1;
      conflict_clause_latched <= conflict_clause;
    end
  end

  // Display notable events as they occur.
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

  // Trail memory model (synchronous reset) — see hw_bcp_sim_stubs.v
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

  // CAM: one clause per variable query, r_last=1 (single-entry watch list)
  tb_cam_empty_watchlist #(
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

  // SRAM: reads always return VALU (unassigned); writes always acked
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

  // CE: programmable — set ce_prog_status before each test
  tb_ce_prog #(
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
    .resp_u_idx(ce_u_idx),
    .prog_status(ce_prog_status),
    .prog_u_lit(ce_prog_u_lit),
    .prog_u_idx(ce_prog_u_idx)
  );

  // --------------------------------------------------------------------------
  // Drive trail literals before a run (low bits = variable id; MSB = polarity).
  // --------------------------------------------------------------------------
  task automatic init_trail_mem(input int unsigned n);
    for (int i = 0; i < n; i++)
      u_trail.mem[i] = LIT_W'((i << 1) | 1'b0);
  endtask

  // --------------------------------------------------------------------------
  // Synchronous reset: hold rst_n low, then release and wait one posedge so
  // DUT and stub_trail registers exit reset on the same clocking convention.
  // --------------------------------------------------------------------------
  task automatic apply_reset();
    rst_n      = 1'b0;
    start      = 1'b0;
    qhead_init = '0;
    trail_size = '0;
    repeat (5) @(posedge clk);
    rst_n = 1'b1;
    @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Clear per-test monitors.  Waits one posedge + 1 ps to ensure NBA updates
  // from the current edge have settled before overwriting with blocking assigns.
  // --------------------------------------------------------------------------
  task automatic clear_monitors();
    @(posedge clk); #1;
    up_valid_seen = 1'b0;
    conflict_seen = 1'b0;
  endtask

  // --------------------------------------------------------------------------
  // Wait until done or cycle timeout.
  //   ok         — 1 if done asserted within max_cycles, else 0
  //   cycles_used — cycles until done (1-based), or max_cycles on timeout
  // --------------------------------------------------------------------------
  task automatic wait_done_timeout(
    input  int unsigned max_cycles,
    output bit          ok,
    output int unsigned cycles_used
  );
    int unsigned cyc;
    ok         = 1'b0;
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

  // --------------------------------------------------------------------------
  // Test 1: After reset, before start, DUT should be idle (not busy, not done).
  // --------------------------------------------------------------------------
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

  // --------------------------------------------------------------------------
  // Test 2: Zero-length trail — POP_REQ sees qhead >= trail_size immediately
  // and the FSM should jump straight to DONE without popping.
  // --------------------------------------------------------------------------
  task automatic test_zero_trail_immediate_done();
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 2: zero trail completes without pop ---");
    trail_size = TRAIL_PTR_W'(0);
    qhead_init = TRAIL_PTR_W'(0);
    ce_prog_status = `CLAUSE_DONE;
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

  // --------------------------------------------------------------------------
  // Test 3: n trail literals, CE always returns CLAUSE_DONE.
  // Liveness check: run completes within timeout.
  // Behavioral check: up_valid must never be asserted (no unit clause → no
  //   implication); conflict_valid must never be asserted.
  // --------------------------------------------------------------------------
  task automatic test_nonempty_trail_runs_to_done(input int unsigned n);
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 3 (n=%0d): trail runs to done, no unit/conflict expected ---", n);
    clear_monitors();
    init_trail_mem(n);
    trail_size     = TRAIL_PTR_W'(n);
    qhead_init     = TRAIL_PTR_W'(0);
    ce_prog_status = `CLAUSE_DONE;
    $display("  stimulus: trail_size=%0d, ce=CLAUSE_DONE", n);
    $display("  expected: done=1 within %0d cycles; up_valid=0 throughout; conflict_valid=0 throughout", TIMEOUT_CYCLES);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    // Check 1: liveness — did the run complete?
    if (!ok) begin
      $error("FAIL (n=%0d): expected done=1 within %0d cycles; got done=%b after %0d cycles | busy=%b",
             n, TIMEOUT_CYCLES, done, cyc_done, busy);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): done=1 after %0d cycle(s)", n, cyc_done);
      tests_passed++;
    end
    // Check 2: no unit implication emitted (CE is CLAUSE_DONE → FSM never reaches EMIT_UP)
    if (up_valid_seen) begin
      $error("FAIL (n=%0d): up_valid was asserted (up_lit=0x%0h) but CE only returns DONE — no unit prop should occur",
             n, up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): up_valid never asserted (correct for all-DONE CE)", n);
      tests_passed++;
    end
    // Check 3: no conflict (CE is CLAUSE_DONE → FSM never reaches REPORT_CONFL)
    if (conflict_seen) begin
      $error("FAIL (n=%0d): conflict_valid was asserted (clause=%0d) but CE only returns DONE — no conflict should occur",
             n, conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS (n=%0d): conflict_valid never asserted (correct for all-DONE CE)", n);
      tests_passed++;
    end
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Test 4: With start held high, FSM reaches S_DONE (done=1) and holds it
  // until start deasserts.  After start=0 + one posedge the FSM returns to
  // IDLE and done=0.
  // --------------------------------------------------------------------------
  task automatic test_done_requires_start_low_to_re_idle();
    bit          ok;
    int unsigned cyc_done;
    logic        done_sample;
    $display("\n--- Test 4: done holds while start=1; deasserts after start=0 ---");
    $display("  stimulus: trail_size=0, start held 1 until done, then start=0");
    $display("  expected: (a) done=1 in S_DONE with start=1; (b) done=0 one posedge after start=0");
    trail_size     = TRAIL_PTR_W'(0);
    qhead_init     = TRAIL_PTR_W'(0);
    ce_prog_status = `CLAUSE_DONE;
    start          = 1'b1;
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
    // Deassert start; sample done one posedge later (+ 1 ps to clear NBA updates).
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

  // --------------------------------------------------------------------------
  // Test 5: Unit propagation.
  //   - 1 trail literal; CE returns CLAUSE_UNIT with a specific unit literal.
  //   - SRAM stub always returns VALU (unassigned), so FSM reaches EMIT_UP.
  //   - Expected: up_valid asserted with up_lit == expected literal; no conflict.
  // --------------------------------------------------------------------------
  task automatic test_unit_propagation();
    logic [LIT_W-1:0]    exp_lit;
    logic [LITIDX_W-1:0] exp_idx;
    bit          ok;
    int unsigned cyc_done;
    exp_lit = LIT_W'('h0F);   // var=15, polarity=0 (arbitrary valid literal)
    exp_idx = LITIDX_W'(2);
    $display("\n--- Test 5: unit propagation emits up_valid ---");
    clear_monitors();
    init_trail_mem(1);
    trail_size     = TRAIL_PTR_W'(1);
    qhead_init     = TRAIL_PTR_W'(0);
    ce_prog_status = `CLAUSE_UNIT;
    ce_prog_u_lit  = exp_lit;
    ce_prog_u_idx  = exp_idx;
    $display("  stimulus: trail_size=1, ce=CLAUSE_UNIT unit_lit=0x%0h idx=%0d", exp_lit, exp_idx);
    $display("  expected: up_valid asserted once with up_lit=0x%0h; then done; no conflict", exp_lit);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    // Check 1: liveness
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end
    // Check 2: up_valid was asserted with the correct literal
    if (!up_valid_seen) begin
      $error("FAIL: up_valid was never asserted (unit propagation expected but did not occur)");
      tests_failed++;
    end else if (up_lit_latched !== exp_lit) begin
      $error("FAIL: up_lit=0x%0h, expected 0x%0h", up_lit_latched, exp_lit);
      tests_failed++;
    end else begin
      $display("PASS: up_valid asserted with up_lit=0x%0h (correct)", up_lit_latched);
      tests_passed++;
    end
    // Check 3: no conflict raised alongside unit
    if (conflict_seen) begin
      $error("FAIL: conflict_valid raised unexpectedly during unit-propagation test (clause=%0d)",
             conflict_clause_latched);
      tests_failed++;
    end else begin
      $display("PASS: no conflict raised (correct for unit-only scenario)");
      tests_passed++;
    end
    // Restore CE to DONE default
    ce_prog_status = `CLAUSE_DONE;
    ce_prog_u_lit  = '0;
    ce_prog_u_idx  = '0;
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Test 6: Conflict detection.
  //   - 1 trail literal; CE returns CLAUSE_CONFLICT.
  //   - Expected: conflict_valid asserted; FSM goes to DONE without emitting
  //     any unit implication (up_valid must stay low).
  // --------------------------------------------------------------------------
  task automatic test_conflict_detection();
    bit          ok;
    int unsigned cyc_done;
    $display("\n--- Test 6: conflict detection emits conflict_valid ---");
    clear_monitors();
    init_trail_mem(1);
    trail_size     = TRAIL_PTR_W'(1);
    qhead_init     = TRAIL_PTR_W'(0);
    ce_prog_status = `CLAUSE_CONFLICT;
    $display("  stimulus: trail_size=1, ce=CLAUSE_CONFLICT");
    $display("  expected: conflict_valid asserted once, then done; up_valid=0 throughout");
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    // Check 1: liveness
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles; got done=%b after %0d cycles",
             TIMEOUT_CYCLES, done, cyc_done);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS: done=1 after %0d cycle(s)", cyc_done);
      tests_passed++;
    end
    // Check 2: conflict_valid was asserted
    if (!conflict_seen) begin
      $error("FAIL: conflict_valid was never asserted (conflict expected but did not occur)");
      tests_failed++;
    end else begin
      $display("PASS: conflict_valid asserted (clause=%0d, correct)", conflict_clause_latched);
      tests_passed++;
    end
    // Check 3: no unit implication emitted on a conflict path
    if (up_valid_seen) begin
      $error("FAIL: up_valid raised during conflict test (no unit prop expected; got up_lit=0x%0h)",
             up_lit_latched);
      tests_failed++;
    end else begin
      $display("PASS: up_valid never asserted (correct — conflict path does not emit implication)");
      tests_passed++;
    end
    ce_prog_status = `CLAUSE_DONE;
  endtask

  // --------------------------------------------------------------------------
  // Simulation entry point
  // --------------------------------------------------------------------------
  initial begin
    $vcdpluson;
    tests_passed             = 0;
    tests_failed             = 0;
    cycle_count              = 0;
    up_valid_seen            = 1'b0;
    up_lit_latched           = '0;
    conflict_seen            = 1'b0;
    conflict_clause_latched  = '0;
    ce_prog_status           = `CLAUSE_DONE;
    ce_prog_u_lit            = '0;
    ce_prog_u_idx            = '0;
    clk                      = 1'b0;
    rst_n                    = 1'b0;
    start                    = 1'b0;

    apply_reset();

    test_idle_before_start();

    test_zero_trail_immediate_done();
    apply_reset();

    test_nonempty_trail_runs_to_done(1);
    apply_reset();

    test_nonempty_trail_runs_to_done(4);
    apply_reset();

    // Test 4: requires a clean FSM state after prior tests.
    test_done_requires_start_low_to_re_idle();
    apply_reset();

    // Test 5: unit propagation
    test_unit_propagation();
    apply_reset();

    // Test 6: conflict detection
    test_conflict_detection();

    $display("\n========================================");
    $display("Summary: %0d passed, %0d failed", tests_passed, tests_failed);
    $display("  Tests: idle | zero-trail | nonempty(n=1) | nonempty(n=4) | done/start | unit-prop | conflict");
    $display("========================================\n");
    if (tests_failed != 0)
      $fatal(1, "One or more tests failed.");
    $finish;
  end

endmodule
