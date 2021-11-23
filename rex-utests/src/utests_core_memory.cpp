#include <rex-core/memory.h>

#include "doctest.h"

TEST_CASE("[rex-core]: memory")
{
	SUBCASE("default allocator")
	{
		auto allocator = rex::core::default_allocator();

		void* ptr = allocator->alloc(1024);
		CHECK(ptr != nullptr);
		allocator->free(ptr);
	}

	SUBCASE("alloc")
	{
		void* ptr = rex::core::alloc(1024);
		CHECK(ptr != nullptr);
		rex::core::free(ptr);
	}

	SUBCASE("alloc zeroed")
	{
		u64 size = 100;
		void* ptr = rex::core::alloc_zeroed(size);
		for (u64 i = 0; i < size; ++i)
			CHECK(((u8*)ptr)[i] == 0);
		rex::core::free(ptr);
	}

	SUBCASE("alloc type")
	{
		struct S { u32 x, y; };

		S* ptr = rex::core::alloc<S>();
		CHECK(ptr != nullptr);
		rex::core::free(ptr);
	}

	SUBCASE("alloc zeroed type")
	{
		struct S { u32 x, y; };

		S* ptr = rex::core::alloc_zeroed<S>();
		CHECK(ptr->x == 0);
		CHECK(ptr->y == 0);
		rex::core::free(ptr);
	}
}
