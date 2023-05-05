cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VGFX)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cross_compile)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#######################arch and os detect#######################
if(NOT DEFINED ${MP_TARGET_OS})
    set(MP_TARGET_OS ${CMAKE_SYSTEM_NAME})
endif()
string(TOLOWER ${MP_TARGET_OS} MP_TARGET_OS)
if(NOT DEFINED  ${MP_TARGET_ARCH})
    set(MP_TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()


if(${MP_TARGET_OS} MATCHES "linux")
    include_directories(/usr/include/drm)
elseif(${MP_TARGET_OS} MATCHES "windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(MP_TARGET_OS "mingw")
    endif()
elseif(${MP_TARGET_OS} MATCHES "darwin")

elseif(${MP_TARGET_OS} MATCHES "android")

elseif(${MP_TARGET_OS} MATCHES "ios")
endif()

if(${MP_TARGET_ARCH} MATCHES "AMD64")
    set(MP_TARGET_ARCH "x86_64")
endif()

set(THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../third_party")
set(THIRD_PARTY_TARGET "${THIRD_PARTY_ROOT}/targets/${MP_TARGET_OS}-${MP_TARGET_ARCH}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    "${THIRD_PARTY_ROOT}/include"
    "${THIRD_PARTY_ROOT}/include/imgui"
    "${THIRD_PARTY_TARGET}/include"
    "${THIRD_PARTY_TARGET}/include/luajit"
    "${THIRD_PARTY_TARGET}/include/freetype2")
link_directories("${THIRD_PARTY_TARGET}/lib")
#######################arch and os detect end#######################



add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vgfx)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/samples/hello_vgfx)
