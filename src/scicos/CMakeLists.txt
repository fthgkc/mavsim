include_directories( "${PROJECT_SOURCE_DIR}/src" ${SCICOSLAB_INCLUDE_DIRS}
	${JSBSIM_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

link_directories( ${OSG_LIBRARY_DIRS} ) 

set(oooarkScicosSources
	sci_mavlinkHilSensor.cpp
	sci_mavlinkHilState.cpp
	sci_mavlinkHilTracker.cpp
	sci_invPend.cpp
    utilities.cpp
    sci_jsbsimTrim.cpp
    sci_jsbsimComm.cpp
	)

if (SIMGEAR_FOUND)
	list(APPEND oooarkScicosSources
		sci_flightGearComm.cpp
	)
endif (SIMGEAR_FOUND)

if (LAPACK_FOUND)
	list(APPEND oooarkScicosSources
    	sci_gpsIns.cpp
	)
endif (LAPACK_FOUND)

if (OPENSCENEGRAPH_FOUND)
	list(APPEND oooarkScicosSources
		sci_jet.cpp
		sci_plane.cpp
		sci_car.cpp
		sci_quad.cpp
	)
endif(OPENSCENEGRAPH_FOUND)

add_library(oooarkScicos SHARED ${oooarkScicosSources})

target_link_libraries(oooarkScicos ${OOOARK_LIBRARIES} ${JSBSIM_LIBRARIES})

set_target_properties(oooarkScicos PROPERTIES VERSION ${oooark_VERISON}
    SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkScicos DESTINATION lib)

install(FILES
    definitions.hpp
    utilities.hpp
    DESTINATION include/oooark/scicos
    )
