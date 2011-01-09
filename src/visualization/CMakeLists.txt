link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_library(oooarkVisualization SHARED
	osgUtils.cpp
	Viewer.cpp
	)
target_link_libraries(oooarkVisualization ${OPENSCENEGRAPH_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)

install(FILES
	osgUtils.hpp
	Viewer.hpp
	DESTINATION include/oooark/visualization
	)
