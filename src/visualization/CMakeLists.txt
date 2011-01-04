link_directories(${OSG_LIBRARY_DIRS}) 
include_directories(${OSG_INCLUDE_DIRS})

add_library(oooarkVisualization SHARED
	osgUtils.cpp
	Viewer.cpp
	)
target_link_libraries(oooarkVisualization ${OSG_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)

install(FILES
	osgUtils.hpp
	Viewer.hpp
	DESTINATION include/oooark/visualization
	)
