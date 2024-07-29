add_library(rex-gfx)

include(GenerateExportHeader)
generate_export_header(rex-gfx EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/rex-gfx/exports.h)

target_sources(rex-gfx PRIVATE
	"include/rex-gfx/gfx.h"

	"src/dx12/gfx.cpp"
)

target_include_directories(rex-gfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rex-gfx PRIVATE
	rex-options
    rex-core
	$<$<PLATFORM_ID:Windows>:D3d12.lib>
	$<$<PLATFORM_ID:Windows>:DXGI.lib>
	$<$<PLATFORM_ID:Windows>:d3dcompiler.lib>

)
