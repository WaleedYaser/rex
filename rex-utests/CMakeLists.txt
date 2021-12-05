add_executable(rex-utests
	src/doctest.h
	src/main.cpp
	src/utests_core_types.cpp
	src/utests_core_memory.cpp
	src/utests_core_vec.cpp
)

target_link_libraries(rex-utests PRIVATE rex-core)

target_compile_options(rex-utests PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Werror -Weverything>
	$<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra>
	$<$<CXX_COMPILER_ID:MSVC>: /WX>
)

add_test(NAME rex-unit-tests COMMAND rex-utests)
