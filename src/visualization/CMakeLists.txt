include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENSCENEGRAPH_LIBRARIES} 
	${Boost_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

set(mavsimVisualizationSource
	osgUtils.cpp
	Viewer.cpp
	)

set(mavsimVisualizationHeaders
	osgUtils.hpp
	Viewer.hpp
	)

if(QT_FOUND)
	list(APPEND mavsimVisualizationSource
		QOSGAdapterWidget.cpp
	)
	list(APPEND mavsimVisualizationHeaders
		QOSGAdapterWidget.hpp
	)
endif(QT_FOUND)

add_library(mavsimVisualization SHARED ${mavsimVisualizationSource})

set_target_properties(mavsimVisualization PROPERTIES VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION})

install(TARGETS mavsimVisualization DESTINATION lib)
install(FILES ${mavsimVisualizationHeaders} DESTINATION include/mavsim/visualization)
