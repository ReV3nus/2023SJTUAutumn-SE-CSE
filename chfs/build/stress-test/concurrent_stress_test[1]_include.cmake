if(EXISTS "/home/stu/chfs/build/stress-test/concurrent_stress_test[1]_tests.cmake")
  include("/home/stu/chfs/build/stress-test/concurrent_stress_test[1]_tests.cmake")
else()
  add_test(concurrent_stress_test_NOT_BUILT concurrent_stress_test_NOT_BUILT)
endif()