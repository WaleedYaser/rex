cmake_minimum_required(VERSION 3.16.0)
project(rex LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(rex-core)
add_subdirectory(rex-raster)
add_subdirectory(rex-viewer)

include(CTest)
add_subdirectory(rex-utests)
add_subdirectory(rex-scratch)

install(TARGETS rex-viewer rex-raster DESTINATION ".")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ".")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/License.txt")
include(CPack)
