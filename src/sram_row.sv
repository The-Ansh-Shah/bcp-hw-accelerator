// SRAM Row — Stores polarity (1 bit) + value (2 bits) for one literal.
// Supports address-decoded write/read and CAM-activated (matchline) write.
module sram_row (
    input  logic       clk,
    input  logic       rst_n,

    // Mode control
    input  logic       wr_en,       // Address-decoded write enable
    input  logic       rd_en,       // Address-decoded read enable
    input  logic       cam_wr_en,   // CAM matchline-activated write enable

    // Write data
    input  logic       pol_in,      // Polarity to write
    input  logic [1:0] val_in,      // Value to write (00=0, 01=1, 10=U)

    // Read data
    output logic       pol_out,     // Stored polarity
    output logic [1:0] val_out,     // Stored value

    // Direct access (always available for processing logic)
    output logic       pol_stored,
    output logic [1:0] val_stored
);

    logic       polarity;
    logic [1:0] value;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            polarity <= 1'b0;
            value    <= 2'b01; // Unassigned (VAL_U); see hw_bcp_defs.vh
        end else if (wr_en) begin
            // Address-decoded write: update both polarity and value
            polarity <= pol_in;
            value    <= val_in;
        end else if (cam_wr_en) begin
            // CAM-activated write: update value only (polarity is fixed after LOAD)
            value <= val_in;
        end
    end

    // Read outputs (active when selected)
    assign pol_out = (rd_en) ? polarity : 1'b0;
    assign val_out = (rd_en) ? value    : 2'b00;

    // Always-available outputs for processing logic
    assign pol_stored = polarity;
    assign val_stored = value;

endmodule
