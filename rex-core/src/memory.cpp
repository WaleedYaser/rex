#include "rex-core/memory.h"
#include "rex-core/assert.h"
#include "rex-core/log.h"

#include <stdlib.h>

// TODO: implement malloc http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
namespace rc
{
	static constexpr sz FRAME_ALLOCATOR_INITIAL_CAPACITY = 256 * 1024 * 1024;

	struct Rex_Allocator: IAllocator
	{
		struct Node
		{
			const char* file;
			const char* function;
			sz line;
			sz size;
			Node* next;
			Node* prev;
		};

		i64 leaked;
		Node* top;

		void*
		alloc(sz size, const char* file, const char* function, i32 line) override
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

			leaked += size;

			return node + 1;
		}

		void
		dealloc(void* ptr) override
		{
			if (ptr == nullptr)
				return;

			auto node = (Node*)ptr - 1;
			leaked -= node->size;
			rex_assert_msg(leaked >= 0, "leaked memory can't be negative");

			if (node == top)
				top = node->prev;
			if (node->next)
				node->next->prev = node->prev;
			if (node->prev)
				node->prev->next = node->next;

			::free(node);
		}

		~Rex_Allocator() override
		{
			if (leaked > 0)
			{
				rex_log_warn("Total leaked memory: %lld bytes", leaked);
				for (auto it = top; it != nullptr; it = it->prev)
					rex_log_warn("\t(%lld bytes) at %s(%lld): %s", it->size, it->file, it->line, it->function);
			}
		}
	};

	struct Frame_Node
	{
		u64 size;
		u64 capacity;
		Frame_Node* next;
	};

	void*
	Frame_Allocator::alloc(sz size, const char*, const char*, i32)
	{
		frame_size += size;
		peak_size = frame_size > peak_size ? frame_size : peak_size;

		if (head->size + size <= head->capacity)
		{
			auto ptr = (u8*)(head + 1) + head->size;
			head->size += size;
			return ptr;
		}
		else
		{
			auto capacity = size > head->capacity ? size : head->capacity;
			auto node = (Frame_Node*)rex_alloc(capacity + sizeof(Frame_Node));
			rex_assert(node);
			node->capacity = capacity;
			node->size = size;
			node->next = head;
			head = node;
			return node + 1;
		}
	}

	void
	Frame_Allocator::dealloc(void*)
	{
		// do nothing
	}

	void
	Frame_Allocator::clear()
	{
		if (peak_size > head->capacity)
		{
			auto it = head;
			while (it)
			{
				auto next = it->next;
				rex_dealloc(it);
				it = next;
			}

			head = (Frame_Node*)rex_alloc(peak_size + sizeof(Frame_Node));
			rex_assert(head);
			head->capacity = peak_size;
			head->next = nullptr;
		}

		head->size = 0;
		frame_size = 0;
	}

	Frame_Allocator::~Frame_Allocator()
	{
		clear();
		rex_log_info("Frame allocator initial capacity: %lld bytes and peak: %lld bytes",
			FRAME_ALLOCATOR_INITIAL_CAPACITY, peak_size);
		rex_dealloc(head);
	}

	struct _Memory
	{
		Rex_Allocator rex;
		Frame_Allocator frame;
	};

	inline static _Memory*
	_memory()
	{
		static _Memory self = {};
		return &self;
	}

	Allocator
	rex_allocator()
	{
		return &_memory()->rex;
	}

	Frame_Allocator*
	frame_allocator()
	{
		auto self = &_memory()->frame;
		if (self->head == nullptr)
		{
			self->head = (Frame_Node*)rex_alloc(FRAME_ALLOCATOR_INITIAL_CAPACITY + sizeof(Frame_Node));
			rex_assert(self->head);
			self->head->capacity = FRAME_ALLOCATOR_INITIAL_CAPACITY;
			self->head->size = 0;
			self->head->next = nullptr;
		}
		return self;
	}
}
