include_directories( "${PROJECT_SOURCE_DIR}/src")
link_directories(${PROJECT_BINARY_DIR})

if (LAPACK_FOUND)
	add_executable(ins ins.cpp)
	target_link_libraries(ins oooarkMath)
endif (LAPACK_FOUND)

if (OPENSCENEGRAPH_FOUND)
	add_executable(visualization visualization.cpp)
	target_link_libraries(visualization oooarkVisualization)
endif (OPENSCENEGRAPH_FOUND)
