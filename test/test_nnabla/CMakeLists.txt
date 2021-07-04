cmake_minimum_required(VERSION 3.14.6)

set(TEST_TARGET_EXE "test_nnabla_core")

add_executable(${TEST_TARGET_EXE}
    test_nnabla_core.cpp
    ../test_utils.hpp
)
target_link_libraries(${TEST_TARGET_EXE} PRIVATE
    nnabla
    GTest::gtest
    GTest::gmock
)

add_dependencies(${TEST_TARGET_EXE} 
    GTest::gtest 
    GTest::gmock
)

# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET ${TEST_TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:nnabla> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
)



set(TEST_TARGET_EXE "test_nnabla_func")

add_executable(${TEST_TARGET_EXE}
    test_nnabla_func.cpp
    ../test_utils.hpp
)
target_link_libraries(${TEST_TARGET_EXE} PRIVATE
    nnabla
    GTest::gtest
    GTest::gmock
)

add_dependencies(${TEST_TARGET_EXE} 
    GTest::gtest 
    GTest::gmock
)

# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET ${TEST_TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:nnabla> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
)