add_test( BasicFileSystemTest.Init /home/stu/chfs/build/test/basic_fs_test [==[--gtest_filter=BasicFileSystemTest.Init]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/basic_fs_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( BasicFileSystemTest.Init PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( basic_fs_test_TESTS BasicFileSystemTest.Init)
