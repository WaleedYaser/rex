#include "rex-core/memory.h"
#include "rex-core/assert.h"
#include "rex-core/log.h"

#include <stdlib.h>
#include <atomic>

// TODO: implement malloc http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
namespace rex::core
{
	struct Default_Allocator: Allocator
	{
		std::atomic<i64> leaked;

		void*
		alloc(u64 size) override
		{
			auto ptr = (i64*)::malloc(size + sizeof(i64));
			rex_assert(ptr);
			*ptr++ = size;
			leaked.fetch_add(size);
			return ptr;
		}

		void
		free(void* old_ptr) override
		{
			if (old_ptr == nullptr)
				return;

			auto ptr = (i64*)old_ptr - 1;
			auto size = *ptr;
			leaked.fetch_sub(size);
			rex_assert_msg(leaked >= 0, "leaked memory can't be negative");
			::free(ptr);
		}

		~Default_Allocator()
		{
			if (leaked.load() > 0)
				rex_log_warn("Leaked memory: %lld", leaked.load());
		}
	};

	Allocator*
	default_allocator()
	{
		static Default_Allocator self = {};
		return &self;
	}
}
