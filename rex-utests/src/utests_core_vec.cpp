#include <rex-core/vec.h>
#include <rex-core/defer.h>

#include "doctest.h"

struct Foo
{
	rc::Allocator allocator;
	rc::i32* ptr;
	rc::sz count;
};

inline static Foo
foo_new(rc::sz count, rc::Allocator allocator = rc::rex_allocator())
{
	Foo self = {};
	self.ptr = rex_alloc_zeroed_N_from(allocator, rc::i32, count);
	self.count = count;
	self.allocator = allocator;
	return self;
}

inline static void
foo_free(Foo& self)
{
	rex_dealloc_from(self.allocator, self.ptr);
	self = {};
}

inline static Foo
clone(const Foo& self, rc::Allocator allocator = rc::rex_allocator())
{
	auto other = self;
	other.ptr = rex_alloc_N_from(allocator, rc::i32, other.count);
	for (rc::sz i = 0; i < self.count; ++i)
		other.ptr[i] = self.ptr[i];
	other.allocator = allocator;
	return other;
}

inline static void
destroy(Foo& self)
{
	foo_free(self);
}

TEST_CASE("[rex-core]: vec")
{
	using namespace rc;

	SUBCASE("vec initialization")
	{
		auto v = vec_init<i32>();
		rex_defer(vec_deinit(v));

		CHECK(v.ptr == nullptr);
		CHECK(v.count == 0);
		CHECK(v.capacity == 0);
		CHECK(v.allocator != nullptr);
	}

	SUBCASE("vec initialization using allocator")
	{
		auto v = vec_init<i32>(frame_allocator());
		rex_defer(vec_deinit(v));

		CHECK(v.ptr == nullptr);
		CHECK(v.count == 0);
		CHECK(v.capacity == 0);
		CHECK(v.allocator != nullptr);
	}


	SUBCASE("vec with capacity")
	{
		auto v = vec_with_capacity<i32>(100);
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 0);
		CHECK(v.capacity == 100);
		CHECK(v.allocator != nullptr);
	}

	SUBCASE("vec with capacity using allocator")
	{
		auto v = vec_with_capacity<i32>(100, frame_allocator());
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 0);
		CHECK(v.capacity == 100);
		CHECK(v.allocator != nullptr);
	}

	SUBCASE("vec with count")
	{
		auto v = vec_with_count<i32>(100);
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 100);
		CHECK(v.capacity == 100);
		CHECK(v.allocator != nullptr);
	}

	SUBCASE("vec with count using allocator")
	{
		auto v = vec_with_count<i32>(100, frame_allocator());
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 100);
		CHECK(v.capacity == 100);
		CHECK(v.allocator != nullptr);
	}

	SUBCASE("vec from range")
	{
		i32 arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		i32 count = sizeof(arr) / sizeof(arr[0]);

		auto v = vec_from(arr, arr + count);
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == count);
		CHECK(v.capacity == count);
		CHECK(v.allocator != nullptr);

		for (sz i = 0; i < v.count; ++i)
			CHECK(v[i] == arr[i]);
	}

	SUBCASE("vec from range using allocator")
	{
		i32 arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		i32 count = sizeof(arr) / sizeof(arr[0]);

		auto v = vec_from(arr, arr + count, frame_allocator());
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == count);
		CHECK(v.capacity == count);
		CHECK(v.allocator != nullptr);

		for (sz i = 0; i < v.count; ++i)
			CHECK(v[i] == arr[i]);
	}


	SUBCASE("vec from initializer list")
	{
		auto v = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 10);
		CHECK(v.capacity == 10);
		CHECK(v.allocator != nullptr);

		for (i32 i = 0; i < 10; ++i)
			CHECK(v[i] == i);
	}

	SUBCASE("vec from initializer list using allocator")
	{
		auto v = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, frame_allocator());
		rex_defer(vec_deinit(v));

		CHECK(v.ptr != nullptr);
		CHECK(v.count == 10);
		CHECK(v.capacity == 10);
		CHECK(v.allocator != nullptr);

		for (i32 i = 0; i < 10; ++i)
			CHECK(v[i] == i);
	}


	SUBCASE("vec access operator")
	{
		auto v = vec_with_count<i32>(10);
		rex_defer(vec_deinit(v));

		for (i32 i = 0; i < 10; ++i)
			v[i] = i;

		for (i32 i = 0; i < 10; ++i)
			CHECK(v[i] == i);
	}

	SUBCASE("vec copy")
	{
		auto v1 = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v1));

		auto v2 = vec_copy(v1);
		rex_defer(vec_deinit(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
			CHECK(v1[i] == v2[i]);
	}

	SUBCASE("vec copy using allocator")
	{
		auto v1 = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v1));

		auto v2 = vec_copy(v1, frame_allocator());
		rex_defer(vec_deinit(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
			CHECK(v1[i] == v2[i]);
	}

	SUBCASE("vec clone simple")
	{
		auto v1 = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v1));

		auto v2 = clone(v1);
		rex_defer(vec_deinit(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
			CHECK(v1[i] == v2[i]);
	}

	SUBCASE("vec clone simple using allocator")
	{
		auto v1 = vec_from({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v1));

		auto v2 = clone(v1, frame_allocator());
		rex_defer(vec_deinit(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
			CHECK(v1[i] == v2[i]);
	}

	SUBCASE("vec clone/destroy complex")
	{
		auto v1 = vec_from<Foo>({
			foo_new(10),
			foo_new(15),
			foo_new(20),
			foo_new(25),
			foo_new(30),
		});
		rex_defer(destroy(v1));

		auto v2 = rc::clone(v1);
		rex_defer(destroy(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
		{
			CHECK(v1[i].ptr != v2[i].ptr);
			CHECK(v1[i].count == v2[i].count);
		}
	}

	SUBCASE("vec clone/destroy complex using allocator")
	{
		auto v1 = vec_from<Foo>({
			foo_new(10),
			foo_new(15),
			foo_new(20),
			foo_new(25),
			foo_new(30),
		});
		rex_defer(destroy(v1));

		auto v2 = rc::clone(v1, frame_allocator());
		rex_defer(destroy(v2));

		CHECK(v1.ptr != v2.ptr);
		CHECK(v1.count == v2.count);

		for (sz i = 0; i < v1.count; ++i)
		{
			CHECK(v1[i].ptr != v2[i].ptr);
			CHECK(v1[i].count == v2[i].count);
		}
	}

	SUBCASE("vec clear")
	{
		auto v = vec_with_count<i32>(100);
		rex_defer(vec_deinit(v));

		vec_clear(v);
		CHECK(v.count == 0);
		CHECK(v.capacity == 100);
	}

	SUBCASE("vec reserve")
	{
		auto v = vec_with_capacity<i32>(50);
		rex_defer(vec_deinit(v));

		vec_reserve(v, 100);
		CHECK(v.capacity == 150);
	}

	SUBCASE("vec fill")
	{
		auto v = vec_with_count<i32>(10);
		rex_defer(vec_deinit(v));
		vec_fill(v, 99);

		for (rc::sz i = 0; i < v.count; ++i)
			CHECK(v[i] == 99);
	}

	SUBCASE("vec push")
	{
		auto v = vec_init<i32>();
		rex_defer(vec_deinit(v));

		for (i32 i = 0; i < 100; ++i)
			vec_push(v, i);

		for (i32 i = 0; i < 100; ++i)
			CHECK(v[i] == i);
	}

	SUBCASE("vec append")
	{
		auto v1 = vec_from<i32>({0, 1, 2, 3, 4});
		rex_defer(vec_deinit(v1));

		auto v2 = vec_from<i32>({5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v2));

		vec_append(v1, v2);
		for (i32 i = 0; i < 10; ++i)
			CHECK(v1[i] == i);
	}

	SUBCASE("vec append range")
	{
		auto v1 = vec_from<i32>({0, 1, 2, 3, 4});
		rex_defer(vec_deinit(v1));

		i32 arr[] = {5, 6, 7, 8, 9};
		auto count = sizeof(arr) / sizeof(arr[0]);

		vec_append(v1, arr, arr + count);
		for (i32 i = 0; i < 10; ++i)
			CHECK(v1[i] == i);
	}

	SUBCASE("vec top/pop")
	{
		auto v = vec_from<i32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(v));

		for (i32 i = 9; i >= 0; --i)
		{
			CHECK(vec_top(v) == i);
			CHECK(vec_pop(v) == i);
		}
		CHECK(v.count == 0);
	}

	SUBCASE("vec ranged for")
	{
		auto values = vec_from<i32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(values));

		i32 i = 0;
		for (auto v: values)
			CHECK(v == i++);

		for (auto& v: values)
			v = 100;

		for (const auto& v: values)
			CHECK(v == 100);
	}

	SUBCASE("vec const ranged for")
	{
		auto values = vec_from<i32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		rex_defer(vec_deinit(values));

		const auto cvalues = values;

		i32 i = 0;
		for (auto v: cvalues)
			CHECK(v == i++);

		for (auto& v: values)
			v = 100;

		for (const auto& v: cvalues)
			CHECK(v == 100);
	}
}
