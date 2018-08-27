cmake_minimum_required (VERSION 3.5)

add_subdirectory(/home/xilinx/ECEN_427/lab04/audio_driver /home/xilinx/ECEN_427/lab04/audio_driver)

include_directories(/home/xilinx/ECEN_427/lab04/audio_driver)

add_executable(audio_test main.c)
target_link_libraries(audio_test audio_driver)
