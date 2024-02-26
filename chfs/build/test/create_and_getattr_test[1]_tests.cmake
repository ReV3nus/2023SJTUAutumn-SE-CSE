add_test( FileSystemTest.CreateAndGetAttr /home/stu/chfs/build/test/create_and_getattr_test [==[--gtest_filter=FileSystemTest.CreateAndGetAttr]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/create_and_getattr_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( FileSystemTest.CreateAndGetAttr PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( create_and_getattr_test_TESTS FileSystemTest.CreateAndGetAttr)
