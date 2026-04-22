// Processing Logic — Evaluates one 3-SAT clause combinationally.
// Inputs: value, polarity, and matchline for each of 3 literals.
// Outputs: clause status (CONF, UP, DONE) and UP literal position.
module processing_logic (
    input  logic [1:0] val  [3],  // 2-bit value per literal
    input  logic       pol  [3],  // Polarity per literal
    input  logic       ml   [3],  // CAM matchline per literal

    output logic       conf,       // Conflict: all literals false
    output logic       up,         // Unit propagation: exactly one unassigned, rest false
    output logic       done,       // Satisfied: at least one literal true
    output logic [1:0] up_lit_pos  // Which literal position (0-2) is the UP candidate
);

    // Effective value computation for each literal
    logic [2:0] lit_true;
    logic [2:0] lit_false;
    logic [2:0] lit_unassigned;

    genvar i;
    generate
        for (i = 0; i < 3; i++) begin : eval
            // Value encoding: 2'b00 = assigned 0, 2'b01 = assigned 1, 2'b10 = unassigned
            assign lit_unassigned[i] = (val[i] == 2'b10);
            // Literal is TRUE when assignment matches polarity:
            //   pol=0 (positive literal xi):  true when val=1
            //   pol=1 (negative literal ~xi): true when val=0
            assign lit_true[i]  = ~lit_unassigned[i] & (val[i][0] == ~pol[i]);
            assign lit_false[i] = ~lit_unassigned[i] & (val[i][0] == pol[i]);
        end
    endgenerate

    // Count unassigned literals
    logic [1:0] num_unassigned;
    assign num_unassigned = {1'b0, lit_unassigned[0]} + {1'b0, lit_unassigned[1]}
                          + {1'b0, lit_unassigned[2]};

    // Clause status
    assign done = |lit_true;                                         // At least one true
    assign conf = &lit_false;                                        // All three false
    assign up   = (num_unassigned == 2'd1) & (&(lit_false | lit_unassigned))
                & ~done & (ml[0] | ml[1] | ml[2]);                   // One unassigned, rest false, matchline active

    // Which literal position is unassigned (for UP readback)
    always_comb begin
        if (lit_unassigned[0])      up_lit_pos = 2'd0;
        else if (lit_unassigned[1]) up_lit_pos = 2'd1;
        else                        up_lit_pos = 2'd2;
    end

endmodule
