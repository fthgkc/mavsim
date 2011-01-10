include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS} ${QT_INCLUDE_DIRS})
link_libraries(${OPENSCENEGRAPH_LIBRARIES} ${Boost_LIBRARIES} ${QT_LIBRARIES})

set(oooarkVisualizationSource
	osgUtils.cpp
	Viewer.cpp
	)

set(oooarkVisualizationHeaders
	osgUtils.hpp
	Viewer.hpp
	)

if(QT_FOUND)
	list(APPEND oooarkVisualizationSource
		QOSGAdapterWidget.cpp
	)
	list(APPEND oooarkVisualizationHeaders
		QOSGAdapterWidget.hpp
	)
endif(QT_FOUND)

add_library(oooarkVisualization SHARED ${oooarkVisualizationSource})

set_target_properties(oooarkVisualization PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkVisualization DESTINATION lib)
install(FILES ${oooarkVisualizationHeaders} DESTINATION include/oooark/visualization)
