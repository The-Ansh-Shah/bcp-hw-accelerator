// Testbench for sample_counter — sanity-check of the VCS flow.
`timescale 1ns/1ps

module sample_counter_tb;

    reg        clk;
    reg        rst;
    wire [3:0] count;

    sample_counter dut (
        .clk   (clk),
        .rst   (rst),
        .count (count)
    );

    // 10 ns clock period
    initial clk = 0;
    always #5 clk = ~clk;

    integer num_errors;

    initial begin
        $vcdpluson;
        num_errors = 0;
        rst = 1;
        @(posedge clk); #1;
        @(posedge clk); #1;
        rst = 0;

        // Let the counter run for 16 cycles and check a few values
        repeat (16) @(posedge clk);
        #1;

        // After 16 clocks (no reset), counter should have wrapped back to 0
        // (it increments on each posedge, 16 mod 16 = 0 for a 4-bit counter)
        if (count !== 4'd0) begin
            $display("FAIL: expected count=0 after 16 cycles, got %0d", count);
            num_errors = num_errors + 1;
        end

        // Run a few more and spot-check
        repeat (3) @(posedge clk);
        #1;
        if (count !== 4'd3) begin
            $display("FAIL: expected count=3, got %0d", count);
            num_errors = num_errors + 1;
        end

        // Test reset mid-count
        rst = 1;
        @(posedge clk); #1;
        if (count !== 4'd0) begin
            $display("FAIL: expected count=0 after reset, got %0d", count);
            num_errors = num_errors + 1;
        end
        rst = 0;

        repeat (5) @(posedge clk);
        #1;
        if (count !== 4'd5) begin
            $display("FAIL: expected count=5, got %0d", count);
            num_errors = num_errors + 1;
        end

        if (num_errors == 0)
            $display("ALL TESTS PASSED");
        else
            $display("FINISHED WITH %0d ERROR(S)", num_errors);

        $finish;
    end

endmodule
