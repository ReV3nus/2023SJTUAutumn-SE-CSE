if(EXISTS "/home/stu/chfs/build/test/server_test[1]_tests.cmake")
  include("/home/stu/chfs/build/test/server_test[1]_tests.cmake")
else()
  add_test(server_test_NOT_BUILT server_test_NOT_BUILT)
endif()
