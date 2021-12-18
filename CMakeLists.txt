cmake_minimum_required(VERSION 3.12.0)
project(rex LANGUAGES CXX VERSION 2021.12.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

option(REX_HOT_RELOAD OFF)

# common options
add_library(rex-options INTERFACE)
target_compile_features(rex-options INTERFACE cxx_std_17)

target_compile_definitions(rex-options INTERFACE
	$<$<PLATFORM_ID:Windows>:REX_OS_WINDOWS=1>
	$<$<PLATFORM_ID:Linux>:REX_OS_LINUX=1>
	$<$<PLATFORM_ID:Emscripten>:REX_OS_WASM=1>
	$<$<CXX_COMPILER_ID:MSVC>:REX_COMPILER_MSVC=1>
	$<$<CXX_COMPILER_ID:GNU>:REX_COMPILER_GNU=1>
	$<$<CXX_COMPILER_ID:Clang>:REX_COMPILER_CLANG=1>
	$<$<CXX_COMPILER_ID:AppleClang>:REX_COMPILER_CLANG=1>
)

# TODO: warnings doesn't work with WASM
if (NOT EMSCRIPTEN)
	target_compile_options(rex-options INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Werror -Weverything>
		$<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>: /WX>
	)
endif()

if (EMSCRIPTEN)
	target_link_options(rex-options INTERFACE
		"SHELL:-o2"
		"SHELL:-s ALLOW_MEMORY_GROWTH=1"
		"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/data@data"
	)
endif()

if (REX_HOT_RELOAD)
	target_compile_definitions(rex-options INTERFACE -DREX_HOT_RELOAD=1)
endif()

add_subdirectory(rex-core)
add_subdirectory(rex-math)
add_subdirectory(rex-raster)
add_subdirectory(rex-viewer)

include(CTest)
add_subdirectory(rex-utests)
add_subdirectory(rex-scratch)

add_custom_target(rex-deploy-data ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/
	COMMENT "[rex-deploy]: deploy rex data."
)

if (EMSCRIPTEN)
	add_custom_target(rex-deploy-html ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/rex-core/src/wasm/index.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
		COMMENT "[rex-deploy]: deploy rex index.html."
	)

	install(
		FILES
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rex-viewer.wasm
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rex-viewer.js
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rex-viewer.data
			${CMAKE_SOURCE_DIR}/LICENSE
		DESTINATION ".")
elseif(REX_HOT_RELOAD)
	install(TARGETS rex-viewer rex-raster RUNTIME DESTINATION ".")
else()
	install(TARGETS rex-viewer RUNTIME DESTINATION ".")
endif()

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data DESTINATION ".")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
include(CPack)
