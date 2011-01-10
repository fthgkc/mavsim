configure_file(config.h.in ${CMAKE_BINARY_DIR}/src/config.h)
install(FILES ${CMAKE_BINARY_DIR}/src/config.h DESTINATION include/oooark)

configure_file(loader.sce.in ${CMAKE_SOURCE_DIR}/scicoslab/loader.sce)
