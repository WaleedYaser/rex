#pragma once

#include "rex-core/memory.h"
#include "rex-core/assert.h"

#include <initializer_list>
#include <vector>

namespace rex::core
{
	// dynamic array that can grow at runtime
	template <typename T>
	struct List
	{
		T* ptr;
		u64 count;
		u64 capacity;
		Allocator* allocator;

		inline T& operator[](u64 index) { return ptr[index]; }
		inline const T& operator[](u64 index) const { return ptr[index]; }
	};

	template <typename T>
	inline static List<T>
	list_new(Allocator* allocator = default_allocator())
	{
		List<T> self = {};
		self.allocator = allocator;
		return self;
	}

	template <typename T>
	inline static List<T>
	list_with_capacity(u64 capacity, Allocator* allocator = default_allocator())
	{
		List<T> self = {};
		self.allocator = allocator;
		self.ptr = (T*)self.allocator->alloc(capacity * sizeof(T));
		rex_assert(self.ptr);
		self.capacity = capacity;
		return self;
	}

	template <typename T>
	inline static List<T>
	list_with_count(u64 count, Allocator* allocator = default_allocator())
	{
		auto self = list_with_capacity<T>(count, allocator);
		self.count = count;
		return self;
	}

	template <typename T>
	inline static List<T>
	list_from(std::initializer_list<T> values, Allocator* allocator = default_allocator())
	{
		auto self = list_with_count<T>(values.size(), allocator);
		u64 i = 0;
		for (const auto& v: values)
			self[i++] = v;
		return self;
	}

	template <typename T>
	inline static void
	list_free(List<T>& self)
	{
		self.allocator->free(self.ptr);
		self = {};
	}

	template <typename T>
	inline static void
	list_fill(List<T>& self, const T& value)
	{
		for (u64 i = 0; i < self.count; ++i)
			self[i] = value;
	}

	template <typename T>
	inline static void
	list_reserve(List<T>& self, u64 additional_capacity)
	{
		self.capacity += additional_capacity;
		auto old_ptr = self.ptr;
		self.ptr = (T*)self.allocator->alloc(self.capacity * sizeof(T));
		rex_assert(self.ptr);
		for (u64 i = 0; i < self.count; ++i)
			self[i] = old_ptr[i];
		self.allocator->free(old_ptr);
	}

	template <typename T>
	inline static void
	list_push(List<T>& self, const T& value)
	{
		if (self.count == self.capacity)
			list_reserve(self, self.capacity > 1 ? self.capacity / 2 : 8);

		self[self.count++] = value;
	}

	template <typename T>
	inline static T
	list_pop(List<T>& self)
	{
		rex_assert(self.count > 0);
		return self[--self.count];
	}

	template <typename T>
	inline static T
	list_top(List<T>& self)
	{
		rex_assert(self.count > 0);
		return self[self.count - 1];
	}

	template <typename T>
	inline static void
	list_append(List<T>& self, const List<T>& other)
	{
		if (self.capacity < (self.count + other.count))
			list_reserve(self, other.count);

		for (u64 i = 0; i < other.count; ++i)
			self[self.count + i] = other[i];
		self.count += other.count;
	}

	template <typename T>
	inline static T*
	begin(List<T>& self)
	{
		return self.ptr;
	}

	template <typename T>
	inline static const T*
	begin(const List<T>& self)
	{
		return self.ptr;
	}

	template <typename T>
	inline static T*
	end(List<T>& self)
	{
		return self.ptr + self.count;
	}

	template <typename T>
	inline static const T*
	end(const List<T>& self)
	{
		return self.ptr + self.count;
	}
}
