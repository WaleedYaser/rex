#pragma once

#include "rex-core/defines.h"
#include "rex-core/types.h"

namespace rex::core
{
	// interface that each allocator needs to implement.
	struct Allocator
	{
		virtual ~Allocator() = default;
		// returns a valid pointer on success, nullptr on failure.
		//  size: size of the required memory in bytes.
		virtual void* alloc(u64 size) = 0;
		// frees the given pointer, do nothing if the ptr is nullptr.
		virtual void free(void* ptr) = 0;
	};

	// default allocator to use if you don't need any custom allocator.
	REX_API Allocator* default_allocator();

	// allocate the required size using the given allocator.
	//  size: size of the required memory in bytes.
	//  allocator: any allocator that implements 'Allocator' interface.
	inline static void*
	alloc(u64 size, Allocator* allocator = default_allocator())
	{
		return allocator->alloc(size);
	}

	// frees the pointer using the given allocator.
	//  ptr: pointer to free, the pointer must have been allocated using the 'allocator'.
	//  allocator: any allocator that implements 'Allocator' interface.
	inline static void
	free(void* ptr, Allocator* allocator = default_allocator())
	{
		allocator->free(ptr);
	}

	// allocate the required size using the given allocator and zero it.
	//  size: size of the required memory in bytes.
	//  allocator: any allocator that implements 'Allocator' interface.
	inline static void*
	alloc_zeroed(u64 size, Allocator* allocator = default_allocator())
	{
		u8* ptr = (u8*)alloc(size, allocator);
		for (u64 i = 0; i < size; ++i)
			ptr[i] = 0;
		return ptr;
	}

	// allocate an instance of 'T' using the given allocator.
	//  allocator: any allocator that implements 'Allocator' interface.
	template <typename T>
	inline static T*
	alloc(Allocator* allocator = default_allocator())
	{
		return (T*)alloc(sizeof(T), allocator);
	}

	// allocate an instance of 'T' using the given allocator and zero it.
	//  allocator: any allocator that implements 'Allocator' interface.
	template <typename T>
	inline static T*
	alloc_zeroed(Allocator* allocator = default_allocator())
	{
		return (T*)alloc_zeroed(sizeof(T), allocator);
	}
}
