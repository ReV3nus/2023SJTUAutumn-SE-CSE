add_test( FileSystemTest.Remove /home/stu/chfs/build/test/remove_test [==[--gtest_filter=FileSystemTest.Remove]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/remove_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( FileSystemTest.Remove PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( remove_test_TESTS FileSystemTest.Remove)
