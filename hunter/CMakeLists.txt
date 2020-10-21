cmake_minimum_required(VERSION 3.17)
project(hunter)

set(CMAKE_CXX_STANDARD 20)

add_executable(hunter main.cpp)
target_link_libraries(hunter ${CMAKE_SOURCE_DIR}/../arrow/cmake-build-debug/libarrow.a)