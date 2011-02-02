include_directories(${APM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

set(oooarkCommunicationSources
	AsyncSerial.cpp
	asio_mavlink_bridge.cpp
	)

set(oooarkCommunicationHeaders
	AsyncSerial.hpp 
	asio_mavlink_bridge.h
	)

if (SIMGEAR_FOUND)
	list(APPEND oooarkCommunicationSources
		FlightGearComm.cpp
		flightGearUtils.cpp
		)
	list(APPEND oooarkCommunicationHeaders
		FlightGearComm.hpp
		flightGearUtils.hpp
		net_ctrls.hxx
		net_fdm.hxx
		)
endif(SIMGEAR_FOUND)

add_library(oooarkCommunication SHARED ${oooarkCommunicationSources})
add_dependencies(oooarkCommunication MAVLINK)
target_link_libraries(oooarkCommunication ${SIMGEAR_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(oooarkCommunication PROPERTIES 
	VERSION ${oooark_VERSION} 
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkCommunication DESTINATION lib)
install(FILES ${oooarkCommunicationHeaders} DESTINATION include/oooark/communication)
