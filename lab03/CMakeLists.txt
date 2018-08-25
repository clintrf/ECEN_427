cmake_minimum_required (VERSION 3.5)

add_subdirectory(/home/xilinx/ECEN_427/lab03/image_render /home/xilinx/ECEN_427/lab03/image_render)
# add_subdirectory(/home/xilinx/ECEN_427/lab02/uioFolder /home/xilinx/ECEN_427/lab02/uioFolder)

include_directories(/home/xilinx/ECEN_427/lab03/image_render)
# include_directories(/home/xilinx/ECEN_427/lab02/uioFolder)

add_executable(space_invaders main.c)
target_link_libraries(space_invaders hdmi)
target_link_libraries(space_invaders sprites)
target_link_libraries(space_invaders render)
