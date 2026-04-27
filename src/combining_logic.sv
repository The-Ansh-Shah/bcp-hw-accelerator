// Combining Logic — Priority reduction of clause evaluation results.
// Reduces NUM_CLAUSES processing outputs to a single {conf, up, done, cid, up_lit_pos}.
// Priority: CONF > UP > DONE
//
// Implemented as a priority encoder (functionally equivalent to binary tree).
module combining_logic #(
    parameter NUM_CLAUSES = 16,
    parameter CID_WIDTH   = $clog2(NUM_CLAUSES)
)(
    input  logic              conf_in  [NUM_CLAUSES],
    input  logic              up_in    [NUM_CLAUSES],
    input  logic              done_in  [NUM_CLAUSES],
    input  logic [1:0]        up_lit_pos_in [NUM_CLAUSES],

    output logic              conf_out,
    output logic              up_out,
    output logic              done_out,
    output logic [CID_WIDTH-1:0] cid_out,
    output logic [1:0]        up_lit_pos_out
);

    always_comb begin
        conf_out       = 1'b0;
        up_out         = 1'b0;
        done_out       = 1'b0;
        cid_out        = '0;
        up_lit_pos_out = 2'd0;

        // CONF: highest priority — first clause in conflict wins
        for (int i = NUM_CLAUSES-1; i >= 0; i--) begin
            if (conf_in[i]) begin
                conf_out = 1'b1;
                cid_out  = CID_WIDTH'(i);
            end
        end

        if (!conf_out) begin
            // UP: first unit-propagation clause wins
            for (int i = NUM_CLAUSES-1; i >= 0; i--) begin
                if (up_in[i]) begin
                    up_out         = 1'b1;
                    cid_out        = CID_WIDTH'(i);
                    up_lit_pos_out = up_lit_pos_in[i];
                end
            end

            if (!up_out) begin
                // DONE: all clauses must be satisfied
                done_out = 1'b1;
                for (int i = 0; i < NUM_CLAUSES; i++) begin
                    if (!done_in[i]) done_out = 1'b0;
                end
            end
        end
    end

endmodule
