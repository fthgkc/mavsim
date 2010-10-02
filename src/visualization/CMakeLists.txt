include_directories(${JSBSIM_INCLUDE_DIR} ${OSG_INCLUDE_DIR})

add_library(oooarkVisualization SHARED
	Viewer.cpp
	cmake_install.cmake
	osgUtils.cpp
	)
target_link_libraries(oooarkVisualization osg osgViewer ${Boost_LIBRARIES} )
set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)

install(FILES
	Viewer.hpp
	osgUtils.hpp
	DESTINATION include/oooark/visualization
	)
