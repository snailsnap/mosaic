cmake_minimum_required(VERSION 3.1)

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Widgets)
find_package(Qt5Network)

set(output_dir bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})

set(CMAKE_AUTOMOC ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(gcc_specific_flags "-finput-charset=Latin1 -fexec-charset=UTF-8")
	add_definitions(${gcc_specific_flags})
endif()

file(GLOB smtp_sources "dependencies/smtpclient/src/*")
add_definitions(-DSMTP_BUILD)
add_library(SmtpClient ${smtp_sources})
target_link_libraries(SmtpClient Qt5::Widgets Qt5::Network)

set(source_dir src)

set(sources
    ${source_dir}/main.cpp
	${source_dir}/mosaic.cpp
	${source_dir}/mollusc.cpp
	${source_dir}/mainwindow.cpp
)

set(headers
	${source_dir}/mosaic.hpp
	${source_dir}/mollusc.hpp
	${source_dir}/mainwindow.hpp
)

add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} Qt5::Widgets SmtpClient)
