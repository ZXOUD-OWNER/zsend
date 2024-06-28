cmake_minimum_required(VERSION 3.29.1 FATAL_ERROR)
project(zSend VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.1.1")
        message(FATAL_ERROR "GCC version must be at least 14.1.1!")
    endif()
endif()

include(CheckCXXCompilerFlag)

function(add_compiler_flag_if_supported flag)
    string(REPLACE "-" "_" flag_var "${flag}")
    set(CXX_FLAG_SUPPORTED_VAR "CXX_FLAG_${flag_var}_SUPPORTED")
    check_cxx_compiler_flag("${flag}" ${CXX_FLAG_SUPPORTED_VAR})
    if(${${CXX_FLAG_SUPPORTED_VAR}})
        add_compile_options("${flag}")
    endif()
endfunction()

add_compiler_flag_if_supported("-Wextra-semi-stmt")
add_compiler_flag_if_supported("-Wfloat-conversion")
add_compiler_flag_if_supported("-Wformat=2")
add_compiler_flag_if_supported("-Wlogical-op")
add_compiler_flag_if_supported("-Wpointer-arith")
add_compiler_flag_if_supported("-Wshadow")
add_compiler_flag_if_supported("-Wshorten-64-to-32")
add_compiler_flag_if_supported("-Wsign-compare")
add_compiler_flag_if_supported("-Wstring-conversion")
add_compiler_flag_if_supported("-Wtype-limits")
add_compiler_flag_if_supported("-Wuninitialized")
add_compiler_flag_if_supported("-Wunreachable-code-aggressive")
add_compiler_flag_if_supported("-Wunused")
add_compiler_flag_if_supported("-Wvla")
add_compiler_flag_if_supported("-Wextra-semi")


# set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -fcoroutines")

add_compile_options(
    $<$<CONFIG:DEBUG>:-Wall>
    $<$<CONFIG:DEBUG>:-Wextra>
    $<$<CONFIG:RELEASE>:-O2>
    $<$<CONFIG:RELEASE>:-march=x86-64-v3>
    $<$<CONFIG:RELEASE>:-pipe>
    $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:RELEASE>:-flto=auto>
    $<$<CONFIG:DEBUG>:-fcoroutines>
    $<$<CONFIG:RELEASE>:-fcoroutines>
)


# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=x86-64-v3 -pipe -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen -frerun-cse-after-loop -fstrict-aliasing -fweb -ftree-vrp -ftree-pta -ftree-loop-vectorize -fvect-cost-model -finline-functions -funswitch-loops -flto=auto")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=auto")
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig/")
find_package (glog REQUIRED VERSION 0.7.0 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(unwind REQUIRED IMPORTED_TARGET "libunwind>=1.8.1")
pkg_check_modules(zmq REQUIRED IMPORTED_TARGET "libzmq>=4.3.5" "libczmq>=4.2.1")
pkg_check_modules(redis REQUIRED IMPORTED_TARGET "hiredis>=1.2.0")
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdPart/libyuv/include)
# #INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdPart/nvencSDK/include)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdPart/decklinkSDK/include)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdPart/ffmpeg/include)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rdPart/MagewellCaptureLib/include)


LINK_DIRECTORIES("/usr/lib")
INCLUDE_DIRECTORIES("/usr/include")
INCLUDE_DIRECTORIES("/usr/local/include/")
INCLUDE_DIRECTORIES("/usr/local/include/mimalloc-2.1")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/head)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCES)

add_library(zSend SHARED ${SOURCES})
target_link_libraries(zSend pthread PkgConfig::zmq glog::glog PkgConfig::redis -lunwind -lunwind-x86_64)