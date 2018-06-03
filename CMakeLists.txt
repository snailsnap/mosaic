cmake_minimum_required(VERSION 3.1)

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets)

set(output_dir bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})

set(source_dir src)

set(sources
    ${source_dir}/main.cpp
	${source_dir}/mosaic.cpp
	${source_dir}/mosaic.hpp
	${source_dir}/mollusc.cpp
	${source_dir}/mollusc.hpp
	${source_dir}/mainwindow.cpp
	${source_dir}/mainwindow.hpp
)

add_executable(${target} ${sources})
target_link_libraries(${target} Qt5::Widgets)
