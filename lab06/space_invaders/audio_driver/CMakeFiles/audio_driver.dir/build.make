# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xilinx/ECEN_427/lab06/space_invaders

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xilinx/ECEN_427/lab06/space_invaders

# Include any dependencies generated for this target.
include audio_driver/CMakeFiles/audio_driver.dir/depend.make

# Include the progress variables for this target.
include audio_driver/CMakeFiles/audio_driver.dir/progress.make

# Include the compile flags for this target's objects.
include audio_driver/CMakeFiles/audio_driver.dir/flags.make

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o: audio_driver/CMakeFiles/audio_driver.dir/flags.make
audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o: audio_driver/audio_driver.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/ECEN_427/lab06/space_invaders/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o"
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/audio_driver.dir/audio_driver.c.o   -c /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver/audio_driver.c

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/audio_driver.dir/audio_driver.c.i"
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver/audio_driver.c > CMakeFiles/audio_driver.dir/audio_driver.c.i

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/audio_driver.dir/audio_driver.c.s"
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver/audio_driver.c -o CMakeFiles/audio_driver.dir/audio_driver.c.s

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.requires:

.PHONY : audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.requires

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.provides: audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.requires
	$(MAKE) -f audio_driver/CMakeFiles/audio_driver.dir/build.make audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.provides.build
.PHONY : audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.provides

audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.provides.build: audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o


# Object files for target audio_driver
audio_driver_OBJECTS = \
"CMakeFiles/audio_driver.dir/audio_driver.c.o"

# External object files for target audio_driver
audio_driver_EXTERNAL_OBJECTS =

audio_driver/libaudio_driver.a: audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o
audio_driver/libaudio_driver.a: audio_driver/CMakeFiles/audio_driver.dir/build.make
audio_driver/libaudio_driver.a: audio_driver/CMakeFiles/audio_driver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/ECEN_427/lab06/space_invaders/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libaudio_driver.a"
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && $(CMAKE_COMMAND) -P CMakeFiles/audio_driver.dir/cmake_clean_target.cmake
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/audio_driver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
audio_driver/CMakeFiles/audio_driver.dir/build: audio_driver/libaudio_driver.a

.PHONY : audio_driver/CMakeFiles/audio_driver.dir/build

audio_driver/CMakeFiles/audio_driver.dir/requires: audio_driver/CMakeFiles/audio_driver.dir/audio_driver.c.o.requires

.PHONY : audio_driver/CMakeFiles/audio_driver.dir/requires

audio_driver/CMakeFiles/audio_driver.dir/clean:
	cd /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver && $(CMAKE_COMMAND) -P CMakeFiles/audio_driver.dir/cmake_clean.cmake
.PHONY : audio_driver/CMakeFiles/audio_driver.dir/clean

audio_driver/CMakeFiles/audio_driver.dir/depend:
	cd /home/xilinx/ECEN_427/lab06/space_invaders && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/ECEN_427/lab06/space_invaders /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver /home/xilinx/ECEN_427/lab06/space_invaders /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver /home/xilinx/ECEN_427/lab06/space_invaders/audio_driver/CMakeFiles/audio_driver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : audio_driver/CMakeFiles/audio_driver.dir/depend

