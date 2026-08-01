cmake_minimum_required(VERSION 3.10)

project(bandbox C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_definitions(_GNU_SOURCE)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(bandbox ${SOURCES})

#link required libraries
target_link_libraries(bandbox PRIVATE seccomp)


