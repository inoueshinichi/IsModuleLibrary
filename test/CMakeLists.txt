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

# Tutorial GoogleTest
add_executable(test_googletest
    tutorial_googletest/test_gtest.cpp
    tutorial_googletest/deal.hpp
    tutorial_googletest/deal.cpp
    tutorial_googletest/account.hpp
    tutorial_googletest/mock_account.hpp
)

target_link_libraries(test_googletest
    GTest::gtest
    GTest::gmock
)

add_dependencies(test_googletest GTest::gtest GTest::gmock)

# # Test: nnabla
# add_executable(test_nnabla
#     test_nnabla.cpp
# )
# target_link_libraries(test_Nnabla PRIVATE
#     nnabla
# )

# # Test: imgproc
# add_executable(test_imgproc
#     test_imgproc.cpp
# )
# target_link_libraries(test_imgproc PRIVATE
#     nnabla
#     imgproc
# )


# # dllファイルを実行ファイルと同じフォルダにコピー
# add_custom_command(TARGET test_Imgproc POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_nnabla> $<TARGET_FILE_DIR:test_Imgproc>
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_imgproc> $<TARGET_FILE_DIR:test_Imgproc>
# )