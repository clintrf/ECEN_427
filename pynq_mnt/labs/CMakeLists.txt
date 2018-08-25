cmake_minimum_required (VERSION 3.5)

add_subdirectory(/home/xilinx/ECEN_427/pynq_mnt/labs/intcFolder /home/xilinx/ECEN_427/pynq_mnt/labs/intcFolder)
add_subdirectory(/home/xilinx/ECEN_427/pynq_mnt/labs/btnFolder /home/xilinx/ECEN_427/pynq_mnt/labs/btnFolder)

include_directories(/home/xilinx/ECEN_427/pynq_mnt/labs/btnFolder)

add_executable(clock main.c)
target_link_libraries(clock intc)
