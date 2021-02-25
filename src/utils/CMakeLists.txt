cmake_minimum_required(VERSION 3.14.6)

set(LIB_NAME is_utils)

project(${LIB_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

# Sources
file(GLOB UTILS_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
)

if (WIN32)
    file(GLOB UTILS_WIN32_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}
        win32/*.cpp
    )
    list(APPEND UTILS_SOURCES ${UTILS_WIN32_SOURCES})
endif()


# is_utils library
add_library(${LIB_NAME} ${UTILS_SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# set_target_properties((${LIB_NAME} PROPERTIES
#     # something
# )