// Simple 4-bit counter to verify the VCS simulation environment.
module sample_counter (
    input  wire       clk,
    input  wire       rst,
    output reg  [3:0] count
);

    always @(posedge clk) begin
        if (rst)
            count <= 4'b0;
        else
            count <= count + 1;
    end

endmodule
