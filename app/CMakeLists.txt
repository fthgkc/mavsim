if(QT4_FOUND AND OPENSCENEGRAPH_FOUND)
	add_subdirectory(trim)
endif()

if(QT4_FOUND AND JSBSIM_FOUND AND MAVLINK_FOUND)
	add_subdirectory(mavsim)
endif()
