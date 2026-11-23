# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\Jamal\Classes2026\ECE520\ECE_520_Lab_4_AXI_Lite\mult_soc_jb\mult_soc_vitis\mult_plat_BRAM\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\Jamal\Classes2026\ECE520\ECE_520_Lab_4_AXI_Lite\mult_soc_jb\mult_soc_vitis\mult_plat_BRAM\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {mult_plat_BRAM}\
-hw {C:\Users\Jamal\Classes2026\ECE520\ECE_520_Lab_4_AXI_Lite\mult_soc_jb\mult_bd_jb_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/Jamal/Classes2026/ECE520/ECE_520_Lab_4_AXI_Lite/mult_soc_jb/mult_soc_vitis}

platform write
platform generate -domains 
platform active {mult_plat_BRAM}
domain active {zynq_fsbl}
bsp reload
bsp reload
domain active {standalone_domain}
bsp reload
bsp setdriver -ip mult_16_bit_0 -driver none -ver {}
bsp write
bsp reload
catch {bsp regenerate}
bsp reload
domain active {zynq_fsbl}
bsp setdriver -ip mult_16_bit_0 -driver none -ver {}
bsp write
bsp reload
catch {bsp regenerate}
platform generate
