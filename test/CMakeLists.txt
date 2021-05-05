cmake_minimum_required(VERSION 3.14.6)

# GoogleTest
include(${CMAKE_SOURCE_DIR}/build-tool/cmake/googletest.cmake)
BuildGoogleTest("GoogleTest" ON ON)

# Test: gtest
add_executable(test_gtest
    test_gtest.cpp
)
target_link_libraries(test_gtest PRIVATE
    GTest::gtest
)
# add_dependencies(test_gtest GTest::gtest)

# # Test: nnabla
# add_executable(test_Nnabla
#     test_Nnabla.cpp
# )
# target_link_libraries(test_Nnabla PRIVATE
#     is_nnabla
# )

# # Test: imgproc
# add_executable(test_Imgproc
#     test_Imgproc.cpp
# )
# target_link_libraries(test_Imgproc PRIVATE
#     is_nnabla
#     is_imgproc
# )


# # dllファイルを実行ファイルと同じフォルダにコピー
# add_custom_command(TARGET test_Imgproc POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_nnabla> $<TARGET_FILE_DIR:test_Imgproc>
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_imgproc> $<TARGET_FILE_DIR:test_Imgproc>
# )