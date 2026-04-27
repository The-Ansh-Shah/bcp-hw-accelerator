// Processing Logic — Evaluates one clause of LITS_PER_CLAUSE literals combinationally.
// Inputs: value, polarity, and matchline for each literal.
// Outputs: clause status (CONF, UP, DONE) and UP literal position.
module processing_logic (
    input  logic [1:0] val  [4],  // 2-bit value per literal
    input  logic       pol  [4],  // Polarity per literal
    input  logic       ml   [4],  // CAM matchline per literal

    output logic       conf,       // Conflict: all literals false
    output logic       up,         // Unit propagation: exactly one unassigned, rest false
    output logic       done,       // Satisfied: at least one literal true
    output logic [1:0] up_lit_pos  // Which literal position (0-3) is the UP candidate
);

    // Value encoding (matches sat_submodule.sv / hw_bcp_defs.vh):
    //   2'b00 = assigned 0 (VAL_ZERO)
    //   2'b11 = assigned 1 (VAL_ONE)
    //   2'b01 = unassigned (VAL_U)
    localparam VAL_U = 2'b01;

    // Effective value computation for each literal
    logic [3:0] lit_true;
    logic [3:0] lit_false;
    logic [3:0] lit_unassigned;

    genvar i;
    generate
        for (i = 0; i < 4; i++) begin : eval
            assign lit_unassigned[i] = (val[i] == VAL_U);
            // Literal is TRUE when assignment matches polarity:
            //   pol=0 (positive literal xi):  true when val=VAL_ONE  (val[0]=1)
            //   pol=1 (negative literal ~xi): true when val=VAL_ZERO (val[0]=0)
            assign lit_true[i]  = ~lit_unassigned[i] & (val[i][0] == ~pol[i]);
            assign lit_false[i] = ~lit_unassigned[i] & (val[i][0] ==  pol[i]);
        end
    endgenerate

    // Count unassigned literals (max 4 → 3 bits)
    logic [2:0] num_unassigned;
    assign num_unassigned = {2'b0, lit_unassigned[0]} + {2'b0, lit_unassigned[1]}
                          + {2'b0, lit_unassigned[2]} + {2'b0, lit_unassigned[3]};

    // Clause status
    assign done = |lit_true;
    assign conf = &lit_false;
    assign up   = (num_unassigned == 3'd1) & (&(lit_false | lit_unassigned))
                & ~done & (ml[0] | ml[1] | ml[2] | ml[3]);

    // Which literal position is unassigned (for UP readback)
    always_comb begin
        if      (lit_unassigned[0]) up_lit_pos = 2'd0;
        else if (lit_unassigned[1]) up_lit_pos = 2'd1;
        else if (lit_unassigned[2]) up_lit_pos = 2'd2;
        else                        up_lit_pos = 2'd3;
    end

endmodule
