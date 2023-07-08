cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCE_UI ui/*.h ui/*.c*)
file( GLOB SOURCE_UI_TEST test/ui_test/*.h test/ui_test/*.c*)

add_executable(ui_test ${SOURCE_UI} ${SOURCE_UI_TEST})

target_compile_definitions(ui_test PRIVATE -DMR_UI_WITH_TIO=1)

if(${MR_TARGET_OS} MATCHES "linux")
    target_link_libraries(ui_test PRIVATE mrbundle mrcommon textureio SDL2)
elseif(${MR_TARGET_OS} MATCHES "windows")
    target_link_libraries(ui_test PRIVATE mrbundle mrcommon textureio SDL2.dll dwmapi)
elseif(${MR_TARGET_OS} MATCHES "darwin")

elseif(${MR_TARGET_OS} MATCHES "android")

elseif(${MR_TARGET_OS} MATCHES "ios")
endif()
