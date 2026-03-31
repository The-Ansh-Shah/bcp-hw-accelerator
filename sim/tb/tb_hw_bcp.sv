`timescale 1ns/1ps

module tb_hw_bcp;

  localparam int unsigned VAR_W         = 6;
  localparam int unsigned LIT_W         = VAR_W + 1;
  localparam int unsigned CLAUSE_W      = 6;
  localparam int unsigned LITIDX_W      = 6;
  localparam int unsigned REASON_W      = CLAUSE_W;
  localparam int unsigned TRAIL_PTR_W   = 6;

  logic clk, rst_n;
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

  // Simple always-ready consumer for unit props (testbench can add behavior later)
  assign up_ready = 1'b1;

  // Use the provided sim stubs in hw_bcp.v by compiling with +define+HW_BCP_SIM_STUBS
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

  stub_cam #(
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

  initial begin
    // Match the repo's VCS flow (see test_env/): enable VPD dumping.
    // The output filename is typically controlled by runtime plusarg:
    //   ./simv +vcdplusfile=<name>.vpd
    $vcdpluson;

    clk = 0;
    rst_n = 0;
    start = 0;
    qhead_init = '0;
    trail_size = 4; // number of pops

    // Example trail literals (sign,var). You can override stub mem[] with hierarchy.
    // This TB leaves stub_trail.mem[] uninitialized by default; set it in your sim.

    repeat (4) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    start = 1;
    @(posedge clk);
    start = 0;

    // Run for a bit
    repeat (50) @(posedge clk);
    $finish;
  end

  always @(posedge clk) begin
    if (conflict_valid) begin
      $display("[%0t] CONFLICT clause=%0d", $time, conflict_clause);
    end
    if (up_valid && up_ready) begin
      $display("[%0t] UNIT lit=0x%0h reason=%0d idx=%0d", $time, up_lit, up_reason_clause, up_lit_idx);
    end
    if (done) begin
      $display("[%0t] DONE", $time);
    end
  end
endmodule

