if (REX_HOT_RELOAD)
	add_library(rex-raster SHARED)
else()
	add_library(rex-raster)
endif()

include(GenerateExportHeader)
generate_export_header(rex-raster EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/rex-raster/exports.h)

target_sources(rex-raster PRIVATE
	"include/rex-raster/constants.h"
	"include/rex-raster/exports.h"
	"include/rex-raster/geometry.h"
	"include/rex-raster/gltf.h"
	"include/rex-raster/rex.h"
	"include/rex-raster/stb_image.h"
	"src/rex.cpp"
	"src/stb_image.cpp"
)

target_link_libraries(rex-raster PRIVATE rex-options rex-core)

target_include_directories(rex-raster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
