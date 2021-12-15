add_library(rex-core)

include(GenerateExportHeader)
generate_export_header(rex-core EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/rex-core/exports.h)

get_target_property(REX_CORE_DIR rex-core SOURCE_DIR)
configure_file(include/rex-core/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/rex-core/version.h)

target_sources(rex-core PRIVATE
	"include/rex-core/api.h"
	"include/rex-core/assert.h"
	"include/rex-core/console.h"
	"include/rex-core/defer.h"
	"include/rex-core/exports.h"
	"include/rex-core/file.h"
	"include/rex-core/log.h"
	"include/rex-core/memory.h"
	"include/rex-core/path.h"
	"include/rex-core/time.h"
	"include/rex-core/types.h"
	"include/rex-core/vec.h"
	"include/rex-core/version.h"
	"include/rex-core/window.h"

	"src/assert.cpp"
	"src/log.cpp"
	"src/memory.cpp"
	"src/str.cpp"

	"src/winos/api.cpp"
	"src/winos/console.cpp"
	"src/winos/file.cpp"
	"src/winos/path.cpp"
	"src/winos/time.cpp"
	"src/winos/window.cpp"

	"src/linux/api.cpp"
	"src/linux/console.cpp"
	"src/linux/file.cpp"
	"src/linux/path.cpp"
	"src/linux/time.cpp"
	"src/linux/window.cpp"

	"src/wasm/api.cpp"
	"src/wasm/console.cpp"
	"src/wasm/file.cpp"
	"src/wasm/path.cpp"
	"src/wasm/time.cpp"
	"src/wasm/window.cpp"
)

target_include_directories(rex-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rex-core PRIVATE
	rex-options
	$<$<PLATFORM_ID:Windows>:Winmm>
	$<$<PLATFORM_ID:Linux>:xcb>
	$<$<PLATFORM_ID:Linux>:xcb-keysyms>
	$<$<PLATFORM_ID:Linux>:xcb-image>
)

target_compile_options(rex-core PUBLIC $<$<CXX_COMPILER_ID:MSVC>: /utf-8>)
