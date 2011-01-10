include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(communication)

if (OPENSCENEGRAPH_FOUND) 
	add_subdirectory(visualization)
endif(OPENSCENEGRAPH_FOUND)

if (LAPACK_FOUND)
	add_subdirectory(math)
endif(LAPACK_FOUND)

if (SCICOSLAB_FOUND)
	add_subdirectory(scicos)
endif(SCICOSLAB_FOUND)
