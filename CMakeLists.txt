cmake_minimum_required(VERSION 3.10)

project(bandbox C)
include_directories(include)
set(CMAKE_C_STANDARD 11)


file(GLOB SOURCES "src/*.c")
add_executable(bandbox ${SOURCES})



