cmake_minimum_required (VERSION 2.8.11)

project (CodePageScreensaver)

set(SOURCE_FILES src/winscreensaver.cpp
    src/main.cpp
	src/charrender.cpp
	include/winscreensaver.h
	include/main.h
	include/charrender.h)

INCLUDE_DIRECTORIES("include")

add_executable(Myscrsvr2 ${SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Myscrsvr2 PRIVATE SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(Myscrsvr2 PRIVATE SDL2::SDL2_image)

if (WIN32)
    # Name the screensaver correctly
    SET(CMAKE_EXECUTABLE_SUFFIX "")
    SET_TARGET_PROPERTIES(Myscrsvr2 PROPERTIES OUTPUT_NAME Myscrsvr2.scr)
	target_link_libraries(Myscrsvr2 PRIVATE Shlwapi)
endif()
