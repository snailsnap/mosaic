cmake_minimum_required(VERSION 3.1)

# project settings

set(target SnailSnap)
project(${target})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

# dependencies

find_package(Qt5 COMPONENTS Widgets Multimedia Network)

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

# smtp client lib

file(GLOB smtp_sources "dependencies/smtpclient/src/*")
add_definitions(-DSMTP_BUILD)
add_library(SmtpClient ${smtp_sources})
target_link_libraries(SmtpClient Qt5::Widgets Qt5::Network)
if(APPLE)
	set_target_properties(SmtpClient PROPERTIES FRAMEWORK TRUE)
endif()

# voronoi lib - not really needed as target, but I like having access to it in VS

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
	${source_dir}/mail.cpp
	${source_dir}/Webcam.cpp
	${source_dir}/molluscpalette.cpp
	${source_dir}/molluscview.cpp
	${source_dir}/helpers/painter.cpp
	${source_dir}/helpers/boundingbox.cpp
	${source_dir}/helpers/positionGenerator.cpp
)

set(headers
	${source_dir}/mosaic.hpp
	${source_dir}/algorithms/floyd-steinberg.hpp
	${source_dir}/algorithms/voronoi.hpp
	${source_dir}/mollusc.hpp
	${source_dir}/mainwindow.hpp
	${source_dir}/mail.hpp
	${source_dir}/Webcam.hpp
	${source_dir}/molluscpalette.hpp
	${source_dir}/molluscview.hpp
	${source_dir}/helpers/painter.hpp
	${source_dir}/helpers/boundingbox.hpp
	${source_dir}/helpers/positionGenerator.hpp
)

# target and required libs

add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} Qt5::Widgets Qt5::Multimedia SmtpClient)

# enable source file tree in VS if cmake supports it

if (NOT ${CMAKE_VERSION} VERSION_LESS "3.8.0")
	option(VS_FILE_TREE_SPLIT_HEADER_AND_SOURCE "In the Visual Studio file list, headers and sources are normally split into two folders. Disable this to merge them." ON)
	
	if(${VS_FILE_TREE_SPLIT_HEADER_AND_SOURCE})
		source_group(TREE ${CMAKE_SOURCE_DIR}/${source_dir} PREFIX "Source Files" FILES ${sources})
		source_group(TREE ${CMAKE_SOURCE_DIR}/${source_dir} PREFIX "Header Files" FILES ${headers})
	else()
		source_group(TREE ${CMAKE_SOURCE_DIR}/${source_dir} PREFIX "Source Files" FILES ${sources} ${headers})
	endif()
endif()

# add option to enable voronoi's floodfill

option(VORONOI_USE_FLOODFILL "Use floodfill to detect cell colors during voronoi mosaic generation. If turned off, only onde pixel will be sampled. This can be useful when compiling the Debug configuration in VS, since the floodfill algorithm gets really slow in this case." OFF)

if (${VORONOI_USE_FLOODFILL})
	add_definitions(-DVORONOI_USE_FLOODFILL)
endif()

# copy resources to output dir

set(resource_dir resources)
file(GLOB resources ${CMAKE_SOURCE_DIR}/${resource_dir}/*.*)
foreach(resource ${resources})
	get_filename_component(filename ${resource} NAME)
	configure_file(${resource} ${CMAKE_BINARY_DIR}/${output_dir}/${resource_dir}/${filename} COPYONLY)
endforeach()
