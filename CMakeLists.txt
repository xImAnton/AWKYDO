cmake_minimum_required(VERSION 3.0)

project(AWKYDO VERSION 0.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_definitions(_UNICODE)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(AWKYDO WIN32 ${SOURCES})

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
