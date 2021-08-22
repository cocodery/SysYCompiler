cmake_minimum_required(VERSION 3.13)
set(project_name compiler)
project(${project_name} CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(${project_name} main.cpp)
