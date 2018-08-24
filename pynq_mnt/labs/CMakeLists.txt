cmake_minimum_required (VERSION 3.5)

add_subdirectory(~/labs/intcFolder ~/labs/intcFolder) // ~/labs/intcFolder/....
add_subdirectory(~/labs/btnFolder ~/labs/btnFolder)

include_directories(~/labs/btnFolder) // ~labs/intcFolder/...

add_executable(clock main.c)
target_link_libraries(clock intc)
