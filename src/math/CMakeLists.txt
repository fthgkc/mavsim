include_directories(${BOOSTNUMERICBINDINGS_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(oooarkMath SHARED
	utilities.cpp
	GpsIns.cpp
	)
add_dependencies(oooarkMath BOOSTNUMERICBINDINGS)
target_link_libraries(oooarkMath ${Boost_LIBRARIES} ${LAPACK_LIBRARIES})
set_target_properties(oooarkMath PROPERTIES VERSION ${oooark_VERSION}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkMath DESTINATION lib)

install(FILES
	utilities.hpp
	GpsIns.hpp
	DESTINATION include/oooark/math
	)
