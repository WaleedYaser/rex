#include <rex-core/memory.h>
#include <rex-core/defer.h>

#include "doctest.h"

TEST_CASE("[rex-core]: memory")
{
	SUBCASE("default alloc")
	{
		auto ptr = rex_alloc(100);
		rex_defer(rex_dealloc(ptr));
		CHECK(ptr != nullptr);
	}

	SUBCASE("default alloc type")
	{
		struct Foo { int x; int y; };
		auto ptr = rex_alloc_T(Foo);
		rex_defer(rex_dealloc(ptr));
		CHECK(ptr != nullptr);
	}

	SUBCASE("default alloc array")
	{
		auto ptr = rex_alloc_N(int, 100);
		rex_defer(rex_dealloc(ptr));
		CHECK(ptr != nullptr);
	}

	SUBCASE("default alloc zeroed")
	{
		struct Foo { int x; int y; };
		auto ptr = rex_alloc_zeroed_T(Foo);
		rex_defer(rex_dealloc(ptr));
		CHECK(ptr != nullptr);
		CHECK(ptr->x == 0);
		CHECK(ptr->y == 0);
	}

	SUBCASE("default alloc zeroed array")
	{
		auto ptr = rex_alloc_zeroed_N(int, 100);
		rex_defer(rex_dealloc(ptr));
		CHECK(ptr != nullptr);
		for (sz i = 0; i < 100; ++i)
			CHECK(ptr[i] == 0);
	}

	SUBCASE("frame alloc")
	{
		auto ptr = rex_alloc_from(rex::core::frame_allocator(), 100);
		rex_defer(rex_dealloc_from(rex::core::frame_allocator(), ptr));
		CHECK(ptr != nullptr);
	}

	SUBCASE("frame alloc type")
	{
		struct Foo { int x; int y; };
		auto ptr = rex_alloc_T_from(rex::core::frame_allocator(), Foo);
		CHECK(ptr != nullptr);
	}

	SUBCASE("frame alloc array")
	{
		auto ptr = rex_alloc_N_from(rex::core::frame_allocator(), int, 100);
		CHECK(ptr != nullptr);
	}

	SUBCASE("frame alloc zeroed")
	{
		struct Foo { int x; int y; };
		auto ptr = rex_alloc_zeroed_T_from(rex::core::frame_allocator(), Foo);
		CHECK(ptr != nullptr);
		CHECK(ptr->x == 0);
		CHECK(ptr->y == 0);
	}

	SUBCASE("frame alloc zeroed array")
	{
		auto ptr = rex_alloc_zeroed_N_from(rex::core::frame_allocator(), int, 100);
		CHECK(ptr != nullptr);
		for (sz i = 0; i < 100; ++i)
			CHECK(ptr[i] == 0);
	}
}
