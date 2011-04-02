include_directories(${Boost_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

set(mavsimCommunicationLibs ${Boost_LIBRARIES})

set(mavsimCommunicationSources
	AsyncSerial.cpp
	asio_mavlink_bridge.cpp
	)

set(mavsimCommunicationHeaders
	AsyncSerial.hpp 
	asio_mavlink_bridge.h
	)

if (SIMGEAR_FOUND AND PLIB_FOUND)
	list(APPEND mavsimCommunicationSources
		FlightGearComm.cpp
		flightGearUtils.cpp
		)
	list(APPEND mavsimCommunicationHeaders
		FlightGearComm.hpp
		flightGearUtils.hpp
		net_ctrls.hxx
		net_fdm.hxx
		)
	list(APPEND mavsimCommunicationLibs
		${PLIB_LIBRARY} ${SIMGEAR_LIBRARIES} 
		)
endif()

add_library(mavsimCommunication SHARED ${mavsimCommunicationSources})
add_dependencies(mavsimCommunication MAVLINK)
target_link_libraries(mavsimCommunication ${mavsimCommunicationLibs})
set_target_properties(mavsimCommunication PROPERTIES 
	VERSION ${LIBRARY_VERSION} 
	SOVERSION ${LIBRARY_SOVERSION})

install(TARGETS mavsimCommunication DESTINATION lib)
install(FILES ${mavsimCommunicationHeaders} DESTINATION include/mavsim/communication)
