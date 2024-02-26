add_test( MetadataConcurrentStressTest.StressTest1 /home/stu/chfs/build/bin/concurrent_stress_test [==[--gtest_filter=MetadataConcurrentStressTest.StressTest1]==] --gtest_also_run_disabled_tests)
set_tests_properties( MetadataConcurrentStressTest.StressTest1 PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/stress-test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( concurrent_stress_test_TESTS MetadataConcurrentStressTest.StressTest1)
