# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/zip/Desktop/server/tasks/task-102

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/zip/Desktop/server/tasks/task-102/build

# Include any dependencies generated for this target.
include proto/CMakeFiles/proto.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include proto/CMakeFiles/proto.dir/compiler_depend.make

# Include the progress variables for this target.
include proto/CMakeFiles/proto.dir/progress.make

# Include the compile flags for this target's objects.
include proto/CMakeFiles/proto.dir/flags.make

proto/message.pb.h: ../proto/message.proto
proto/message.pb.h: /usr/local/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/zip/Desktop/server/tasks/task-102/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on message.proto"
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && /usr/local/bin/protoc --cpp_out /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto -I /mnt/c/Users/zip/Desktop/server/tasks/task-102/proto /mnt/c/Users/zip/Desktop/server/tasks/task-102/proto/message.proto

proto/message.pb.cc: proto/message.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate proto/message.pb.cc

proto/CMakeFiles/proto.dir/message.pb.cc.o: proto/CMakeFiles/proto.dir/flags.make
proto/CMakeFiles/proto.dir/message.pb.cc.o: proto/message.pb.cc
proto/CMakeFiles/proto.dir/message.pb.cc.o: proto/CMakeFiles/proto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/zip/Desktop/server/tasks/task-102/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object proto/CMakeFiles/proto.dir/message.pb.cc.o"
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT proto/CMakeFiles/proto.dir/message.pb.cc.o -MF CMakeFiles/proto.dir/message.pb.cc.o.d -o CMakeFiles/proto.dir/message.pb.cc.o -c /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto/message.pb.cc

proto/CMakeFiles/proto.dir/message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proto.dir/message.pb.cc.i"
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto/message.pb.cc > CMakeFiles/proto.dir/message.pb.cc.i

proto/CMakeFiles/proto.dir/message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proto.dir/message.pb.cc.s"
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto/message.pb.cc -o CMakeFiles/proto.dir/message.pb.cc.s

# Object files for target proto
proto_OBJECTS = \
"CMakeFiles/proto.dir/message.pb.cc.o"

# External object files for target proto
proto_EXTERNAL_OBJECTS =

proto/libproto.a: proto/CMakeFiles/proto.dir/message.pb.cc.o
proto/libproto.a: proto/CMakeFiles/proto.dir/build.make
proto/libproto.a: proto/CMakeFiles/proto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/zip/Desktop/server/tasks/task-102/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libproto.a"
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && $(CMAKE_COMMAND) -P CMakeFiles/proto.dir/cmake_clean_target.cmake
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
proto/CMakeFiles/proto.dir/build: proto/libproto.a
.PHONY : proto/CMakeFiles/proto.dir/build

proto/CMakeFiles/proto.dir/clean:
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto && $(CMAKE_COMMAND) -P CMakeFiles/proto.dir/cmake_clean.cmake
.PHONY : proto/CMakeFiles/proto.dir/clean

proto/CMakeFiles/proto.dir/depend: proto/message.pb.cc
proto/CMakeFiles/proto.dir/depend: proto/message.pb.h
	cd /mnt/c/Users/zip/Desktop/server/tasks/task-102/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/zip/Desktop/server/tasks/task-102 /mnt/c/Users/zip/Desktop/server/tasks/task-102/proto /mnt/c/Users/zip/Desktop/server/tasks/task-102/build /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto /mnt/c/Users/zip/Desktop/server/tasks/task-102/build/proto/CMakeFiles/proto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : proto/CMakeFiles/proto.dir/depend

