cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCE_UI *.h *.cpp)
file( GLOB SOURCE_YOGA yoga/*.h yoga/*.c* yoga/event/*.h yoga/event/*.c*)

add_library(mrui STATIC ${SOURCE_UI} ${SOURCE_YOGA})
target_include_directories(mrui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(mrui PRIVATE -DMR_UI_WITH_TIO=1)

if(${MR_TARGET_OS} MATCHES "linux")
    target_link_libraries(mrui PRIVATE imgui_stb textureio mrcommon  SDL2)
elseif(${MR_TARGET_OS} MATCHES "windows")
    target_link_libraries(mrui PRIVATE imgui_stb textureio mrcommon dwmapi)
elseif(${MR_TARGET_OS} MATCHES "darwin")

elseif(${MR_TARGET_OS} MATCHES "android")

elseif(${MR_TARGET_OS} MATCHES "ios")
endif()
