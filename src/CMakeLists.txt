include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.h.in ${CMAKE_BINARY_DIR}/src/config.h)
install(FILES ${CMAKE_BINARY_DIR}/src/config.h DESTINATION include/mavsim)

add_subdirectory(communication)
add_subdirectory(navigation)

if (OPENSCENEGRAPH_FOUND) 
	add_subdirectory(visualization)
endif(OPENSCENEGRAPH_FOUND)

if (LAPACK_FOUND)
	add_subdirectory(math)
endif(LAPACK_FOUND)

if (SCICOSLAB_FOUND)
	if(UNIX)
		add_subdirectory(scicos)
	elseif(APPLE)
		message(STATUS "Scicoslab toolbox not yet supported on mac.")
	else()
		message(STATUS "Scicoslab toolbox currently only works for linux.")
	endif()
endif(SCICOSLAB_FOUND)
