if(EXISTS "/home/stu/chfs/build/test/bitmap_test[1]_tests.cmake")
  include("/home/stu/chfs/build/test/bitmap_test[1]_tests.cmake")
else()
  add_test(bitmap_test_NOT_BUILT bitmap_test_NOT_BUILT)
endif()