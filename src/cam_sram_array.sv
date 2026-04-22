// CAM-SRAM Array — Full array of NUM_ROWS combined CAM/SRAM rows.
// Provides address-decoded access and broadcast search with matchline-activated writes.
module cam_sram_array #(
    parameter VID_WIDTH   = 20,
    parameter NUM_CLAUSES = 8,
    parameter NUM_ROWS    = 3 * NUM_CLAUSES
)(
    input  logic                              clk,
    input  logic                              rst_n,

    // Mode control
    input  logic                              search_en,   // Broadcast search mode
    input  logic                              cam_act_wr,  // Enable CAM-activated SRAM writes
    input  logic                              addr_wr_en,  // Address-decoded write (CAM + SRAM)
    input  logic                              addr_rd_en,  // Address-decoded read (CAM + SRAM)

    // Address (for write/read modes)
    input  logic [$clog2(NUM_ROWS)-1:0]       row_addr,

    // Data inputs
    input  logic [VID_WIDTH-1:0]              vid_in,      // VID write data / search key
    input  logic                              pol_in,      // Polarity write data
    input  logic [1:0]                        val_in,      // Value write data

    // Addressed read outputs
    output logic [VID_WIDTH-1:0]              vid_out,
    output logic                              pol_out,
    output logic [1:0]                        val_out,

    // Direct access outputs for processing logic (all rows)
    output logic                              pol_stored [NUM_ROWS],
    output logic [1:0]                        val_stored [NUM_ROWS],
    output logic                              matchlines [NUM_ROWS]
);

    // Per-row control signals
    logic cam_wr_sel  [NUM_ROWS];
    logic cam_rd_sel  [NUM_ROWS];
    logic sram_wr_sel [NUM_ROWS];
    logic sram_rd_sel [NUM_ROWS];

    // Per-row outputs
    logic [VID_WIDTH-1:0] row_vid_out [NUM_ROWS];
    logic                 row_pol_out [NUM_ROWS];
    logic [1:0]           row_val_out [NUM_ROWS];

    // Address decoder: generate per-row select signals
    always_comb begin
        for (int r = 0; r < NUM_ROWS; r++) begin
            cam_wr_sel[r]  = addr_wr_en & (row_addr == r[$clog2(NUM_ROWS)-1:0]);
            cam_rd_sel[r]  = addr_rd_en & (row_addr == r[$clog2(NUM_ROWS)-1:0]);
            sram_wr_sel[r] = addr_wr_en & (row_addr == r[$clog2(NUM_ROWS)-1:0]);
            sram_rd_sel[r] = addr_rd_en & (row_addr == r[$clog2(NUM_ROWS)-1:0]);
        end
    end

    // Instantiate rows
    genvar r;
    generate
        for (r = 0; r < NUM_ROWS; r++) begin : rows
            cam_sram_row #(.VID_WIDTH(VID_WIDTH)) u_row (
                .clk           (clk),
                .rst_n         (rst_n),
                .cam_wr_en     (cam_wr_sel[r]),
                .cam_rd_en     (cam_rd_sel[r]),
                .cam_search_en (search_en),
                .sram_wr_en    (sram_wr_sel[r]),
                .sram_rd_en    (sram_rd_sel[r]),
                .cam_act_wr    (cam_act_wr),
                .vid_in        (vid_in),
                .vid_out       (row_vid_out[r]),
                .pol_in        (pol_in),
                .val_in        (val_in),
                .pol_out       (row_pol_out[r]),
                .val_out       (row_val_out[r]),
                .pol_stored    (pol_stored[r]),
                .val_stored    (val_stored[r]),
                .matchline     (matchlines[r])
            );
        end
    endgenerate

    // Output mux for addressed reads
    always_comb begin
        vid_out = '0;
        pol_out = 1'b0;
        val_out = 2'b00;
        for (int r = 0; r < NUM_ROWS; r++) begin
            if (cam_rd_sel[r]) begin
                vid_out = row_vid_out[r];
                pol_out = row_pol_out[r];
                val_out = row_val_out[r];
            end
        end
    end

endmodule
