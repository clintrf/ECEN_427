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
CMAKE_SOURCE_DIR = /home/xilinx/ECEN_427/lab03/image_render

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xilinx/ECEN_427/lab03/image_render

# Include any dependencies generated for this target.
include CMakeFiles/render.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/render.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/render.dir/flags.make

CMakeFiles/render.dir/image_render.c.o: CMakeFiles/render.dir/flags.make
CMakeFiles/render.dir/image_render.c.o: image_render.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xilinx/ECEN_427/lab03/image_render/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/render.dir/image_render.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/render.dir/image_render.c.o   -c /home/xilinx/ECEN_427/lab03/image_render/image_render.c

CMakeFiles/render.dir/image_render.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/render.dir/image_render.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xilinx/ECEN_427/lab03/image_render/image_render.c > CMakeFiles/render.dir/image_render.c.i

CMakeFiles/render.dir/image_render.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/render.dir/image_render.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xilinx/ECEN_427/lab03/image_render/image_render.c -o CMakeFiles/render.dir/image_render.c.s

CMakeFiles/render.dir/image_render.c.o.requires:

.PHONY : CMakeFiles/render.dir/image_render.c.o.requires

CMakeFiles/render.dir/image_render.c.o.provides: CMakeFiles/render.dir/image_render.c.o.requires
	$(MAKE) -f CMakeFiles/render.dir/build.make CMakeFiles/render.dir/image_render.c.o.provides.build
.PHONY : CMakeFiles/render.dir/image_render.c.o.provides

CMakeFiles/render.dir/image_render.c.o.provides.build: CMakeFiles/render.dir/image_render.c.o


# Object files for target render
render_OBJECTS = \
"CMakeFiles/render.dir/image_render.c.o"

# External object files for target render
render_EXTERNAL_OBJECTS =

librender.a: CMakeFiles/render.dir/image_render.c.o
librender.a: CMakeFiles/render.dir/build.make
librender.a: CMakeFiles/render.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xilinx/ECEN_427/lab03/image_render/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library librender.a"
	$(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/render.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/render.dir/build: librender.a

.PHONY : CMakeFiles/render.dir/build

CMakeFiles/render.dir/requires: CMakeFiles/render.dir/image_render.c.o.requires

.PHONY : CMakeFiles/render.dir/requires

CMakeFiles/render.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean.cmake
.PHONY : CMakeFiles/render.dir/clean

CMakeFiles/render.dir/depend:
	cd /home/xilinx/ECEN_427/lab03/image_render && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xilinx/ECEN_427/lab03/image_render /home/xilinx/ECEN_427/lab03/image_render /home/xilinx/ECEN_427/lab03/image_render /home/xilinx/ECEN_427/lab03/image_render /home/xilinx/ECEN_427/lab03/image_render/CMakeFiles/render.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/render.dir/depend

