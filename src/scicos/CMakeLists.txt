include_directories(${SCICOSLAB_INCLUDE_DIRS}
	${JSBSIM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
	${MAVLINK_INCLUDE_DIRS}
	)

set(mavsimScicosSources
	definitions.cpp
	sci_mavlinkHilSensor.cpp
	sci_mavlinkHilState.cpp
	sci_mavlinkHilTracker.cpp
	sci_invPend.cpp
	utilities.cpp
	sci_jsbsimTrim.cpp
	sci_jsbsimComm.cpp
	sci_insErrorDynamics.cpp
	sci_insDynamics.cpp
	sci_zeroOrderHold.cpp
	sci_randn.cpp
	sci_geoMag.cpp
	sci_magMeasModel.cpp
	sci_gpsMeasModel.cpp
	sci_quat2EulerDcm.cpp
	sci_waypointGuidance.cpp
	sci_euler2Dcm.cpp
	sci_quadForceMoments.cpp
	sci_eom6Dof.cpp
	)

if (SIMGEAR_FOUND)
	list(APPEND mavsimScicosSources
		sci_flightGearComm.cpp
	)
endif (SIMGEAR_FOUND)

if (LAPACK_FOUND)
	list(APPEND mavsimScicosSources
    	sci_gpsIns.cpp
	)
endif (LAPACK_FOUND)

if (OPENSCENEGRAPH_FOUND)
	list(APPEND mavsimScicosSources
		sci_jet.cpp
		sci_plane.cpp
		sci_car.cpp
		sci_quad.cpp
	)
	include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
	link_directories( ${OSG_LIBRARY_DIRS} ) 
endif(OPENSCENEGRAPH_FOUND)
set(CMAKE_C_FLAGS -lpthread)
add_library(mavsimScicos MODULE ${mavsimScicosSources})
add_dependencies(mavsimScicos JSBSIM)

target_link_libraries(mavsimScicos ${OOOARK_LIBRARIES} ${JSBSIM_LIBRARIES} ${LAPACK_LIBRARIES} -lpthread)

set_target_properties(mavsimScicos PROPERTIES VERSION ${mavsim_VERSION}
    SOVERSION ${mavsim_SOVERSION})

add_custom_target(mavsimToolbox ALL
	DEPENDS mavsimScicos
    COMMAND cd ${PROJECT_SOURCE_DIR}/mavsim-toolbox && ./build.sh)

get_target_property(SCICOSLAB_LOCATION mavsimScicos LOCATION)

set(SCICOSLAB_INSTALL_SETTINGS 0)
configure_file(loadscicos.sce.in ${CMAKE_SOURCE_DIR}/mavsim-toolbox/scicos/loadscicos.sce)
set(SCICOSLAB_INSTALL_SETTINGS 1)
configure_file(loadscicos.sce.in ${CMAKE_CURRENT_BINARY_DIR}/loadscicos.sce)

configure_file(VERSION.in ${CMAKE_SOURCE_DIR}/mavsim-toolbox/VERSION)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/loadscicos.sce DESTINATION ${SCICOSLAB_CONTRIB_DIR}/mavsim-toolbox/scicos)

install(TARGETS mavsimScicos DESTINATION lib)

install(FILES
    definitions.hpp
    utilities.hpp
	DESTINATION include/mavsim/scicos
    )
