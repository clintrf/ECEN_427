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
CMAKE_SOURCE_DIR = /home/xilinx/ECEN_427/lab03

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xilinx/ECEN_427/lab03

# Include any dependencies generated for this target.
include i2cps/CMakeFiles/i2cps.dir/depend.make

# Include the progress variables for this target.
include i2cps/CMakeFiles/i2cps.dir/progress.make

# Include the compile flags for this target's objects.
include i2cps/CMakeFiles/i2cps.dir/flags.make

i2cps/CMakeFiles/i2cps.dir/i2cps.c.o: i2cps/CMakeFiles/i2cps.dir/flags.make
i2cps/CMakeFiles/i2cps.dir/i2cps.c.o: i2cps/i2cps.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/ECEN_427/lab03/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object i2cps/CMakeFiles/i2cps.dir/i2cps.c.o"
	cd /home/xilinx/ECEN_427/lab03/i2cps && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/i2cps.dir/i2cps.c.o   -c /home/xilinx/ECEN_427/lab03/i2cps/i2cps.c

i2cps/CMakeFiles/i2cps.dir/i2cps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2cps.dir/i2cps.c.i"
	cd /home/xilinx/ECEN_427/lab03/i2cps && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/ECEN_427/lab03/i2cps/i2cps.c > CMakeFiles/i2cps.dir/i2cps.c.i

i2cps/CMakeFiles/i2cps.dir/i2cps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2cps.dir/i2cps.c.s"
	cd /home/xilinx/ECEN_427/lab03/i2cps && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/ECEN_427/lab03/i2cps/i2cps.c -o CMakeFiles/i2cps.dir/i2cps.c.s

i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.requires:

.PHONY : i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.requires

i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.provides: i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.requires
	$(MAKE) -f i2cps/CMakeFiles/i2cps.dir/build.make i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.provides.build
.PHONY : i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.provides

i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.provides.build: i2cps/CMakeFiles/i2cps.dir/i2cps.c.o


# Object files for target i2cps
i2cps_OBJECTS = \
"CMakeFiles/i2cps.dir/i2cps.c.o"

# External object files for target i2cps
i2cps_EXTERNAL_OBJECTS =

i2cps/libi2cps.a: i2cps/CMakeFiles/i2cps.dir/i2cps.c.o
i2cps/libi2cps.a: i2cps/CMakeFiles/i2cps.dir/build.make
i2cps/libi2cps.a: i2cps/CMakeFiles/i2cps.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/ECEN_427/lab03/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libi2cps.a"
	cd /home/xilinx/ECEN_427/lab03/i2cps && $(CMAKE_COMMAND) -P CMakeFiles/i2cps.dir/cmake_clean_target.cmake
	cd /home/xilinx/ECEN_427/lab03/i2cps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/i2cps.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
i2cps/CMakeFiles/i2cps.dir/build: i2cps/libi2cps.a

.PHONY : i2cps/CMakeFiles/i2cps.dir/build

i2cps/CMakeFiles/i2cps.dir/requires: i2cps/CMakeFiles/i2cps.dir/i2cps.c.o.requires

.PHONY : i2cps/CMakeFiles/i2cps.dir/requires

i2cps/CMakeFiles/i2cps.dir/clean:
	cd /home/xilinx/ECEN_427/lab03/i2cps && $(CMAKE_COMMAND) -P CMakeFiles/i2cps.dir/cmake_clean.cmake
.PHONY : i2cps/CMakeFiles/i2cps.dir/clean

i2cps/CMakeFiles/i2cps.dir/depend:
	cd /home/xilinx/ECEN_427/lab03 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/ECEN_427/lab03 /home/xilinx/ECEN_427/lab03/i2cps /home/xilinx/ECEN_427/lab03 /home/xilinx/ECEN_427/lab03/i2cps /home/xilinx/ECEN_427/lab03/i2cps/CMakeFiles/i2cps.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : i2cps/CMakeFiles/i2cps.dir/depend

