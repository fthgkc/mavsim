include_directories( "${PROJECT_SOURCE_DIR}/src" ${SCICOSLAB_INCLUDE_DIRS}
    ${JSBSIM_INCLUDE_DIRS} /usr/local/include/JSBSim ${OSG_INCLUDE_DIRS}
    ${APM_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

link_directories( /usr/local/lib ${OSG_LIBRARY_DIRS} ) 

add_library(oooarkScicos SHARED
    sci_ardupilotmegaHil.cpp
    sci_mavlinkHil.cpp
    #sci_flightGearComm.cpp
    sci_invPend.cpp
    sci_jet.cpp
    sci_jsbsimComm.cpp
    sci_gpsIns.cpp
    sci_jsbsimTrim.cpp
    utilities.cpp
    )
target_link_libraries(oooarkScicos oooarkCommunication oooarkVisualization oooarkMath
    ${APM_LIBRARIES} ${JSBSIM_LIBRARIES})

set_target_properties(oooarkScicos PROPERTIES VERSION ${oooark_VERISON}
    SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkScicos DESTINATION lib)

install(FILES
    definitions.hpp
    utilities.hpp
    DESTINATION include/oooark/scicos
    )
