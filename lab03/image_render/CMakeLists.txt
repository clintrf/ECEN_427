cmake_minimum_required (VERSION 3.5)

add_subdirectory(/home/xilinx/ECEN_427/lab03/image_render/hdmi /home/xilinx/ECEN_427/lab03/image_render/hdmi)
add_subdirectory(/home/xilinx/ECEN_427/lab03/image_render/sprites /home/xilinx/ECEN_427/lab03/image_render/sprites)

include_directories(/home/xilinx/ECEN_427/lab03/image_render/hdmi)
include_directories(/home/xilinx/ECEN_427/lab03/image_render/sprites)

add_library(render STATIC image_render.c)
target_link_libraries(render hdmi)
target_link_libraries(render sprites)
