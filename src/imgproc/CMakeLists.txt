cmake_minimum_required(VERSION 3.14.6)

set(LIB_NAME "imgproc")
project(${LIB_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

# Sources
file(GLOB IMGPROC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    ./io/*.cpp
    ./filter/*.cpp
)

if (WIN32)
    file(GLOB IMGPROC_WIN32_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        win32/*.cpp
    )
    list(APPEND IMGPROC_SOURCES ${IMGPROC_WIN32_SOURCES})
endif()

# imgproc library
add_library(${LIB_NAME} ${IMGPROC_SOURCES})

if(MSVC)
    # msvc does not append 'lib' - do it here to have consistent name
    set_target_properties(${LIB_NAME} PROPERTIES PREFIX "lib")
    set_target_properties(${LIB_NAME} PROPERTIES IMPORT_PREFIX "lib")
endif()

# imgproc_EXPORTS or imgproc_dbg_EXPORTS 定義済みマクロを設定 for windows *.dll
target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:${LIB_NAME}_dbg_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:${LIB_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:${LIB_NAME}_dbg_EXPORTS>
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${LIB_NAME} PUBLIC
    nnabla
    # utils
    # libpng
)

add_dependencies(${LIB_NAME} 
    nnabla
    # utils
    # libpng
)

# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET ${LIB_NAME} POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${LIB_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:libpng> $<TARGET_FILE_DIR:${LIB_NAME}>
)

# set_target_properties((${LIB_NAME} PROPERTIES
#     # something
# )