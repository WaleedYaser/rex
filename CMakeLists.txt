cmake_minimum_required(VERSION 3.12.0)
project(rex LANGUAGES CXX VERSION 2021.12.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(REX_HOT_RELOAD OFF)

add_subdirectory(rex-core)
add_subdirectory(rex-raster)
add_subdirectory(rex-viewer)

include(CTest)
add_subdirectory(rex-utests)
add_subdirectory(rex-scratch)

get_target_property(REX_CORE_DIR rex-core SOURCE_DIR)
configure_file(${REX_CORE_DIR}/include/rex-core/version.h.in ${REX_CORE_DIR}/include/rex-core/version.h)

add_custom_target(rex-deploy-data ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/
	COMMENT "[rex-deploy]: deploy rex data."
)

if (REX_HOT_RELOAD)
	install(TARGETS rex-viewer rex-raster RUNTIME DESTINATION ".")
else()
	install(TARGETS rex-viewer RUNTIME DESTINATION ".")
endif()
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ".")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
include(CPack)
