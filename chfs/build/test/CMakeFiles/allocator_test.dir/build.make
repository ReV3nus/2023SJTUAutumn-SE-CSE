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
include test/CMakeFiles/allocator_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/allocator_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/allocator_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/allocator_test.dir/flags.make

test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o: test/CMakeFiles/allocator_test.dir/flags.make
test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o: ../test/block/allocator_test.cc
test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o: test/CMakeFiles/allocator_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stu/chfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o"
	cd /home/stu/chfs/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o -MF CMakeFiles/allocator_test.dir/block/allocator_test.cc.o.d -o CMakeFiles/allocator_test.dir/block/allocator_test.cc.o -c /home/stu/chfs/test/block/allocator_test.cc

test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/allocator_test.dir/block/allocator_test.cc.i"
	cd /home/stu/chfs/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stu/chfs/test/block/allocator_test.cc > CMakeFiles/allocator_test.dir/block/allocator_test.cc.i

test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/allocator_test.dir/block/allocator_test.cc.s"
	cd /home/stu/chfs/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stu/chfs/test/block/allocator_test.cc -o CMakeFiles/allocator_test.dir/block/allocator_test.cc.s

# Object files for target allocator_test
allocator_test_OBJECTS = \
"CMakeFiles/allocator_test.dir/block/allocator_test.cc.o"

# External object files for target allocator_test
allocator_test_EXTERNAL_OBJECTS =

test/allocator_test: test/CMakeFiles/allocator_test.dir/block/allocator_test.cc.o
test/allocator_test: test/CMakeFiles/allocator_test.dir/build.make
test/allocator_test: lib/libchfs.a
test/allocator_test: lib/libgtest.a
test/allocator_test: lib/libgmock_main.a
test/allocator_test: lib/libfmtd.a
test/allocator_test: lib/librpc.a
test/allocator_test: lib/libgmock.a
test/allocator_test: lib/libgtest.a
test/allocator_test: test/CMakeFiles/allocator_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stu/chfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable allocator_test"
	cd /home/stu/chfs/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/allocator_test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/stu/chfs/build/test && /usr/bin/cmake -D TEST_TARGET=allocator_test -D TEST_EXECUTABLE=/home/stu/chfs/build/test/allocator_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/stu/chfs/build/test -D "TEST_EXTRA_ARGS=--gtest_color=auto;--gtest_output=xml:/home/stu/chfs/build/test/allocator_test.xml;--gtest_catch_exceptions=0" -D "TEST_PROPERTIES=TIMEOUT;1000" -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=allocator_test_TESTS -D CTEST_FILE=/home/stu/chfs/build/test/allocator_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=120 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.22/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/allocator_test.dir/build: test/allocator_test
.PHONY : test/CMakeFiles/allocator_test.dir/build

test/CMakeFiles/allocator_test.dir/clean:
	cd /home/stu/chfs/build/test && $(CMAKE_COMMAND) -P CMakeFiles/allocator_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/allocator_test.dir/clean

test/CMakeFiles/allocator_test.dir/depend:
	cd /home/stu/chfs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stu/chfs /home/stu/chfs/test /home/stu/chfs/build /home/stu/chfs/build/test /home/stu/chfs/build/test/CMakeFiles/allocator_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/allocator_test.dir/depend

