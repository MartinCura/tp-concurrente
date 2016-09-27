cmake_minimum_required(VERSION 3.6)
project(tp_concurrente)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/include/jsoncpp)

set(SOURCE_FILES main.cpp Recepcionista.cpp Recepcionista.h Mozo.cpp Mozo.h Cocinero.cpp Cocinero.h Empleado.cpp Empleado.h Comensal.cpp Comensal.h Restaurante.cpp Restaurante.h Plato.cpp Plato.h)
add_executable(tp_concurrente ${SOURCE_FILES})