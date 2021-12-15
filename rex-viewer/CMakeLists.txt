add_executable(rex-viewer "src/main.cpp")

target_link_libraries(rex-viewer PRIVATE
	rex-options
	rex-core
	rex-raster
)
