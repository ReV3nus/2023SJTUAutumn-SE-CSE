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

# Utility rule file for clean-fs.

# Include any custom commands dependencies for this target.
include CMakeFiles/clean-fs.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clean-fs.dir/progress.make

CMakeFiles/clean-fs:
	rm -rf /tmp/inode_* /tmp/data_block_* /tmp/meta_block* /tmp/test_* /tmp/chfs.log

clean-fs: CMakeFiles/clean-fs
clean-fs: CMakeFiles/clean-fs.dir/build.make
.PHONY : clean-fs

# Rule to build all files generated by this target.
CMakeFiles/clean-fs.dir/build: clean-fs
.PHONY : CMakeFiles/clean-fs.dir/build

CMakeFiles/clean-fs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clean-fs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clean-fs.dir/clean

CMakeFiles/clean-fs.dir/depend:
	cd /home/stu/chfs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stu/chfs /home/stu/chfs /home/stu/chfs/build /home/stu/chfs/build /home/stu/chfs/build/CMakeFiles/clean-fs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clean-fs.dir/depend
