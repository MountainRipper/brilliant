cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file( GLOB SOURCE_UI_TEST ui_test/*.h ui_test/*.c*)

add_executable(ui_test ${SOURCE_UI_TEST})

target_compile_definitions(ui_test PRIVATE -DMR_UI_WITH_TIO=1)

if(${MR_TARGET_OS} MATCHES "linux")
    target_link_libraries(ui_test PRIVATE mrbundle mrcommon mrui textureio SDL2)
elseif(${MR_TARGET_OS} MATCHES "windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(MR_TARGET_OS "mingw")
        target_link_libraries(ui_test mrbundle mrcommon mrui textureio SDL2.dll luajit-5.1.dll dwmapi shlwapi)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_link_libraries(ui_test mrbundle mrcommon mrui textureio SDL2 lua51 dwmapi shlwapi )
    endif()
elseif(${MR_TARGET_OS} MATCHES "darwin")

elseif(${MR_TARGET_OS} MATCHES "android")

elseif(${MR_TARGET_OS} MATCHES "ios")
endif()

add_custom_command(TARGET ui_test
                    POST_BUILD
                    COMMAND echo  "copy ui_test scripts and resources"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/ui_test/resources"  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
                   )
