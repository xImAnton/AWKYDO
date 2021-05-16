cmake_minimum_required(VERSION 3.0)

project(AWKYDO VERSION 0.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(_UNICODE)

set(SOURCES
        main.cpp
        awklib.cpp
        DesktopWindow.cpp
        awklib.h
        DesktopWindow.h)

add_executable(AWKYDO WIN32 ${SOURCES})
