include_directories(${APM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

set(oooarkNavigationSources
	GeoMag.cpp
	)

set(oooarkNavigationHeaders
	GeoMag.hpp
	)

add_library(oooarkNavigation SHARED ${oooarkNavigationSources})
target_link_libraries(oooarkNavigation ${SIMGEAR_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(oooarkNavigation PROPERTIES 
	VERSION ${oooark_VERSION} 
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkNavigation DESTINATION lib)
install(FILES ${oooarkNavigationHeaders} DESTINATION include/oooark/navigation)
