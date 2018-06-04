cmake_minimum_required(VERSION 3.1)

# project settings

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# dependencies

find_package(Qt5Widgets)

# output dir

set(output_dir bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})

# charset fix for g++

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(gcc_specific_flags "-finput-charset=Latin1 -fexec-charset=UTF-8")
	add_definitions(${gcc_specific_flags})
endif()

# voronoi lib

set(jcv_sources dependencies/jc_voronoi/src/jc_voronoi.h)
add_library(JCVoronoi ${jcv_sources})
set_target_properties(JCVoronoi PROPERTIES LINKER_LANGUAGE CXX)

# snailsnap sources

set(source_dir src)

set(sources
    ${source_dir}/main.cpp
	${source_dir}/mosaic.cpp
	${source_dir}/algorithms/floyd-steinberg.cpp
	${source_dir}/algorithms/voronoi.cpp
	${source_dir}/mollusc.cpp
	${source_dir}/mainwindow.cpp
)

set(headers
	${source_dir}/mosaic.hpp
	${source_dir}/algorithms/floyd-steinberg.hpp
	${source_dir}/algorithms/voronoi.hpp
	${source_dir}/mollusc.hpp
	${source_dir}/mainwindow.hpp
)

add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} Qt5::Widgets)
