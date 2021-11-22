#include "rex-core/memory.h"

#include <stdlib.h>

// TODO: implement malloc http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
namespace rex::core
{
	struct Default_Allocator: Allocator
	{
		void*
		alloc(u64 size) override
		{
			return ::malloc(size);
		}

		void
		free(void* old_ptr) override
		{
			::free(old_ptr);
		}
	};

	Allocator*
	default_allocator()
	{
		static Default_Allocator self = {};
		return &self;
	}
}
