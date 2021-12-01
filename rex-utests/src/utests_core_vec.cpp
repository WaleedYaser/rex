#include <rex-core/vec.h>

#include "doctest.h"

TEST_CASE("[rex-core]: vec")
{
	using namespace rex::core;

	SUBCASE("vec creation")
	{
		auto vec = vec_new<i32>();
		CHECK(vec.ptr == nullptr);
		CHECK(vec.count == 0);
		CHECK(vec.capacity == 0);
		CHECK(vec.allocator != nullptr);
		vec_free(vec);

		vec = vec_with_capacity<i32>(100);
		CHECK(vec.ptr != nullptr);
		CHECK(vec.count == 0);
		CHECK(vec.capacity == 100);
		vec_free(vec);

		vec = vec_with_count<i32>(100);
		CHECK(vec.ptr != nullptr);
		CHECK(vec.count == 100);
		CHECK(vec.capacity == 100);
		vec_free(vec);

		vec = vec_from({1, 2, 3, 4, 5});
		CHECK(vec.ptr != nullptr);
		CHECK(vec.count == 5);
		CHECK(vec.capacity == 5);
		CHECK(vec[0] == 1);
		CHECK(vec[1] == 2);
		CHECK(vec[2] == 3);
		CHECK(vec[3] == 4);
		CHECK(vec[4] == 5);
		vec_free(vec);
	}

	SUBCASE("vec fill")
	{
		auto vec = vec_with_count<f32>(50);
		vec_fill(vec, 5.0f);
		for (i64 i = 0; i < vec.count; ++i)
			CHECK(vec[i] == 5.0f);
	}

	SUBCASE("vec reserve ")
	{
		auto vec = vec_with_capacity<i32>(10);
		CHECK(vec.ptr != nullptr);
		CHECK(vec.capacity == 10);
		vec_reserve(vec, 20);
		CHECK(vec.ptr != nullptr);
		CHECK(vec.capacity == 30);
		vec_free(vec);
	}

	SUBCASE("vec push/pop/top")
	{
		auto vec = vec_new<i32>();
		for (i32 i = 0; i < 100; ++i)
			vec_push(vec, i);
		CHECK(vec.count == 100);

		for (i64 i = 0; i < vec.count; ++i)
			CHECK(vec[i] == i);

		CHECK(vec_top(vec) == 99);

		for (i32 i = 0; i < 100; ++i)
			CHECK(vec_pop(vec) == 99 - i);

		CHECK(vec.count == 0);
		vec_free(vec);
	}

	SUBCASE("vec append")
	{
		auto vec1 = vec_from({0, 1, 2, 3, 4});
		auto vec2 = vec_from({5, 6, 7, 8, 9});

		vec_append(vec1, vec2);
		CHECK(vec1.count == 10);

		for (i32 i = 0; i < (i32)vec1.count; ++i)
			CHECK(vec1[i] == i);

		vec_free(vec2);
		vec_free(vec1);
	}

	SUBCASE("vec iterators")
	{
		auto vec = vec_from({0, 1, 2, 3, 4});
		CHECK(begin(vec) == vec.ptr);
		CHECK(end(vec) == vec.ptr + vec.count);

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

		vec_free(vec);
	}
}
