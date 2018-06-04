cmake_minimum_required(VERSION 3.1)

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets Multimedia)

set(output_dir bin)

add_subdirectory(src)
