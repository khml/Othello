cmake_minimum_required(VERSION 3.0)

project(PyOthello)
set(PYBIND11_CPP_STANDARD -std=c++14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")
add_subdirectory(pybind11)

file(GLOB SOURCES ../src/Othello.hpp ../src/Othello.cpp)

pybind11_add_module(othello SHARED ${SOURCES} PyOthello.cpp)
