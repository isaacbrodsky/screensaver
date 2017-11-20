cmake_minimum_required (VERSION 2.8.11)

project (Screensaver)

set(SOURCE_FILES src/winscreensaver.cpp
    src/main.cpp
	src/building.cpp
	src/star.cpp
	include/winscreensaver.h
	include/main.h
	include/building.h
	include/star.h)

INCLUDE_DIRECTORIES("include")

add_executable(Myscrsvr ${SOURCE_FILES})

if (win32)
    # Setup SDL
    INCLUDE_DIRECTORIES("E:/Dev/SDL2-2.0.4/include")
    TARGET_LINK_LIBRARIES(Myscrsvr "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2.lib")
    TARGET_LINK_LIBRARIES(Myscrsvr "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2main.lib")
    add_custom_command(TARGET Myscrsvr POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "E:/Dev/SDL2-2.0.4/VisualC/Win32/Debug/SDL2.dll"
            $<TARGET_FILE_DIR:Myscrsvr>)

    # Name the screensaver correctly
    SET(CMAKE_EXECUTABLE_SUFFIX "")
    SET_TARGET_PROPERTIES(Myscrsvr PROPERTIES OUTPUT_NAME Myscrsvr.scr)
else()
    set(CMAKE_CXX_FLAGS "-std=c++11")
    include_directories("/usr/include/SDL2")
    link_directories("/usr/lib/x86_64-linux-gnu")
    target_link_libraries(Myscrsvr "SDL2")
endif()
