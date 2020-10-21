cmake_minimum_required(VERSION 3.17)
project(dll_injection)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(dll_injection main.cpp)