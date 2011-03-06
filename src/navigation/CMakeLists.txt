include_directories(${APM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

set(mavsimNavigationSources
	GeoMag.cpp
	)

set(mavsimNavigationHeaders
	GeoMag.hpp
	)

add_library(mavsimNavigation SHARED ${mavsimNavigationSources})
target_link_libraries(mavsimNavigation ${SIMGEAR_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(mavsimNavigation PROPERTIES 
	VERSION ${mavsim_VERSION} 
	SOVERSION ${mavsim_SOVERSION})

install(TARGETS mavsimNavigation DESTINATION lib)
install(FILES ${mavsimNavigationHeaders} DESTINATION include/mavsim/navigation)
