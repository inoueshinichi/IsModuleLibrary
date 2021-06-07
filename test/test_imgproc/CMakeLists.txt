cmake_minimum_required(VERSION 3.14.6)

set(TEST_TARGET_EXE "test_imgproc")

find_package(Python3 COMPONENTS Development NumPy)

add_executable(${TEST_TARGET_EXE}
    test_imgproc.cpp
)

target_include_directories(${TEST_TARGET_EXE} PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
)

target_link_libraries(${TEST_TARGET_EXE} PRIVATE
    nnabla
    imgproc
    GTest::gtest
    GTest::gmock
    Python3::Python
    Python3::NumPy
)

add_dependencies(${TEST_TARGET_EXE} 
    nnabla
    imgproc
    GTest::gtest 
    GTest::gmock
)


# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET ${TEST_TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:nnabla> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:imgproc> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
)