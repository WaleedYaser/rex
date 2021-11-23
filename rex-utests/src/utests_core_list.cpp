#include <rex-core/list.h>

#include "doctest.h"

TEST_CASE("[rex-core]: list")
{
	using namespace rex::core;

	SUBCASE("list creation")
	{
		auto list = list_new<i32>();
		CHECK(list.ptr == nullptr);
		CHECK(list.count == 0);
		CHECK(list.capacity == 0);
		list_free(list);

		list = list_with_capacity<i32>(100);
		CHECK(list.ptr != nullptr);
		CHECK(list.count == 0);
		CHECK(list.capacity == 100);
		list_free(list);

		list = list_with_count<i32>(100);
		CHECK(list.ptr != nullptr);
		CHECK(list.count == 100);
		CHECK(list.capacity == 100);
		list_free(list);

		list = list_from({1, 2, 3, 4, 5});
		CHECK(list.ptr != nullptr);
		CHECK(list.count == 5);
		CHECK(list.capacity == 5);
		CHECK(list[0] == 1);
		CHECK(list[1] == 2);
		CHECK(list[2] == 3);
		CHECK(list[3] == 4);
		CHECK(list[4] == 5);
		list_free(list);
	}

	SUBCASE("list fill")
	{
		auto list = list_with_count<f32>(50);
		list_fill(list, 5.0f);
		for (u64 i = 0; i < list.count; ++i)
			CHECK(list[i] == 5.0f);
	}

	SUBCASE("list reserve ")
	{
		auto list = list_with_capacity<i32>(10);
		CHECK(list.ptr != nullptr);
		CHECK(list.capacity == 10);
		list_reserve(list, 20);
		CHECK(list.ptr != nullptr);
		CHECK(list.capacity == 30);
		list_free(list);
	}

	SUBCASE("list push/pop/top")
	{
		auto list = list_new<i32>();
		for (i32 i = 0; i < 100; ++i)
			list_push(list, i);
		CHECK(list.count == 100);

		for (u64 i = 0; i < list.count; ++i)
			CHECK(list[i] == i);

		CHECK(list_top(list) == 99);

		for (i32 i = 0; i < 100; ++i)
			CHECK(list_pop(list) == 99 - i);

		CHECK(list.count == 0);
		list_free(list);
	}

	SUBCASE("list append")
	{
		auto list1 = list_from({0, 1, 2, 3, 4});
		auto list2 = list_from({5, 6, 7, 8, 9});

		list_append(list1, list2);
		CHECK(list1.count == 10);

		for (i32 i = 0; i < (i32)list1.count; ++i)
			CHECK(list1[i] == i);

		list_free(list2);
		list_free(list1);
	}

	SUBCASE("list iterators")
	{
		auto list = list_from({0, 1, 2, 3, 4});
		CHECK(begin(list) == list.ptr);
		CHECK(end(list) == list.ptr + list.count);

		i32 i = 0;
		for (auto v: list)
			CHECK(v == i++);

		i = 0;
		for (const auto v: list)
			CHECK(v == i++);

		for (auto& v: list)
			v++;

		i = 1;
		for (const auto& v: list)
			CHECK(v == i++);

		list_free(list);
	}
}
