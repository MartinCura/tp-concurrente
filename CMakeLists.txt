cmake_minimum_required(VERSION 3.6)
project(tp_concurrente)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/include/jsoncpp)
include_directories(include)

file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(tp_concurrente "source/main.cpp" ${SOURCES})
