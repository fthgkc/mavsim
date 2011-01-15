include_directories(${BOOSTNUMERICBINDINGS_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(oooarkMath SHARED
	utilities.cpp
	GpsIns.cpp
	)
target_link_libraries(oooarkMath ${Boost_LIBRARIES} ${LAPACK_LIBRARIES})
set_target_properties(oooarkMath PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkMath DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES
	utilities.hpp
	GpsIns.hpp
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include/oooark/math
	)
