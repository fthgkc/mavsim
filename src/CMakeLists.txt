include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.h.in ${CMAKE_BINARY_DIR}/src/config.h)
install(FILES ${CMAKE_BINARY_DIR}/src/config.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/oooark)

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
