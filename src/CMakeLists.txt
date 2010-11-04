add_subdirectory(communication)
add_subdirectory(visualization)
add_subdirectory(scicos)
if(NOT APPLE)
	add_subdirectory(math)
endif(NOT APPLE)

