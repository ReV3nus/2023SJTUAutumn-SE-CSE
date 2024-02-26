add_test( BasicTest.Hello /home/stu/chfs/build/test/hello_test [==[--gtest_filter=BasicTest.Hello]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/hello_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( BasicTest.Hello PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( hello_test_TESTS BasicTest.Hello)
