add_executable(rex-utests
	"src/doctest.h"
	"src/main.cpp"
	"src/utests_core_memory.cpp"
	"src/utests_core_str.cpp"
	"src/utests_core_vec.cpp"
	"src/utests_math.cpp"
	"src/utests_math_types.cpp"
	"src/utests_math_vec2.cpp"
	"src/utests_math_vec3.cpp"
)

target_link_libraries(rex-utests PRIVATE rex-options rex-core rex-math)

add_test(NAME rex-unit-tests COMMAND rex-utests)
