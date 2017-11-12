cmake_minimum_required (VERSION 2.8.11)

project (Screensaver)

set(SOURCE_FILES src/main.cpp
    src/winscreensaver.cpp
	include/winscreensaver.h)

INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES("E:/Dev/SDL2-2.0.4/include")

SET(CMAKE_EXECUTABLE_SUFFIX "")

add_executable(Myscrsvr ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Myscrsvr "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2.lib")
TARGET_LINK_LIBRARIES(Myscrsvr "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2main.lib")
add_custom_command(TARGET Myscrsvr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2.dll"
        $<TARGET_FILE_DIR:Myscrsvr>)

SET_TARGET_PROPERTIES(Myscrsvr PROPERTIES OUTPUT_NAME Myscrsvr.scr)