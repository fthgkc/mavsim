include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIRS} ${OSG_INCLUDE_DIRS})

add_definitions(-DUSE_QT4)

add_library(oooarkVisualization SHARED
	osgUtils.cpp
	Viewer.cpp
	QOSGAdapterWidget.cpp
	)
target_link_libraries(oooarkVisualization ${OSG_LIBRARIES} ${Boost_LIBRARIES} )
set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)

install(FILES
	osgUtils.hpp
	Viewer.hpp
	QOSGAdapterWidget.hpp
	DESTINATION include/oooark/visualization
	)
