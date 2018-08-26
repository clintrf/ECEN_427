cmake_minimum_required (VERSION 3.5)

add_subdirectory(/home/xilinx/ECEN_427/lab02/intcFolder /home/xilinx/ECEN_427/lab02/intcFolder)
add_subdirectory(/home/xilinx/ECEN_427/lab02/uioFolder /home/xilinx/ECEN_427/lab02/uioFolder)

include_directories(/home/xilinx/ECEN_427/lab02/intcFolder)
include_directories(/home/xilinx/ECEN_427/lab02/uioFolder)

add_executable(clock main.c)
target_link_libraries(clock btn)
target_link_libraries(clock swtch)
target_link_libraries(clock intc)
