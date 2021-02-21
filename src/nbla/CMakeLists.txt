cmake_minimum_required(VERSION 3.14.6)

set(LIB_NAME nnabla)

file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    memory/*.cpp
    array/*.cpp
)

# # add version info into DLL
# if (WIN32)
#     configure_file(
#         ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
#         ${CMAKE_CURRENT_BINARY_DIR}/version-utf8.rc
#     )

#     execute_process(
#         COMMAND ${PYTHON_COMMAND}
#         ${CMAKE_SOURCE_DIR}/build-tools/cmake/utf8_to_utf16.py
#         ${CMAKE_CURRENT_BINARY_DIR}/version-utf8.rc ${CMAKE_CURRENT_BINARY_DIR}/version.rc
#     )

#     list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
# endif()


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



