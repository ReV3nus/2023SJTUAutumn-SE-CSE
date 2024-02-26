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
CMAKE_SOURCE_DIR = /home/stu/chfs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stu/chfs/build

# Include any dependencies generated for this target.
include src/metadata/CMakeFiles/chfs_metadata.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/metadata/CMakeFiles/chfs_metadata.dir/compiler_depend.make

# Include the progress variables for this target.
include src/metadata/CMakeFiles/chfs_metadata.dir/progress.make

# Include the compile flags for this target's objects.
include src/metadata/CMakeFiles/chfs_metadata.dir/flags.make

src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/flags.make
src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o: ../src/metadata/superblock.cc
src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stu/chfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o -MF CMakeFiles/chfs_metadata.dir/superblock.cc.o.d -o CMakeFiles/chfs_metadata.dir/superblock.cc.o -c /home/stu/chfs/src/metadata/superblock.cc

src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chfs_metadata.dir/superblock.cc.i"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stu/chfs/src/metadata/superblock.cc > CMakeFiles/chfs_metadata.dir/superblock.cc.i

src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chfs_metadata.dir/superblock.cc.s"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stu/chfs/src/metadata/superblock.cc -o CMakeFiles/chfs_metadata.dir/superblock.cc.s

src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/flags.make
src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o: ../src/metadata/manager.cc
src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stu/chfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o -MF CMakeFiles/chfs_metadata.dir/manager.cc.o.d -o CMakeFiles/chfs_metadata.dir/manager.cc.o -c /home/stu/chfs/src/metadata/manager.cc

src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chfs_metadata.dir/manager.cc.i"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stu/chfs/src/metadata/manager.cc > CMakeFiles/chfs_metadata.dir/manager.cc.i

src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chfs_metadata.dir/manager.cc.s"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stu/chfs/src/metadata/manager.cc -o CMakeFiles/chfs_metadata.dir/manager.cc.s

src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/flags.make
src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o: ../src/metadata/inode.cc
src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o: src/metadata/CMakeFiles/chfs_metadata.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stu/chfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o -MF CMakeFiles/chfs_metadata.dir/inode.cc.o.d -o CMakeFiles/chfs_metadata.dir/inode.cc.o -c /home/stu/chfs/src/metadata/inode.cc

src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chfs_metadata.dir/inode.cc.i"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stu/chfs/src/metadata/inode.cc > CMakeFiles/chfs_metadata.dir/inode.cc.i

src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chfs_metadata.dir/inode.cc.s"
	cd /home/stu/chfs/build/src/metadata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stu/chfs/src/metadata/inode.cc -o CMakeFiles/chfs_metadata.dir/inode.cc.s

chfs_metadata: src/metadata/CMakeFiles/chfs_metadata.dir/superblock.cc.o
chfs_metadata: src/metadata/CMakeFiles/chfs_metadata.dir/manager.cc.o
chfs_metadata: src/metadata/CMakeFiles/chfs_metadata.dir/inode.cc.o
chfs_metadata: src/metadata/CMakeFiles/chfs_metadata.dir/build.make
.PHONY : chfs_metadata

# Rule to build all files generated by this target.
src/metadata/CMakeFiles/chfs_metadata.dir/build: chfs_metadata
.PHONY : src/metadata/CMakeFiles/chfs_metadata.dir/build

src/metadata/CMakeFiles/chfs_metadata.dir/clean:
	cd /home/stu/chfs/build/src/metadata && $(CMAKE_COMMAND) -P CMakeFiles/chfs_metadata.dir/cmake_clean.cmake
.PHONY : src/metadata/CMakeFiles/chfs_metadata.dir/clean

src/metadata/CMakeFiles/chfs_metadata.dir/depend:
	cd /home/stu/chfs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stu/chfs /home/stu/chfs/src/metadata /home/stu/chfs/build /home/stu/chfs/build/src/metadata /home/stu/chfs/build/src/metadata/CMakeFiles/chfs_metadata.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/metadata/CMakeFiles/chfs_metadata.dir/depend
