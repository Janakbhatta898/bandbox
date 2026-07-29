cmake_minimum_required(VERSION 3.10)

project(bandbox C)
include_directories(include)
add_executable(bandbox src/main.c src/namespaces.c)
set(CMAKE_C_STANDARD 12)






