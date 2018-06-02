cmake_minimum_required(VERSION 3.1)

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Multimedia)
find_package(Qt5Widgets)

set(output_dir bin)

add_subdirectory(src)
