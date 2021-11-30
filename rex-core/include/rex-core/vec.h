#pragma once

#include "rex-core/memory.h"
#include "rex-core/assert.h"

#include <initializer_list>

namespace rex::core
{
	// dynamic array that can grow at runtime.
	template <typename T>
	struct Vec
	{
		T* ptr;
		i64 count;
		i64 capacity;
		Allocator* allocator;

		inline T& operator[](i64 index) { return ptr[index]; }
		inline const T& operator[](i64 index) const { return ptr[index]; }
	};

	template <typename T>
	inline static Vec<T>
	vec_new(Allocator* allocator = default_allocator())
	{
		Vec<T> self = {};
		self.allocator = allocator;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_with_capacity(i64 capacity, Allocator* allocator = default_allocator())
	{
		Vec<T> self = {};
		self.allocator = allocator;
		self.ptr = (T*)self.allocator->alloc(capacity * sizeof(T));
		rex_assert(self.ptr);
		self.capacity = capacity;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_with_count(i64 count, Allocator* allocator = default_allocator())
	{
		auto self = vec_with_capacity<T>(count, allocator);
		self.count = count;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_from(T* first, T* last, Allocator* allocator = default_allocator())
	{
		auto self = vec_with_capacity(last - first, allocator);
		for (T* it = first; it != last; ++it)
			self[self.count++] = *it;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_from(std::initializer_list<T> values, Allocator* allocator = default_allocator())
	{
		return vec_from(values.begin(), values.end(), allocator);
	}

	template <typename T>
	inline static Vec<T>
	vec_copy(Vec<T>& self, Allocator* allocator = default_allocator())
	{
		auto other = vec_with_capacity(self.count, allocator);
		for (i64 i = 0; i < self.count; ++i)
			other[other.count++] = self[i];
		return other;
	}

	template <typename T>
	inline static void
	vec_free(Vec<T>& self)
	{
		self.allocator->free(self.ptr);
		self = {};
	}

	template <typename T>
	inline static void
	vec_clear(Vec<T>& self)
	{
		self.count = 0;
	}

	template <typename T, typename R>
	inline static void
	vec_fill(Vec<T>& self, const R& value)
	{
		for (i64 i = 0; i < self.count; ++i)
			self[i] = value;
	}

	template <typename T>
	inline static void
	vec_reserve(Vec<T>& self, i64 additional_capacity)
	{
		self.capacity += additional_capacity;
		auto old_ptr = self.ptr;
		self.ptr = (T*)self.allocator->alloc(self.capacity * sizeof(T));
		rex_assert(self.ptr);
		for (i64 i = 0; i < self.count; ++i)
			self[i] = old_ptr[i];
		self.allocator->free(old_ptr);
	}

	template <typename T>
	inline static void
	vec_push(Vec<T>& self, const T& value)
	{
		if (self.count == self.capacity)
			vec_reserve(self, self.capacity > 1 ? self.capacity / 2 : 8);

		self[self.count++] = value;
	}

	template <typename T>
	inline static T
	vec_pop(Vec<T>& self)
	{
		rex_assert(self.count > 0);
		return self[--self.count];
	}

	template <typename T>
	inline static T
	vec_top(Vec<T>& self)
	{
		rex_assert(self.count > 0);
		return self[self.count - 1];
	}

	template <typename T>
	inline static void
	vec_append(Vec<T>& self, const Vec<T>& other)
	{
		if (self.capacity < (self.count + other.count))
			vec_reserve(self, other.count);

		for (i64 i = 0; i < other.count; ++i)
			self[self.count + i] = other[i];
		self.count += other.count;
	}

	template <typename T>
	inline static T*
	begin(Vec<T>& self)
	{
		return self.ptr;
	}

	template <typename T>
	inline static const T*
	begin(const Vec<T>& self)
	{
		return self.ptr;
	}

	template <typename T>
	inline static T*
	end(Vec<T>& self)
	{
		return self.ptr + self.count;
	}

	template <typename T>
	inline static const T*
	end(const Vec<T>& self)
	{
		return self.ptr + self.count;
	}
}
