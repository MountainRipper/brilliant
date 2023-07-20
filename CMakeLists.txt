cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Brilliant)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#######################arch and os detect#######################
if(NOT DEFINED MR_TARGET_OS)
    set(MR_TARGET_OS ${CMAKE_SYSTEM_NAME})
endif()
string(TOLOWER ${MR_TARGET_OS} MR_TARGET_OS)
if(NOT DEFINED  MR_TARGET_ARCH)
    set(MR_TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

set(MR_COMPILER_VER "gcc")
set(MR_BUILD_TYPE "release          ")
string(TOLOWER "${CMAKE_BUILD_TYPE}" MR_BUILD_TYPE)

if(${MR_TARGET_OS} MATCHES "linux")
    include_directories(/usr/include/drm)
elseif(${MR_TARGET_OS} MATCHES "windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(MR_COMPILER_VER "mingw")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if ("${MSVC_TOOLSET_VERSION}" STREQUAL "143")
            set(MR_COMPILER_VER "msvc2022")
        elseif ("${MSVC_TOOLSET_VERSION}" STREQUAL "142")
            set(MR_COMPILER_VER "msvc2019")
        elseif ("${MSVC_TOOLSET_VERSION}" STREQUAL "141")
            set(MR_COMPILER_VER "msvc2017")
        endif()
    endif()
elseif(${MR_TARGET_OS} MATCHES "darwin")

elseif(${MR_TARGET_OS} MATCHES "android")

elseif(${MR_TARGET_OS} MATCHES "ios")
endif()

if(${MR_TARGET_ARCH} MATCHES "AMD64")
    set(MR_TARGET_ARCH "x86_64")
endif()

if(NOT DEFINED MR_THIRD_PARTY_ROOT)
    set(MR_THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../third_party")
    set(MR_THIRD_PARTY_TARGET "${MR_THIRD_PARTY_ROOT}/targets/${MR_BUILD_TYPE}/${MR_TARGET_OS}-${MR_COMPILER_VER}-${MR_TARGET_ARCH}")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        "${MR_THIRD_PARTY_ROOT}/include"
        "${MR_THIRD_PARTY_ROOT}/include/imgui"
        "${MR_THIRD_PARTY_TARGET}/include"
        "${MR_THIRD_PARTY_TARGET}/include/luajit"
        "${MR_THIRD_PARTY_TARGET}/include/freetype2")
    link_directories("${MR_THIRD_PARTY_TARGET}/lib")
    SET(MR_SEARCH_LIB_PATHS "${MR_THIRD_PARTY_TARGET}/lib")
endif()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
message(STATUS "INFO:BRILLIANT MAKE FOR OS:${MR_TARGET_OS} ARCH:${MR_TARGET_ARCH} SYSROOT:${CMAKE_SYSROOT}")
message(STATUS "INFO:BRILLIANT MAKE USE PLATFORM THIRD-PARTY LIBS:${MR_SEARCH_LIB_PATHS}")
#######################arch and os detect end#######################



add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdmp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vgfx)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/brilliant)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/brilliant-v)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
