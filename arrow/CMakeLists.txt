cmake_minimum_required(VERSION 3.17)
project(arrow)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#add_library(arrow STATIC arrow.h arrow.cpp)
add_library(arrow SHARED arrow.h arrow.cpp)