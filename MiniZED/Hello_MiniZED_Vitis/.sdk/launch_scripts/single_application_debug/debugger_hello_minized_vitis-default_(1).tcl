connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Avnet MiniZed V1 1234-oj1A" && level==0} -index 1
fpga -file C:/Github/Own/Embedded-FPGA/MiniZED/Hello_MiniZED_Vitis/Hello_MiniZED_Vitis/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Github/Own/Embedded-FPGA/MiniZED/Hello_MiniZED_Vitis/design_1_wrapper/export/design_1_wrapper/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Github/Own/Embedded-FPGA/MiniZED/Hello_MiniZED_Vitis/Hello_MiniZED_Vitis/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Github/Own/Embedded-FPGA/MiniZED/Hello_MiniZED_Vitis/Hello_MiniZED_Vitis/Debug/Hello_MiniZED_Vitis.elf
configparams force-mem-access 0
bpadd -addr &main