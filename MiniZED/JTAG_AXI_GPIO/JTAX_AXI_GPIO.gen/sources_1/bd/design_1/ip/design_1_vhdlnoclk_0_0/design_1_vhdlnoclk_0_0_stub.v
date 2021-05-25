// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
// Date        : Mon May 10 15:15:22 2021
// Host        : PF1K6K4W running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/Users/20002890/JTAX_AXI_GPIO/JTAX_AXI_GPIO.gen/sources_1/bd/design_1/ip/design_1_vhdlnoclk_0_0/design_1_vhdlnoclk_0_0_stub.v
// Design      : design_1_vhdlnoclk_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z007sclg225-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "vhdlnoclk,Vivado 2020.2" *)
module design_1_vhdlnoclk_0_0(clk65MHz)
/* synthesis syn_black_box black_box_pad_pin="clk65MHz" */;
  output clk65MHz;
endmodule