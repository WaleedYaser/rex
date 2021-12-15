add_executable(rex-utests
	"src/doctest.h"
	"src/main.cpp"
	"src/utests_core_memory.cpp"
	"src/utests_core_str.cpp"
	"src/utests_core_types.cpp"
	"src/utests_core_vec.cpp"
)

target_link_libraries(rex-utests PRIVATE rex-options rex-core)

add_test(NAME rex-unit-tests COMMAND rex-utests)
