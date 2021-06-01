# ダウンロードと解凍を行う関数
function(download_and_extract_library NAME EXT URL)
    if(NOT IS_DIRECTORY ${CMAKE_SOURCE_DIR}/third-party/${NAME})
        message(STATUS "Downloading ${NAME}")
        file(DOWNLOAD
            ${URL}
            ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}${EXT}
            SHOW_PROGRESS
            STATUS status
            LOG log)
        list(GET status 0 status_code)
        list(GET status 1 status_string)
        if(NOT status_code EQUAL 0)
            if(UNIX OR APPLE)
                message("Failed to download ${NAME}. Try to download with wget.")
                execute_process(
                    COMMAND wget ${URL} -O ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}${EXT}
                )
            else()
                message(FATAL_ERROR "Failed to download ${NAME}. Please check network connection and proxy settings.
                status_code: ${status_code}
                status_string: ${status_string}
                log: ${log}
                ")
                error() # Place Unknown command to stop cmake process here.
            endif()
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E 
            chdir ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_COMMAND} -E tar xf ${NAME}${EXT}
        )
    endif()
endfunction()


###############################################################################
# 以下に追加したいサードパーティ製のライブラリを明記
###############################################################################
# eigen3.3.7
download_and_extract_library(eigen-3.3.7 .zip https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.zip)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen-3.3.7/Eigen)


###############################################################################
# 以下に追加したいサードパーティ製のライブラリを明記 by External Project
###############################################################################
include(ExternalProject)

# zlib ------------------------------------------------------------------------
# http://www.zlib.net/
# https://github.com/madler/zlib
message(STATUS "Download and extract library: zlib-1.2.11")

# Setting zlib Project
set(ZLIB_TARGET zlib)
set(ZLIB_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${ZLIB_TARGET})
set(ZLIB_INSTALL ${ZLIB_PREFIX}/install)

ExternalProject_Add(
    # Taget Name
    ${ZLIB_TARGET}

    #################### No.0 Common Directory Path ####################
    # 一時ファイルを保存するディレクトリパス
    TMP_DIR ${ZLIB_PREFIX}/tmp
    # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
    STAMP_DIR ${ZLIB_PREFIX}/stamp
    # 各ステップでのLogファイルを保存するディレクトリパス
    LOG_DIR ${ZLIB_PREFIX}/log
    # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
    DOWNLOAD_DIR ${ZLIB_PREFIX}/archive
    # (解凍後の)ダウンロード内容を保存するディレクトリパス
    SOURCE_DIR ${ZLIB_PREFIX}/src
    # ビルド後の成果物を保存するディレクトリパス
    BINARY_DIR ${ZLIB_PREFIX}/build
    # ビルド成果物のインストール先のディレクトリパス
    # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
    INSTALL_DIR ${ZLIB_PREFIX} #${GTEST_PREFIX}/install 

     #################### No.1 Download Step ####################
    # 原因不明で前回, git cloneを行っていないのに、すでにgit cloneされていると判断されて、git cloneが実行されない.
    # # Git Repository URL
    # GIT_REPOSITORY https://github.com/google/googletest.git
    # # Git branch-name, tag or commit's hash id
    # GIT_TAG release-1.10.0
    # # Git Remote Name (default: origin)
    # GIT_REMOTE_NAME origin
    # # Git Remote Clone Progress
    # GIT_REMOTE_PROGRESS ON

    # URL Download
    URL https://github.com/madler/zlib/archive/refs/tags/v1.2.11.zip
    # Timeout
    TIMEOUT 10 # 10秒

    #################### No.2 Update/Patch Step ####################
    # ダウンロードした内容にパッチを当てるコマンド
    # UPDATE_COMMAND ""

    #################### No.3 Configure Step ####################
    # ジェネレータ
    # CMAKE_GENERATOR ""
    # cmake コマンドに渡す引数
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_INSTALL} 
               -DBUILD_SHARED_LIBS=OFF
               -DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DCMAKE_C_FLAGS=${GL_BINDING_C_FLAGS}
    
    #################### No.4 Build Step ####################
    # ビルドコマンド
    #BUILD_COMMAND ""
    # Build In Source or Build Out Source
    BUILD_IN_SOURCE OFF
    # Build Always or Not
    BUILD_ALWAYS OFF

    #################### No.5 Install Step ####################
    # インストールコマンド
    #INSTALL_COMMAND ""
    #################### No.6 Test Step ####################
    #################### No.7 Output Logging ####################
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_PATCH ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_TEST ON
    LOG_MERGED_STDOUTERR ON
    LOG_OUTPUT_ON_FAILURE ON
    #################### No.8 Terminal Access ####################
    #################### No.9 Target ####################
    #################### No.10 Miscellaneous ####################
)

ExternalProject_Get_Property(${ZLIB_TARGET} INSTALL_DIR)
message(STATUS "zlib's installed dir: ${INSTALL_DIR}")

include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

# # libpng ----------------------------------------------------------
# # http://www.libpng.org/pub/png/libpng.html
# # https://github.com/glennrp/libpng
# message(STATUS "Download and extract library: libpng-1.6.35")
# # download_and_extract_library(libpng-1.6.35 .zip https://github.com/glennrp/libpng/archive/refs/tags/v1.6.35.zip)

# # Setting libpng Project
# set(LIBPNG_TARGET libpng)
# set(LIBPNG_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${LIBPNG_TARGET})
# set(LIBPNG_INSTALL ${LIBPNG_PREFIX}/install)

# ExternalProject_Add(
#     # Taget Name
#     ${LIBPNG_TARGET}

#     #################### No.0 Common Directory Path ####################
#     # 一時ファイルを保存するディレクトリパス
#     TMP_DIR ${LIBPNG_PREFIX}/tmp
#     # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
#     STAMP_DIR ${LIBPNG_PREFIX}/stamp
#     # 各ステップでのLogファイルを保存するディレクトリパス
#     LOG_DIR ${LIBPNG_PREFIX}/log
#     # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
#     DOWNLOAD_DIR ${LIBPNG_PREFIX}/archive
#     # (解凍後の)ダウンロード内容を保存するディレクトリパス
#     SOURCE_DIR ${LIBPNG_PREFIX}/src
#     # ビルド後の成果物を保存するディレクトリパス
#     BINARY_DIR ${LIBPNG_PREFIX}/build
#     # ビルド成果物のインストール先のディレクトリパス
#     # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
#     INSTALL_DIR ${LIBPNG_PREFIX} #${GTEST_PREFIX}/install 

#      #################### No.1 Download Step ####################
#     # 原因不明で前回, git cloneを行っていないのに、すでにgit cloneされていると判断されて、git cloneが実行されない.
#     # # Git Repository URL
#     # GIT_REPOSITORY https://github.com/google/googletest.git
#     # # Git branch-name, tag or commit's hash id
#     # GIT_TAG release-1.10.0
#     # # Git Remote Name (default: origin)
#     # GIT_REMOTE_NAME origin
#     # # Git Remote Clone Progress
#     # GIT_REMOTE_PROGRESS ON

#     # URL Download
#     URL https://github.com/glennrp/libpng/archive/refs/tags/v1.6.35.zip
#     # Timeout
#     TIMEOUT 10 # 10秒

#     #################### No.2 Update/Patch Step ####################
#     # ダウンロードした内容にパッチを当てるコマンド
#     # UPDATE_COMMAND ""

#     #################### No.3 Configure Step ####################
#     # 依存関係
#     DEPENDS ${ZLIB_TARGET}
#     # ジェネレータ
#     # CMAKE_GENERATOR ""
#     # cmake コマンドに渡す引数
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${LIBPNG_INSTALL} 
#                -DBUILD_SHARED_LIBS=OFF
#                -DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
#                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#                -DCMAKE_C_FLAGS=${GL_BINDING_C_FLAGS}
#                -DPNG_BUILD_ZLIB=ON
#                -DPNG_DEBUG=off
    
#     #################### No.4 Build Step ####################
#     # ビルドコマンド
#     #BUILD_COMMAND ""
#     # Build In Source or Build Out Source
#     BUILD_IN_SOURCE OFF
#     # Build Always or Not
#     BUILD_ALWAYS OFF

#     #################### No.5 Install Step ####################
#     # インストールコマンド
#     #INSTALL_COMMAND ""
#     #################### No.6 Test Step ####################
#     #################### No.7 Output Logging ####################
#     LOG_DOWNLOAD ON
#     LOG_UPDATE ON
#     LOG_PATCH ON
#     LOG_CONFIGURE ON
#     LOG_BUILD ON
#     LOG_INSTALL ON
#     LOG_TEST ON
#     LOG_MERGED_STDOUTERR ON
#     LOG_OUTPUT_ON_FAILURE ON
#     #################### No.8 Terminal Access ####################
#     #################### No.9 Target ####################
#     #################### No.10 Miscellaneous ####################
# )

# ExternalProject_Get_Property(${LIBPNG_TARGET} INSTALL_DIR)
# message(STATUS "libpng's installed dir: ${INSTALL_DIR}")

# message(STATUS "libpng target: ${LIBPNG_TARGET}")
# target_include_directories(${LIBPNG_TARGET} ${ZLIB_INCLUDE_DIR})
# target_link_directories(${LIBPNG_TARGET} ${ZLIB_LIBRARY})


# # # zlib Target
# # # https://www.kofuk.org/blog/20200526-cmake-external-project/
# # add_library(zlib IMPORTED SHARED)
# # if(MSVC)
# #     set_target_properties(zlib PROPERTIERS
# #         IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/third-party/zlib-1.2.11/zlib1.dll
# #     )
# # else()
# #     set_target_properties(zlib PROPERTIERS
# #             IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/third-party/zlib-1.2.11/zlib1.so
# #         )
# # endif()
# set(PNG_BUILD_ZLIB ON) # for user's custom zlib build
# set(ZLIB_INCLUDE_DIR ${ZLIB_PUBLIC_HDRS})
# message(STATUS "ZLIB_INCLUDE_DIR: ${ZLIB_INCLUDE_DIR}")
# # message()
# add_subdirectory(libpng-1.6.35)
# # target_include_directories(png PUBLIC ${ZLIB_INCLUDE_DIR})

