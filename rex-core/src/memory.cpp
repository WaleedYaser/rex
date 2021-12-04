#include "rex-core/memory.h"
#include "rex-core/assert.h"
#include "rex-core/log.h"

#include <stdlib.h>
#include <atomic>

// TODO: implement malloc http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
namespace rex::core
{
	struct Node
	{
		const char* file;
		const char* function;
		u64 line;
		u64 size;
		Node* next;
		Node* prev;
	};

	struct Rex_Allocator: IAllocator
	{
		std::atomic<i64> leaked = 0;
		Node* top = nullptr;

		void*
		alloc(u64 size, const char* file, const char* function, u64 line) override
		{
			auto node = (Node*)::malloc(size + sizeof(Node));
			rex_assert(node);

			node->file = file;
			node->function = function;
			node->line = line;
			node->size = size;
			node->next = nullptr;
			node->prev = top;

			if (top)
				top->next = node;
			top = node;

			leaked.fetch_add(size);

			return node + 1;
		}

		void
		dealloc(void* ptr) override
		{
			if (ptr == nullptr)
				return;

			auto node = (Node*)ptr - 1;
			leaked.fetch_sub(node->size);
			rex_assert_msg(leaked >= 0, "leaked memory can't be negative");

			if (node == top)
				top = node->prev;
			if (node->next)
				node->next->prev = node->prev;
			if (node->prev)
				node->prev->next = node->next;

			::free(node);
		}

		~Rex_Allocator()
		{
			if (leaked.load() > 0)
			{
				rex_log_warn("Total leaked memory: %lld", leaked.load());
				for (auto it = top; it != nullptr; it = it->prev)
					rex_log_warn("(%lld bytes) at %s(%lld): %s", it->size, it->file, it->line, it->function);
			}
		}
	};

	Allocator
	rex_allocator()
	{
		static Rex_Allocator self = {};
		return &self;
	}
}
