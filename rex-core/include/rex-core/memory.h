#pragma once

#include "rex-core/exports.h"
#include "rex-core/types.h"

namespace rc
{
	struct IAllocator
	{
		virtual ~IAllocator() = default;
		virtual void* alloc(sz size, const char* file, const char* function, i32 line) = 0;
		virtual void dealloc(void* ptr) = 0;
	};

	using Allocator = IAllocator*;

	struct Frame_Allocator: IAllocator
	{
		struct Frame_Node* head;
		sz peak_size;
		sz frame_size;

		~Frame_Allocator() override;
		void* alloc(sz size, const char* file, const char* function, i32 line) override;
		void dealloc(void* ptr) override;

		void clear();
	};

	REX_CORE_EXPORT Allocator rex_allocator();
	REX_CORE_EXPORT Frame_Allocator* frame_allocator();

	inline static void*
	_alloc_zeroed(Allocator allocator, sz size, const char* file, const char* function, i32 line)
	{
		auto ptr = (u8*)allocator->alloc(size, file, function, line);
		for (sz i = 0; i < size; ++i)
			ptr[i] = 0;
		return ptr;
	}
}

#define rex_alloc_from(allocator, size) allocator->alloc(size, __FILE__, __FUNCTION__, __LINE__)
#define rex_dealloc_from(allocator, ptr) allocator->dealloc(ptr)
#define rex_alloc_T_from(allocator, T) (T*)rex_alloc_from(allocator, sizeof(T))
#define rex_alloc_N_from(allocator, T, count) (T*)rex_alloc_from(allocator, sizeof(T) * count)
#define rex_alloc_zeroed_from(allocator, size) rc::_alloc_zeroed(allocator, size, __FILE__, __FUNCTION__, __LINE__)
#define rex_alloc_zeroed_T_from(allocator, T) (T*)rex_alloc_zeroed_from(allocator, sizeof(T))
#define rex_alloc_zeroed_N_from(allocator, T, count) (T*)rex_alloc_zeroed_from(allocator, sizeof(T) * count)

#define rex_alloc(size) rex_alloc_from(rc::rex_allocator(), size)
#define rex_dealloc(ptr) rex_dealloc_from(rc::rex_allocator(), ptr)
#define rex_alloc_T(T) rex_alloc_T_from(rc::rex_allocator(), T)
#define rex_alloc_N(T, count) rex_alloc_N_from(rc::rex_allocator(), T, count)
#define rex_alloc_zeroed(size) rex_alloc_zeroed_from(rc::rex_allocator(), size)
#define rex_alloc_zeroed_T(T) rex_alloc_zeroed_T_from(rc::rex_allocator(), T);
#define rex_alloc_zeroed_N(T, count) rex_alloc_zeroed_N_from(rc::rex_allocator(), T, count)
