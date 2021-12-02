#include <rex-core/vec.h>
#include <rex-core/defer.h>

#include "doctest.h"

TEST_CASE("[rex-core]: vec")
{
	using namespace rex::core;

	SUBCASE("vec creation")
	{
		{
			auto vec = Vec<i32>::init();
			rex_defer(vec.deinit());

			CHECK(vec.ptr == nullptr);
			CHECK(vec.count == 0);
			CHECK(vec.capacity == 0);
			CHECK(vec.allocator != nullptr);
		}

		{
			auto vec = Vec<i32>::with_capacity(100);
			rex_defer(vec.deinit());

			CHECK(vec.ptr != nullptr);
			CHECK(vec.count == 0);
			CHECK(vec.capacity == 100);
		}

		{
			auto vec = Vec<i32>::with_count(100);
			rex_defer(vec.deinit());

			CHECK(vec.ptr != nullptr);
			CHECK(vec.count == 100);
			CHECK(vec.capacity == 100);
		}

		{
			auto vec = Vec<i32>::from({1, 2, 3, 4, 5});
			rex_defer(vec.deinit());

			CHECK(vec.ptr != nullptr);
			CHECK(vec.count == 5);
			CHECK(vec.capacity == 5);
			CHECK(vec[0] == 1);
			CHECK(vec[1] == 2);
			CHECK(vec[2] == 3);
			CHECK(vec[3] == 4);
			CHECK(vec[4] == 5);
		}
	}

	SUBCASE("vec fill")
	{
		auto vec = Vec<f32>::with_count(50);
		rex_defer(vec.deinit());

		vec.fill(5.0f);
		for (i64 i = 0; i < vec.count; ++i)
			CHECK(vec[i] == 5.0f);
	}

	SUBCASE("vec reserve ")
	{
		auto vec = Vec<i32>::with_capacity(10);
		rex_defer(vec.deinit());

		CHECK(vec.ptr != nullptr);
		CHECK(vec.capacity == 10);
		vec.reserve(20);
		CHECK(vec.ptr != nullptr);
		CHECK(vec.capacity == 30);
	}

	SUBCASE("vec push/pop/top")
	{
		auto vec = Vec<i32>::init();
		rex_defer(vec.deinit());

		for (i32 i = 0; i < 100; ++i)
			vec.push(i);
		CHECK(vec.count == 100);

		for (i64 i = 0; i < vec.count; ++i)
			CHECK(vec[i] == i);

		CHECK(vec.top() == 99);

		for (i32 i = 0; i < 100; ++i)
			CHECK(vec.pop() == 99 - i);

		CHECK(vec.count == 0);
	}

	SUBCASE("vec append")
	{
		auto vec1 = Vec<i32>::from({0, 1, 2, 3, 4});
		rex_defer(vec1.deinit());

		auto vec2 = Vec<i32>::from({5, 6, 7, 8, 9});
		rex_defer(vec2.deinit());

		vec1.append(vec2);
		CHECK(vec1.count == 10);

		for (i32 i = 0; i < (i32)vec1.count; ++i)
			CHECK(vec1[i] == i);
	}

	SUBCASE("vec iterators")
	{
		auto vec = Vec<i32>::from({0, 1, 2, 3, 4});
		rex_defer(vec.deinit());

		CHECK(vec.begin() == vec.ptr);
		CHECK(vec.end() == vec.ptr + vec.count);

		i32 i = 0;
		for (auto v: vec)
			CHECK(v == i++);

		i = 0;
		for (const auto v: vec)
			CHECK(v == i++);

		for (auto& v: vec)
			v++;

		i = 1;
		for (const auto& v: vec)
			CHECK(v == i++);
	}

	SUBCASE("vec destroy")
	{
		auto v = Vec<Vec<i32>>::init();
		rex_defer(v.destroy());

		v.push(Vec<i32>::from({1, 2, 3}));
		v.push(Vec<i32>::from({4, 5, 6}));
	}
}
