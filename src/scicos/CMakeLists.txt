include_directories( "${PROJECT_SOURCE_DIR}/src" ${SCICOSLAB_INCLUDE_DIR}
	${JSBSIM_INCLUDE_DIR} ${OSG_INCLUDE_DIR} )
link_directories( ${OSG_LIBRARY_DIRS} ) 

add_library(oooarkScicos SHARED
	sci_ardupilotmegaHil.cpp
	sci_flightGearComm.cpp
	sci_invPend.cpp
	sci_jet.cpp
	sci_jsbSimComm.cpp
	sci_jsbsimComm.cpp
	sci_jsbsimTrim.cpp
	utilities.cpp
	)
target_link_libraries(oooarkScicos oooarkCommunication oooarkVisualization)
set_target_properties(oooarkScicos PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkScicos DESTINATION lib)

install(FILES
	definitions.hpp
	utilities.hpp
	DESTINATION include/oooark/scicos
	)
