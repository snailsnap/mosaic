set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${output_dir})

set(sources
	main.cpp
	mainwindow.cpp
	mainwindow.hpp
	mosaic.cpp
	mosaic.hpp
)

add_executable(${target} ${sources})
target_link_libraries(${target} Qt5::Widgets)
