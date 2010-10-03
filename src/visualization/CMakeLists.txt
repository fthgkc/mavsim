include_directories(${JSBSIM_INCLUDE_DIRS} ${OSG_INCLUDE_DIRS})

add_library(oooarkVisualization SHARED
	Ellipsoid.cpp
	osgUtils.cpp
	PointCloud.cpp
	Viewer.cpp
	)
target_link_libraries(oooarkVisualization osg osgViewer ${Boost_LIBRARIES} )
set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)

install(FILES
	Ellipsoid.hpp
	osgUtils.hpp
	PointCloud.hpp
	Viewer.hpp
	DESTINATION include/oooark/visualization
	)
