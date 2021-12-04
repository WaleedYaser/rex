#include "rex-core/memory.h"
#include "rex-core/assert.h"
#include "rex-core/log.h"

#include <stdlib.h>

// TODO: implement malloc http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
namespace rex::core
{
	struct Rex_Allocator: IAllocator
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

		i64 leaked;
		Node* top;

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
				rex_log_warn("Total leaked memory: %lld", leaked);
				for (auto it = top; it != nullptr; it = it->prev)
					rex_log_warn("(%lld bytes) at %s(%lld): %s", it->size, it->file, it->line, it->function);
			}
		}
	};

	static constexpr u64 initial_capacity = 256 * 1024 * 1024;

	struct Frame_Node
	{
		u64 size;
		u64 capacity;
		Frame_Node* next;
	};

	void*
	Frame_Allocator::alloc(u64 size, const char*, const char*, u64)
	{
		frame_size += size;
		peak_size = frame_size > peak_size ? frame_size : peak_size;

		if (head->size + size <= head->capacity)
		{
			auto ptr = (u8*)(head + 1) + head->size;
			head->size += size;
			return ptr;
		}
		else if (size <= head->capacity)
		{
			auto node = (Frame_Node*)rex_alloc(head->capacity + sizeof(Frame_Node));
			rex_assert(node);
			node->capacity = head->capacity;
			node->size = size;
			node->next = head;
			head = node;
			return node + 1;
		}
		else
		{
			auto node = (Frame_Node*)rex_alloc(size + sizeof(Frame_Node));
			rex_assert(node);
			node->capacity = size;
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
		rex_log_info("Frame allocator initial capacity: %lld bytes and peak: %lld bytes", initial_capacity, peak_size);
		rex_dealloc(head);
	}

	Allocator
	rex_allocator()
	{
		static Rex_Allocator self = {};
		return &self;
	}

	Frame_Allocator*
	frame_allocator()
	{
		// TODO: this line is to make sure rex_allocator is constructed before frame_allocator so destructors are called
		// in the correct order
		rex_allocator();

		static Frame_Allocator self = {};
		if (self.head == nullptr)
		{
			self.head = (Frame_Node*)rex_alloc(initial_capacity + sizeof(Frame_Node));
			rex_assert(self.head);
			self.head->capacity = initial_capacity;
			self.head->size = 0;
			self.head->next = nullptr;
		}
		return &self;
	}
}
