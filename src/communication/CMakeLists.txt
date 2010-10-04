include_directories(${JSBSIM_INCLUDE_DIRS} /usr/include/apmhsl)

add_library(oooarkCommunication SHARED
	ApmProtocol.cpp
	AsyncSerial.cpp
	FlightGearComm.cpp
	JSBSimComm.cpp
	flightGearUtils.cpp
	)
target_link_libraries(oooarkCommunication jsbsim sgio apmhsl)
set_target_properties(oooarkCommunication PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkCommunication DESTINATION lib)

install(FILES
	ApmProtocol.hpp
	AsyncSerial.hpp
	FlightGearComm.hpp
	JSBSimComm.hpp
	flightGearUtils.hpp
	net_ctrls.hxx
	net_fdm.hxx
	DESTINATION include/oooark/communication
	)
