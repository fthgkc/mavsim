include_directories(${Boost_INCLUDE_DIRS})

add_library(oooarkMath SHARED
	utilities.cpp
	)
target_link_libraries(oooarkMath ${Boost_LIBRARIES} ${LAPACK_LIBRARIES})
set_target_properties(oooarkMath PROPERTIES VERSION ${oooark_VERISON}
	SOVERSION ${oooark_SOVERSION})

install(TARGETS oooarkMath DESTINATION lib)

install(FILES
	utilities.hpp
	DESTINATION include/oooark/math
	)
