# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\Jamal\Classes2026\ECE520\ECE_520_Lab_4_AXI_Lite\mult_soc_jb\mult_soc_vitis\system_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\Jamal\Classes2026\ECE520\ECE_520_Lab_4_AXI_Lite\mult_soc_jb\mult_soc_vitis\system_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {system_wrapper}\
-hw {C:\Users\Jamal\Classes2026\ECE520\ECE_520_Final_Project\hw\hft_engine\system_wrapper.xsa}\
-out {C:/Users/Jamal/Classes2026/ECE520/ECE_520_Lab_4_AXI_Lite/mult_soc_jb/mult_soc_vitis}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {lwip_echo_server}
platform generate -domains 
platform active {system_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
bsp reload
domain active {zynq_fsbl}
bsp reload
domain active {standalone_ps7_cortexa9_0}
bsp config lwip_dhcp "false"
bsp write
bsp reload
catch {bsp regenerate}
domain active {zynq_fsbl}
bsp reload
