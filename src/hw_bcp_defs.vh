// ============================================================================
// HW-BCP shared `defines — included by the propagate FSM and simulation stubs.
// ============================================================================
//
// Clause evaluation (CE) reports one of these per watched clause under the
// current partial assignment:
//   DONE     — clause satisfied or has multiple unassigned literals (no action).
//   UNIT     — exactly one unassigned literal remains (unit clause).
//   CONFLICT — all literals falsified (conflict clause).
//
`define CLAUSE_DONE     2'b00
`define CLAUSE_UNIT     2'b01
`define CLAUSE_CONFLICT 2'b10
//
// Literal value encoding in SRAM / CE responses (2-bit):
//   VAL0 — false; VAL1 — true; VALU — unassigned.
//
`define VAL0            2'b00
`define VALU            2'b01
`define VAL1            2'b11

// ----------------------------------------------------------------------------
// FSM state encoding for hw_bcp_propagate (one-hot-style numeric ids).
// See hw_bcp_propagate.v header for the control-flow narrative.
// ----------------------------------------------------------------------------
`define S_IDLE          4'd0
`define S_POP_REQ       4'd1
`define S_POP_WAIT      4'd2
`define S_CAM_Q         4'd3
`define S_CAM_STREAM    4'd4
`define S_CE_REQ        4'd5
`define S_CE_WAIT       4'd6
`define S_HANDLE_UNIT   4'd7
`define S_LITVAL_REQ    4'd8
`define S_LITVAL_WAIT   4'd9
`define S_EMIT_UP       4'd10
`define S_REPORT_CONFL  4'd11
`define S_DONE          4'd12
