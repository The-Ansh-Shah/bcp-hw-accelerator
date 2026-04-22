// CAM-SRAM Row — Combined single row: CAM cell + SRAM row.
// The CAM matchline drives the SRAM wordline in CAM-activated write mode.
module cam_sram_row #(
    parameter VID_WIDTH = 20
)(
    input  logic                  clk,
    input  logic                  rst_n,

    // CAM mode control
    input  logic                  cam_wr_en,    // CAM address-decoded write
    input  logic                  cam_rd_en,    // CAM address-decoded read
    input  logic                  cam_search_en,// CAM broadcast search

    // SRAM mode control
    input  logic                  sram_wr_en,   // SRAM address-decoded write
    input  logic                  sram_rd_en,   // SRAM address-decoded read
    input  logic                  cam_act_wr,   // Enable CAM-activated SRAM write mode

    // CAM data
    input  logic [VID_WIDTH-1:0]  vid_in,       // VID write data / search key
    output logic [VID_WIDTH-1:0]  vid_out,      // VID read data

    // SRAM data
    input  logic                  pol_in,
    input  logic [1:0]            val_in,
    output logic                  pol_out,
    output logic [1:0]            val_out,

    // Direct access for processing logic
    output logic                  pol_stored,
    output logic [1:0]            val_stored,

    // Matchline output (exposed for processing logic)
    output logic                  matchline
);

    // CAM cell
    cam_cell #(.VID_WIDTH(VID_WIDTH)) u_cam (
        .clk       (clk),
        .rst_n     (rst_n),
        .wr_en     (cam_wr_en),
        .rd_en     (cam_rd_en),
        .search_en (cam_search_en),
        .data_in   (vid_in),
        .data_out  (vid_out),
        .match     (matchline)
    );

    // SRAM wordline: either address-decoded or matchline-activated
    logic sram_cam_wr_en;
    assign sram_cam_wr_en = cam_act_wr & matchline;

    // SRAM row
    sram_row u_sram (
        .clk        (clk),
        .rst_n      (rst_n),
        .wr_en      (sram_wr_en),
        .rd_en      (sram_rd_en),
        .cam_wr_en  (sram_cam_wr_en),
        .pol_in     (pol_in),
        .val_in     (val_in),
        .pol_out    (pol_out),
        .val_out    (val_out),
        .pol_stored (pol_stored),
        .val_stored (val_stored)
    );

endmodule
