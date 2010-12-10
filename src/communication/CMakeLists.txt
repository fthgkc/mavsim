include_directories(${APM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

add_library(oooarkCommunication SHARED
	AsyncSerial.cpp
	asio_mavlink_bridge.cpp
	#FlightGearComm.cpp
	#flightGearUtils.cpp
	)
target_link_libraries(oooarkCommunication 
	#${SIMGEAR_LIBRARIES} 
	${Boost_LIBRARIES})

set_target_properties(oooarkCommunication PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkCommunication DESTINATION lib)

install(FILES
	AsyncSerial.hpp 
	asio_mavlink_bridge.h
	#FlightGearComm.hpp
	#flightGearUtils.hpp
	#net_ctrls.hxx
	#net_fdm.hxx
	DESTINATION include/oooark/communication
	)
