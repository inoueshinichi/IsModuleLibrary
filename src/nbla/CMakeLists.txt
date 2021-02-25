cmake_minimum_required(VERSION 3.14.6)

set(LIB_NAME is_nnabla)

file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    memory/*.cpp
    array/*.cpp
)


# is_nnabla libarary
add_library(${LIB_NAME} ${SOURCES})

# nnabla_EXPORTS or nnabla_dbg_EXPORTS 定義済みマクロを設定 for windows *.dll
target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:${LIB_NAME}_dbg_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:${LIB_NAME}_dbg_EXPORTS>
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)



