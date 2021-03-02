cmake_minimum_required(VERSION 3.14.6)

# Test: nnabla
add_executable(test_Nnabla
    test_Nnabla.cpp
)
target_link_libraries(test_Nnabla PRIVATE
    is_nnabla
)

# Test: imgproc
add_executable(test_Imgproc
    test_Imgproc.cpp
)
target_link_libraries(test_Imgproc PRIVATE
    is_nnabla
    is_imgproc
)


# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET test_Imgproc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_nnabla> $<TARGET_FILE_DIR:test_Imgproc>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:is_imgproc> $<TARGET_FILE_DIR:test_Imgproc>
)