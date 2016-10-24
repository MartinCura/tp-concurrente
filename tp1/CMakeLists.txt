cmake_minimum_required(VERSION 3.6)
project(tp_concurrente)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

include_directories(/usr/include/jsoncpp)
include_directories(include)

file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(tp_concurrente "source/main.cpp" ${SOURCES} source/procesos/ProcesoMesasManager.cpp include/procesos/ProcesoMesasManager.h source/utils/Utils.cpp)
