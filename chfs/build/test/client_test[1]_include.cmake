if(EXISTS "/home/stu/chfs/build/test/client_test[1]_tests.cmake")
  include("/home/stu/chfs/build/test/client_test[1]_tests.cmake")
else()
  add_test(client_test_NOT_BUILT client_test_NOT_BUILT)
endif()