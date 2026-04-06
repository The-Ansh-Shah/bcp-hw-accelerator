// ============================================================================
// Simulation-only stub models for HW-BCP (compile with +define+HW_BCP_SIM_STUBS)
// ============================================================================
//
// These replace real CAM / SRAM / clause-eval / trail memories so tb_hw_bcp
// can elaborate without full RTL. They are not synthesizable stand-ins for
// timing-accurate behavior.
//
`include "hw_bcp_defs.vh"

`ifdef HW_BCP_SIM_STUBS

// ----------------------------------------------------------------------------
// stub_trail — simple register file + pointer: pops literals when pop_req
//              asserts and rd_ptr < trail_size. Synchronous active-low reset.
// ----------------------------------------------------------------------------
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
  reg             pop_req_d;

  // One pop per rising edge of pop_req while rd_ptr < trail_size. The DUT holds
  // trail_pop_req across POP_WAIT until pop_valid is seen; we must not advance
  // rd_ptr every cycle while pop_req is stuck high.
  always @(posedge clk) begin
    if (!rst_n) begin
      rd_ptr     <= {PTR_W{1'b0}};
      pop_valid  <= 1'b0;
      pop_lit    <= {LIT_W{1'b0}};
      pop_req_d  <= 1'b0;
    end else begin
      pop_req_d <= pop_req;
      pop_valid <= 1'b0;
      if (pop_req && !pop_req_d && (rd_ptr < trail_size)) begin
        pop_lit   <= mem[rd_ptr];
        rd_ptr    <= rd_ptr + 1'b1;
        pop_valid <= 1'b1;
      end
    end
  end
endmodule

// ----------------------------------------------------------------------------
// stub_cam — combinational placeholder: never drives valid clause data
//            (q_ready=1, r_valid=0) so propagation exits quickly in skeleton sim.
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// stub_sram_litval — trivial assignment store: reads return VALU; writes acked.
// ----------------------------------------------------------------------------
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
  logic rd_resp_valid_r;

  always_ff @(posedge clk) begin
    if (!rst_n)
      rd_resp_valid_r <= 1'b0;
    else if (rd_resp_valid_r && rd_resp_ready)
      rd_resp_valid_r <= 1'b0;
    else if (rd_valid && rd_ready)
      rd_resp_valid_r <= 1'b1;
  end

  always @(*) begin
    rd_ready       = 1'b1;
    rd_resp_valid  = rd_resp_valid_r;
    rd_val         = `VALU;
    wr_ready       = 1'b1;
  end
endmodule

// ----------------------------------------------------------------------------
// stub_clause_eval — when a request handshakes, raises resp_valid for one
//                    accepted response cycle (CLAUSE_DONE, no unit info).
//                    Response is registered so it stays valid after req_valid
//                    drops in the DUT’s wait state (req/resp split transaction).
// ----------------------------------------------------------------------------
module stub_clause_eval #(
  parameter CLAUSE_W  = 8,
  parameter LIT_W     = 11,
  parameter LITIDX_W  = 8
) (
  input  wire                   clk,
  input  wire                   rst_n,
  input  wire                   req_valid,
  output wire                   req_ready,
  input  wire [CLAUSE_W-1:0]    req_clause,
  output reg                    resp_valid,
  input  wire                   resp_ready,
  output reg [1:0]              resp_status,
  output reg [LIT_W-1:0]        resp_u_lit,
  output reg [LITIDX_W-1:0]     resp_u_idx
);
  // Tie high from time 0 so master never sees X on the ready line.
  assign req_ready = 1'b1;

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      resp_valid  <= 1'b0;
      resp_status <= `CLAUSE_DONE;
      resp_u_lit  <= {LIT_W{1'b0}};
      resp_u_idx  <= {LITIDX_W{1'b0}};
    end else begin
      if (resp_valid && resp_ready) begin
        resp_valid <= 1'b0;
      end else if (req_valid && req_ready) begin
        resp_valid  <= 1'b1;
        resp_status <= `CLAUSE_DONE;
        resp_u_lit  <= {LIT_W{1'b0}};
        resp_u_idx  <= {LITIDX_W{1'b0}};
      end
    end
  end
endmodule

`endif
