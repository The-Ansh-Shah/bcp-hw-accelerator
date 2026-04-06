`timescale 1ns/1ps

// ============================================================================
// tb_hw_bcp — HW-BCP propagate FSM testbench
// ============================================================================
// The DUT (hw_bcp_propagate) and stub_trail use synchronous active-low reset:
// after releasing rst_n, wait at least one posedge clk before driving meaningful
// control (apply_reset does this).
//
// The stock stub_cam never asserts r_valid, so the FSM would stick in CAM_STREAM.
// This file uses tb_cam_empty_watchlist instead: after each CAM query handshake it
// pulses one clause read with r_last=1 (empty watch list for that variable), which
// matches how the skeleton flow is meant to complete.
// ============================================================================

// Emits a single (r_valid,r_last) beat after each CAM query accept so the DUT can
// leave CAM_STREAM and return to the trail pop loop.
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

  // Outputs
  logic conflict_valid;
  logic [CLAUSE_W-1:0] conflict_clause;
  logic up_valid, up_ready;
  logic [LIT_W-1:0] up_lit;
  logic [REASON_W-1:0] up_reason_clause;
  logic [LITIDX_W-1:0] up_lit_idx;

  int unsigned tests_passed;
  int unsigned tests_failed;

  // Decode DUT FSM state for messages (must match hw_bcp_defs.vh / hw_bcp_propagate.v).
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

  stub_clause_eval #(
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
    rst_n = 1'b0;
    start = 1'b0;
    qhead_init = '0;
    trail_size = '0;
    repeat (5) @(posedge clk);
    rst_n = 1'b1;
    @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Wait until done or cycle timeout. cycles_used = clocks until done (1-based),
  // or max_cycles if timed out.
  // --------------------------------------------------------------------------
  task automatic wait_done_timeout(
    input int unsigned max_cycles,
    output bit ok,
    output int unsigned cycles_used
  );
    int unsigned cyc;
    ok = 1'b0;
    cycles_used = max_cycles;
    cyc  = 0;
    while (cyc < max_cycles) begin
      @(posedge clk);
      cyc++;
`ifdef TB_DEBUG
      if (cyc <= 60)
        $display("  [wait_done cyc=%0d] done=%b busy=%b trail_pop=%b cam_q=%b ce_v=%b",
                 cyc, done, busy, trail_pop_req, cam_q_valid, ce_valid);
`endif
      if (done) begin
        ok = 1'b1;
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
    $display("  expected: busy=0, done=0 (no run in progress)");
    if (busy !== 1'b0 || done !== 1'b0) begin
      $error("FAIL: got busy=%b done=%b (start=%b)", busy, done, start);
      print_dut_snapshot("fail");
      tests_failed++;
    end else begin
      $display("PASS: got busy=%b done=%b", busy, done);
      tests_passed++;
    end
  endtask

  // --------------------------------------------------------------------------
  // Test 2: Zero-length trail (trail_size == qhead_init conceptual window).
  // With qhead_init=0 and trail_size=0, POP_REQ sees qhead < trail_size false
  // and the FSM should jump straight to DONE without popping.
  // --------------------------------------------------------------------------
  task automatic test_zero_trail_immediate_done();
    bit ok;
    int unsigned cyc_done;
    $display("\n--- Test 2: zero trail completes without pop ---");
    trail_size = TRAIL_PTR_W'(0);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  stimulus: trail_size=%0d qhead_init=%0d (nothing to pop)", trail_size, qhead_init);
    $display("  expected: done=1 within %0d cycles, then busy=0 (finished, idle)", TIMEOUT_CYCLES);
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
      $display("PASS: got done=1 after %0d posedge(s), busy=%b (matches expected)", cyc_done, busy);
      tests_passed++;
    end
    // Wait until start can be re-armed (DUT returns to IDLE when start drops)
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Test 3: Several trail literals — each needs CAM empty list + CE DONE; stub
  // trail pops literals from mem[0..n-1]. Expect run to finish with done.
  // --------------------------------------------------------------------------
  task automatic test_nonempty_trail_runs_to_done(input int unsigned n);
    bit ok;
    int unsigned cyc_done;
    $display("\n--- Test 3: nonempty trail (n=%0d literals) runs to done ---", n);
    init_trail_mem(n);
    trail_size = TRAIL_PTR_W'(n);
    qhead_init = TRAIL_PTR_W'(0);
    $display("  stimulus: trail_size=%0d qhead_init=%0d (pop n literals, CAM+CE per lit)", trail_size, qhead_init);
    $display("  expected: done=1 within %0d cycles after start (propagation finished)", TIMEOUT_CYCLES);
    pulse_start();
    wait_done_timeout(TIMEOUT_CYCLES, ok, cyc_done);
    if (!ok) begin
      $error("FAIL: expected done=1 within %0d cycles (n=%0d); got done=%b after %0d cycles | busy=%b",
             TIMEOUT_CYCLES, n, done, cyc_done, busy);
      print_dut_snapshot("timeout");
      tests_failed++;
    end else begin
      $display("PASS: got done=1 after %0d posedge(s) (n=%0d trail literals as expected)", cyc_done, n);
      tests_passed++;
    end
    @(posedge clk);
    while (start !== 1'b0) @(posedge clk);
    repeat (2) @(posedge clk);
  endtask

  // --------------------------------------------------------------------------
  // Test 4: With start held high, FSM reaches S_DONE (done=1). It stays there
  // until start deasserts; then on the next cycle the FSM can return to IDLE
  // (done=0). This matches synchronous-reset control: re-arm by dropping start.
  // --------------------------------------------------------------------------
  task automatic test_done_requires_start_low_to_re_idle();
    bit ok;
    int unsigned cyc_done;
    logic done_sample;
    $display("\n--- Test 4: start must deassert to return from done ---");
    $display("  stimulus: trail_size=0, start held 1 until done, then start=0");
    $display("  expected: (a) done=1 in S_DONE with start=1; (b) after start=0, next posedge + #1ps, done=0 (idle)");
    trail_size = TRAIL_PTR_W'(0);
    qhead_init = TRAIL_PTR_W'(0);
    start      = 1'b1;
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
    $display("  checkpoint: got done=1 after %0d clk(s) with start=1 (expected)", cyc_done);
    // Deassert start, then wait one posedge + 1ps so NBA updates to state
    // and assign done= (state==S_DONE) settle before sampling (VCS ordering).
    start = 1'b0;
    @(posedge clk);
    #1;
    done_sample = done;
    if (done_sample !== 1'b0) begin
      $error("FAIL: expected done=0, busy=0, state=S_IDLE after start=0 + 1 posedge + #1ps sample");
      $error("      got done=%b, busy=%b, start=%b, state=%s",
             done_sample, busy, start, fsm_state_str(dut.state));
      print_dut_snapshot("fail test 4");
      tests_failed++;
    end else begin
      $display("PASS: expected done=0, busy=0 in idle; got done=%b busy=%b start=%b",
               done, busy, start);
      tests_passed++;
    end
  endtask

  initial begin
    $vcdpluson;
    tests_passed = 0;
    tests_failed = 0;

    clk   = 1'b0;
    rst_n = 1'b0;
    start = 1'b0;

    apply_reset();

    test_idle_before_start();

    test_zero_trail_immediate_done();
    apply_reset();

    test_nonempty_trail_runs_to_done(1);
    apply_reset();

    test_nonempty_trail_runs_to_done(4);
    apply_reset();

    // Test 4 assumes a clean FSM (prior tests may have timed out mid-run).
    test_done_requires_start_low_to_re_idle();

    $display("\n========================================");
    $display("Summary: %0d passed, %0d failed (5 checkpoints: idle, zero-trail, n=1, n=4, done/start)", tests_passed, tests_failed);
    $display("========================================\n");
    if (tests_failed != 0)
      $fatal(1, "One or more tests failed.");
    $finish;
  end

  always @(posedge clk) begin
    if (conflict_valid)
      $display("[%0t] CONFLICT clause=%0d", $time, conflict_clause);
    if (up_valid && up_ready)
      $display("[%0t] UNIT lit=0x%0h reason=%0d idx=%0d", $time, up_lit, up_reason_clause, up_lit_idx);
    if (done)
      $display("[%0t] DONE", $time);
  end
endmodule
