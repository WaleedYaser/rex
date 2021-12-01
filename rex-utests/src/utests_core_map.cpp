#include <rex-core/map.h>
#include <rex-core/str.h>

#include "doctest.h"

TEST_CASE("[rex-core]: map")
{
	using namespace rex::core;

	auto map = map_new<Str, int>();
	CHECK(map.nodes.count == 0);
	CHECK(map.nodes.capacity == 0);
	CHECK(map.nodes.allocator != nullptr);
	map_free(map);

	map = map_from<Str, int>({
		{str_literal("apple"), 5},
		{str_literal("orange"), 10}
	});

	auto node = map_find(map, str_literal("apple"));
	CHECK(node != nullptr);
	CHECK(node->key == "apple");
	CHECK(node->value == 5);

	node = map_find(map, str_literal("orange"));
	CHECK(node != nullptr);
	CHECK(node->key == "orange");
	CHECK(node->value == 10);

	map_insert(map, str_literal("banana"), 20);
	node = map_find(map, str_literal("banana"));
	CHECK(node != nullptr);
	CHECK(node->key == "banana");
	CHECK(node->value == 20);

	map_free(map);
}
