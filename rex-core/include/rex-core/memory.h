#pragma once

#include "rex-core/exports.h"
#include "rex-core/types.h"

namespace rex::core
{
	struct IAllocator
	{
		virtual ~IAllocator() = default;
		virtual void* alloc(u64 size, const char* file, const char* func, u64 line) = 0;
		virtual void dealloc(void* ptr) = 0;
	};

	using Allocator = IAllocator*;

	struct Frame_Node;
	struct Frame_Allocator: IAllocator
	{
		Frame_Node* head;
		u64 peak_size;
		u64 frame_size;

		~Frame_Allocator() override;
		void* alloc(u64 size, const char* file, const char* func, u64 line) override;
		void dealloc(void* ptr) override;

		void clear();
	};

	REX_CORE_EXPORT Allocator rex_allocator();
	REX_CORE_EXPORT Frame_Allocator* frame_allocator();
}

#define rex_alloc_from(allocator, size) allocator->alloc(size, __FILE__, __FUNCTION__, __LINE__)
#define rex_dealloc_from(allocator, ptr) allocator->dealloc(ptr)

#define rex_alloc(size) rex_alloc_from(rex::core::rex_allocator(), size)
#define rex_dealloc(ptr) rex_dealloc_from(rex::core::rex_allocator(), ptr)
