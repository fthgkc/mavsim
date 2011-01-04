include_directories( "${PROJECT_SOURCE_DIR}/src" ${SCICOSLAB_INCLUDE_DIRS}
    ${JSBSIM_INCLUDE_DIRS} ${OSG_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

link_directories( ${OSG_LIBRARY_DIRS} ) 

add_library(oooarkScicos SHARED
	sci_mavlinkHilSensor.cpp
	sci_mavlinkHilState.cpp
	sci_mavlinkHilTracker.cpp
    sci_invPend.cpp
    sci_jet.cpp
	sci_plane.cpp
	sci_car.cpp
	sci_quad.cpp
    sci_jsbsimComm.cpp
    sci_gpsIns.cpp
    sci_jsbsimTrim.cpp
    utilities.cpp
    )
target_link_libraries(oooarkScicos oooarkCommunication oooarkVisualization oooarkMath
    ${JSBSIM_LIBRARIES})

set_target_properties(oooarkScicos PROPERTIES VERSION ${oooark_VERISON}
    SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkScicos DESTINATION lib)

install(FILES
    definitions.hpp
    utilities.hpp
    DESTINATION include/oooark/scicos
    )
