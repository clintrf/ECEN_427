cmake_minimum_required (VERSION 3.5)

add_subdirectory(<path_to_intc_driver> <path_to_intc_driver>) // ~/labs/intcFolder/....

include_directories(<path_to_intc_driver>) //~labs/intcFolder/...

add_executable(clock main.c)
target_link_libraries(clock intc)
