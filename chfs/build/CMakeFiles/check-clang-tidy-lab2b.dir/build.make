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

# Utility rule file for check-clang-tidy-lab2b.

# Include any custom commands dependencies for this target.
include CMakeFiles/check-clang-tidy-lab2b.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/check-clang-tidy-lab2b.dir/progress.make

CMakeFiles/check-clang-tidy-lab2b:
	../build_support/run_clang_tidy.py -clang-tidy-binary /usr/bin/clang-tidy-14 -p /home/stu/chfs/build -checks=-*,clang-diagnostic-*,-clang-diagnostic-unused-value,clang-analyzer-*,-*,bugprone-*,performance-*,readability-*,-readability-magic-numbers,-readability-braces-around-statements,-readability-inconsistent-declaration-parameter-name,-readability-named-parameter /home/stu/chfs/src/include/distributed/*.h /home/stu/chfs/src/distributed/*.cc /home/stu/chfs/src/include/librpc/*.h /home/stu/chfs/src/librpc/*.cc /home/stu/chfs/src/include/common/*.h /home/stu/chfs/src/include/block/*.h /home/stu/chfs/src/block/*.cc /home/stu/chfs/src/include/metadata/*.h /home/stu/chfs/src/include/metadata/*.cc

check-clang-tidy-lab2b: CMakeFiles/check-clang-tidy-lab2b
check-clang-tidy-lab2b: CMakeFiles/check-clang-tidy-lab2b.dir/build.make
.PHONY : check-clang-tidy-lab2b

# Rule to build all files generated by this target.
CMakeFiles/check-clang-tidy-lab2b.dir/build: check-clang-tidy-lab2b
.PHONY : CMakeFiles/check-clang-tidy-lab2b.dir/build

CMakeFiles/check-clang-tidy-lab2b.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check-clang-tidy-lab2b.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check-clang-tidy-lab2b.dir/clean

CMakeFiles/check-clang-tidy-lab2b.dir/depend:
	cd /home/stu/chfs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stu/chfs /home/stu/chfs /home/stu/chfs/build /home/stu/chfs/build /home/stu/chfs/build/CMakeFiles/check-clang-tidy-lab2b.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check-clang-tidy-lab2b.dir/depend
