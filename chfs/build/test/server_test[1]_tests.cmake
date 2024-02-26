add_test( LibRpcServerTest.Create /home/stu/chfs/build/test/server_test [==[--gtest_filter=LibRpcServerTest.Create]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/server_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( LibRpcServerTest.Create PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( LibRpcServerTest.Bind /home/stu/chfs/build/test/server_test [==[--gtest_filter=LibRpcServerTest.Bind]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/server_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( LibRpcServerTest.Bind PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( server_test_TESTS LibRpcServerTest.Create LibRpcServerTest.Bind)
