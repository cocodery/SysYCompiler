cmake_minimum_required(VERSION 3.16)
set(project_name compiler)
project(${project_name} CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "source/*.cpp")
file(GLOB_RECURSE header_files "include/*.h")
add_executable(${project_name} ${source_files} ${header_files})
