cmake_minimum_required(VERSION 3.0)

project(AWKYDO VERSION 0.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(_UNICODE)

set(SOURCES
        src/main.cpp
        src/awklib.cpp
        src/DesktopWindow.cpp
        src/awklib.h
        src/DesktopWindow.h)

add_executable(AWKYDO WIN32 ${SOURCES})
