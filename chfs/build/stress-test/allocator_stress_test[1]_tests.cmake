add_test( BlockAllocatorTest.StressTest1 /home/stu/chfs/build/stress-test/allocator_stress_test [==[--gtest_filter=BlockAllocatorTest.StressTest1]==] --gtest_also_run_disabled_tests)
set_tests_properties( BlockAllocatorTest.StressTest1 PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/stress-test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( allocator_stress_test_TESTS BlockAllocatorTest.StressTest1)
