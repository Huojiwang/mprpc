# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/xd/Public/program/mprpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xd/Public/program/mprpc/build

# Include any dependencies generated for this target.
include example/callee/CMakeFiles/friendprovider.dir/depend.make

# Include the progress variables for this target.
include example/callee/CMakeFiles/friendprovider.dir/progress.make

# Include the compile flags for this target's objects.
include example/callee/CMakeFiles/friendprovider.dir/flags.make

example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.o: example/callee/CMakeFiles/friendprovider.dir/flags.make
example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.o: ../example/callee/friendservice.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xd/Public/program/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.o"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/friendprovider.dir/friendservice.cc.o -c /home/xd/Public/program/mprpc/example/callee/friendservice.cc

example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/friendprovider.dir/friendservice.cc.i"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xd/Public/program/mprpc/example/callee/friendservice.cc > CMakeFiles/friendprovider.dir/friendservice.cc.i

example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/friendprovider.dir/friendservice.cc.s"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xd/Public/program/mprpc/example/callee/friendservice.cc -o CMakeFiles/friendprovider.dir/friendservice.cc.s

example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.o: example/callee/CMakeFiles/friendprovider.dir/flags.make
example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.o: ../example/friend.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xd/Public/program/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.o"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/friendprovider.dir/__/friend.pb.cc.o -c /home/xd/Public/program/mprpc/example/friend.pb.cc

example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/friendprovider.dir/__/friend.pb.cc.i"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xd/Public/program/mprpc/example/friend.pb.cc > CMakeFiles/friendprovider.dir/__/friend.pb.cc.i

example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/friendprovider.dir/__/friend.pb.cc.s"
	cd /home/xd/Public/program/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xd/Public/program/mprpc/example/friend.pb.cc -o CMakeFiles/friendprovider.dir/__/friend.pb.cc.s

# Object files for target friendprovider
friendprovider_OBJECTS = \
"CMakeFiles/friendprovider.dir/friendservice.cc.o" \
"CMakeFiles/friendprovider.dir/__/friend.pb.cc.o"

# External object files for target friendprovider
friendprovider_EXTERNAL_OBJECTS =

../bin/friendprovider: example/callee/CMakeFiles/friendprovider.dir/friendservice.cc.o
../bin/friendprovider: example/callee/CMakeFiles/friendprovider.dir/__/friend.pb.cc.o
../bin/friendprovider: example/callee/CMakeFiles/friendprovider.dir/build.make
../bin/friendprovider: ../lib/libmprpc.a
../bin/friendprovider: example/callee/CMakeFiles/friendprovider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xd/Public/program/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../bin/friendprovider"
	cd /home/xd/Public/program/mprpc/build/example/callee && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/friendprovider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/callee/CMakeFiles/friendprovider.dir/build: ../bin/friendprovider

.PHONY : example/callee/CMakeFiles/friendprovider.dir/build

example/callee/CMakeFiles/friendprovider.dir/clean:
	cd /home/xd/Public/program/mprpc/build/example/callee && $(CMAKE_COMMAND) -P CMakeFiles/friendprovider.dir/cmake_clean.cmake
.PHONY : example/callee/CMakeFiles/friendprovider.dir/clean

example/callee/CMakeFiles/friendprovider.dir/depend:
	cd /home/xd/Public/program/mprpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xd/Public/program/mprpc /home/xd/Public/program/mprpc/example/callee /home/xd/Public/program/mprpc/build /home/xd/Public/program/mprpc/build/example/callee /home/xd/Public/program/mprpc/build/example/callee/CMakeFiles/friendprovider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/callee/CMakeFiles/friendprovider.dir/depend

