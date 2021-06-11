cmake_minimum_required(VERSION 3.14.6)

set(LIB_NAME "nnabla")
project(${LIB_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

file(GLOB NBLA_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    memory/*.cpp
    array/*.cpp
    function/generic/*.cpp
)

# is_nnabla libarary
add_library(${LIB_NAME} ${NBLA_SOURCES})

if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${LIB_NAME} PROPERTIES PREFIX "lib")
    set_target_properties(${LIB_NAME} PROPERTIES IMPORT_PREFIX "lib")
endif()

# is_nnabla_EXPORTS or is_nnabla_dbg_EXPORTS 定義済みマクロを設定 for windows *.dll
target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:${LIB_NAME}_dbg_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:${LIB_NAME}_dbg_EXPORTS>
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third-party/eigen-3.3.7
)



