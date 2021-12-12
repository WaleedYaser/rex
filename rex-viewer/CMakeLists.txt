add_executable(rex-viewer "src/main.cpp")

target_link_libraries(rex-viewer PRIVATE
	rex-core
	rex-raster
)

if (REX_HOT_RELOAD)
	target_compile_definitions(rex-viewer PRIVATE -DREX_HOT_RELOAD=1)
endif()

set_target_properties(rex-viewer
	PROPERTIES
	LINK_FLAGS
	"-s ALLOW_MEMORY_GROWTH=1"
)

# target_compile_options(rex-viewer PRIVATE
# 	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Werror -Weverything>
# 	$<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra>
# 	$<$<CXX_COMPILER_ID:MSVC>: /WX>
# )
