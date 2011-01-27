include_directories( "${PROJECT_SOURCE_DIR}/src" ${SCICOSLAB_INCLUDE_DIRS}
	${JSBSIM_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS} ${MAVLINK_INCLUDE_DIRS})

link_directories( ${OSG_LIBRARY_DIRS} ) 

set(oooarkScicosSources
	definitions.cpp
	sci_mavlinkHilSensor.cpp
	sci_mavlinkHilState.cpp
	sci_mavlinkHilTracker.cpp
	sci_invPend.cpp
	utilities.cpp
	sci_jsbsimTrim.cpp
	sci_jsbsimComm.cpp
	sci_insErrorDynamics.cpp
	sci_zeroOrderHold.cpp
	sci_insErrorDynamicsQ.cpp
	sci_randn
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
add_dependencies(oooarkScicos JSBSIM)

target_link_libraries(oooarkScicos ${OOOARK_LIBRARIES} ${JSBSIM_LIBRARIES})

set_target_properties(oooarkScicos PROPERTIES VERSION ${oooark_VERSION}
    SOVERSION ${oooark_SOVERSION})

add_custom_target(oooarkToolbox ALL
	DEPENDS oooarkScicos
    COMMAND cd ${PROJECT_SOURCE_DIR}/oooark-toolbox && ./build.sh)

get_target_property(SCICOSLAB_LOCATION oooarkScicos LOCATION)

set(SCICOSLAB_INSTALL_SETTINGS 0)
configure_file(loadscicos.sce.in ${CMAKE_SOURCE_DIR}/oooark-toolbox/scicos/loadscicos.sce)
set(SCICOSLAB_INSTALL_SETTINGS 1)
configure_file(loadscicos.sce.in ${CMAKE_CURRENT_BINARY_DIR}/loadscicos.sce)

configure_file(VERSION.in ${CMAKE_SOURCE_DIR}/oooark-toolbox/VERSION)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/loadscicos.sce DESTINATION ${SCICOSLAB_CONTRIB_DIR}/oooark-toolbox/scicos)

install(TARGETS oooarkScicos DESTINATION lib)

install(FILES
    definitions.hpp
    utilities.hpp
	DESTINATION include/oooark/scicos
    )
