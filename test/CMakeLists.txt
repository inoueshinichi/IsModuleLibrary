cmake_minimum_required(VERSION 3.14.6)

# GoogleTest
include(${CMAKE_SOURCE_DIR}/build-tool/cmake/googletest.cmake)
BuildGoogleTest("GoogleTest" ON ON)

if((NOT TARGET GTest::gtest)      OR 
   (NOT TARGET GTest::gmock)      OR
   (NOT TARGET GTest::gtest_main) OR
   (NOT TARGET GTest::gmock_main)  )
   message(STATUS "Return CMakeLists.txt of test because GoogleTest is not built.")
   return()
endif()


add_subdirectory(tutorial_googletest)
add_subdirectory(test_nnabla)
add_subdirectory(test_imgproc)











