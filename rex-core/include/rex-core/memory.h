#pragma once

#include "rex-core/defines.h"
#include "rex-core/types.h"

namespace rex::core
{
	struct Allocator
	{
		virtual void* alloc(u64 size) = 0;
		virtual void free(void* ptr) = 0;
	};

	REX_API Allocator* default_allocator();

	inline static void*
	alloc(u64 size, Allocator* allocator = default_allocator())
	{
		return allocator->alloc(size);
	}

	inline static void
	free(void* ptr, Allocator* allocator = default_allocator())
	{
		allocator->free(ptr);
	}

	inline static void*
	alloc_zeroed(u64 size, Allocator* allocator = default_allocator())
	{
		u8* ptr = (u8*)alloc(size, allocator);
		for (u64 i = 0; i < size; ++i)
			ptr[i] = 0;
		return ptr;
	}

	template <typename T>
	inline static T*
	alloc(Allocator* allocator = default_allocator())
	{
		return (T*)alloc(sizeof(T), allocator);
	}

	template <typename T>
	inline static T*
	alloc_zeroed(Allocator* allocator = default_allocator())
	{
		return (T*)alloc_zeroed(sizeof(T), allocator);
	}
}
