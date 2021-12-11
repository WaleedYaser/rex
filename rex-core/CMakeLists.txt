add_library(rex-core)

include(GenerateExportHeader)
generate_export_header(rex-core
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/rex-core/exports.h
)

target_sources(rex-core PRIVATE
	"include/rex-core/api.h"
	"include/rex-core/assert.h"
	"include/rex-core/console.h"
	"include/rex-core/defer.h"
	"include/rex-core/file.h"
	"include/rex-core/log.h"
	"include/rex-core/memory.h"
	"include/rex-core/time.h"
	"include/rex-core/types.h"
	"include/rex-core/vec.h"
	"include/rex-core/window.h"
	"include/rex-core/path.h"

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
)

target_include_directories(rex-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(rex-core PRIVATE
	$<$<CXX_COMPILER_ID:GNU>: xcb xcb-keysyms xcb-image>
	# $<$<CXX_COMPILER_ID:MSVC>: Winmm>
)

target_compile_options(rex-core
	PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Werror -Weverything>
		$<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>: /WX>
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>: /utf-8>
)

target_compile_definitions(rex-core PUBLIC
	$<$<PLATFORM_ID:Windows>:REX_OS_WINDOWS=1>
	$<$<PLATFORM_ID:Linux>:REX_OS_LINUX=1>
	$<$<CXX_COMPILER_ID:MSVC>:REX_COMPILER_MSVC=1>
	$<$<CXX_COMPILER_ID:GNU>:REX_COMPILER_GNU=1>
	$<$<CXX_COMPILER_ID:Clang>:REX_COMPILER_CLANG=1>
	$<$<CXX_COMPILER_ID:AppleClang>:REX_COMPILER_CLANG=1>
)
