cmake_minimum_required(VERSION 2.8)

project(pjit)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} pseudojit.cpp pseudocompiler.hpp fnvhash.hpp)
