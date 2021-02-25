cmake_minimum_required(VERSION 3.14.6)

add_executable(test_Nnabla
    test_Nnabla.cpp
)

target_link_libraries(test_Nnabla PRIVATE
    nnabla
)