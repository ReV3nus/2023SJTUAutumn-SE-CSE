add_test( SuperblockTest.CreateFromZero /home/stu/chfs/build/test/superblock_test [==[--gtest_filter=SuperblockTest.CreateFromZero]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/superblock_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( SuperblockTest.CreateFromZero PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( superblock_test_TESTS SuperblockTest.CreateFromZero)
