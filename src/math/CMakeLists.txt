include_directories(${BOOSTNUMERICBINDINGS_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(mavsimMath SHARED
	utilities.cpp
	GpsIns.cpp
	)
add_dependencies(mavsimMath BOOSTNUMERICBINDINGS)
target_link_libraries(mavsimMath ${Boost_LIBRARIES} ${LAPACK_LIBRARIES})
set_target_properties(mavsimMath PROPERTIES VERSION ${mavsim_VERSION}
	SOVERSION ${mavsim_SOVERSION})

install(TARGETS mavsimMath DESTINATION lib)

install(FILES
	utilities.hpp
	GpsIns.hpp
	DESTINATION include/mavsim/math
	)
