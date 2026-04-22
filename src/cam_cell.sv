// CAM Cell — Single row storing one Variable ID (VID_WIDTH bits).
// Supports write, read, and associative search modes.
module cam_cell #(
    parameter VID_WIDTH = 20
)(
    input  logic                  clk,
    input  logic                  rst_n,

    // Mode control (one-hot)
    input  logic                  wr_en,      // Address-decoded write enable
    input  logic                  rd_en,      // Address-decoded read enable
    input  logic                  search_en,  // Broadcast search enable

    // Data
    input  logic [VID_WIDTH-1:0]  data_in,    // Write data / search key
    output logic [VID_WIDTH-1:0]  data_out,   // Read data
    output logic                  match       // Search match result
);

    logic [VID_WIDTH-1:0] stored_vid;
    logic                 valid;

    // Write: store VID and mark valid
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            stored_vid <= '0;
            valid      <= 1'b0;
        end else if (wr_en) begin
            stored_vid <= data_in;
            valid      <= 1'b1;
        end
    end

    // Read: drive data_out when selected
    assign data_out = (rd_en) ? stored_vid : '0;

    // Search: compare stored VID against broadcast key
    assign match = search_en & valid & (stored_vid == data_in);

endmodule
