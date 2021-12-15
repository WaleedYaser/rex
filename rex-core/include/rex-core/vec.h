#pragma once

#include "rex-core/memory.h"
#include "rex-core/assert.h"

#include <initializer_list>
#include <type_traits>

namespace rc
{
	template <typename T>
	struct Vec
	{
		T* ptr;
		sz count;
		sz capacity;
		Allocator allocator;

		T& operator[](sz index) { return ptr[index]; }
		const T& operator[](sz index) const {return ptr[index]; }
	};

	template <typename T>
	inline static Vec<T>
	vec_init(Allocator allocator = rex_allocator())
	{
		Vec<T> self = {};
		self.allocator = allocator;
		return self;
	}

	template <typename T>
	inline static void
	vec_deinit(Vec<T>& self)
	{
		if (self.allocator)
			rex_dealloc_from(self.allocator, self.ptr);
		self = {};
	}

	template <typename T>
	inline static Vec<T>
	vec_with_capacity(sz capacity, Allocator allocator = rex_allocator())
	{
		Vec<T> self = {};
		self.allocator = allocator;
		self.ptr = rex_alloc_N_from(allocator, T, capacity);
		rex_assert(self.ptr);
		self.capacity = capacity;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_with_count(sz count, Allocator allocator = rex_allocator())
	{
		auto self = vec_with_capacity<T>(count, allocator);
		self.count = count;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_from(const T* first, const T* last, Allocator allocator = rex_allocator())
	{
		auto self = vec_with_capacity<T>(last - first, allocator);
		for (const T* it = first; it != last; ++it)
			self.ptr[self.count++] = *it;
		return self;
	}

	template <typename T>
	inline static Vec<T>
	vec_from(std::initializer_list<T> values, Allocator allocator = rex_allocator())
	{
		return vec_from(values.begin(), values.end(), allocator);
	}

	template <typename T>
	inline static Vec<T>
	vec_copy(Vec<T>& self, Allocator allocator = rex_allocator())
	{
		auto other = vec_with_capacity<T>(self.count, allocator);
		for (sz i = 0; i < self.count; ++i)
			other[other.count++] = self[i];
		return other;
	}

	template <typename T>
	inline static Vec<T>
	clone(Vec<T>& self, Allocator allocator = rex_allocator())
	{
		if constexpr (std::is_compound_v<T>)
		{
			auto other = vec_with_capacity<T>(self.count, allocator);
			for (sz i = 0; i < self.count; ++i)
				other[other.count++] = clone(self[i], allocator);
			return other;
		}
		else
		{
			return vec_copy(self, allocator);
		}
	}

	template <typename T>
	inline static void
	destroy(Vec<T>& self)
	{
		if constexpr (std::is_compound_v<T>)
		{
			for (sz i = 0; i < self.count; ++i)
				destroy(self[i]);
		}
		vec_deinit(self);
	}

	template <typename T>
	inline static void
	vec_clear(Vec<T>& self)
	{
		self.count = 0;
	}

	template <typename T>
	inline static void
	vec_reserve(Vec<T>& self, sz additional_capacity)
	{
		self.capacity += additional_capacity;
		auto old_ptr = self.ptr;
		self.ptr = rex_alloc_N_from(self.allocator, T, self.capacity);
		rex_assert(self.ptr);
		for (sz i = 0; i < self.count; ++i)
			self[i] = old_ptr[i];
		rex_dealloc_from(self.allocator, old_ptr);
	}

	// TODO: add unittest
	template <typename T>
	inline static void
	vec_resize(Vec<T>& self, sz count)
	{
		if (count > self.capacity)
			vec_reserve(self, count - self.capacity);
		self.count = count;
	}

	template <typename T, typename R>
	inline static void
	vec_fill(Vec<T>& self, const R& value)
	{
		for (sz i = 0; i < self.count; ++i)
			self[i] = value;
	}

	template <typename T, typename R>
	inline static void
	vec_push(Vec<T>& self, const R& value)
	{
		if (self.count == self.capacity)
			vec_reserve(self, self.capacity > 1 ? self.capacity / 2 : 8);

		self[self.count++] = value;
	}

	template <typename T, typename R>
	inline static void
	vec_append(Vec<T>& self, const R* first, const R* last)
	{
		auto count = last - first;
		rex_assert(count >= 0);

		if (self.capacity < (self.count + count))
			vec_reserve(self, count);

		for (sz i = 0; i < (sz)count; ++i)
			self[self.count + i] = first[i];
		self.count += count;
	}

	template <typename T, typename R>
	inline static void
	vec_append(Vec<T>& self, const Vec<R>& other)
	{
		vec_append(self, other.ptr, other.ptr + other.count);
	}

	template <typename T>
	inline static T&
	vec_top(Vec<T>& self)
	{
		rex_assert(self.count > 0);
		return self[self.count - 1];
	}

	template <typename T>
	inline static T
	vec_pop(Vec<T>& self)
	{
		rex_assert(self.count > 0);
		return self[--self.count];
	}

	template <typename T> T* begin(Vec<T>& self) { return self.ptr; }
	template <typename T> T* end(Vec<T>& self) { return self.ptr + self.count; }
	template <typename T> const T* begin(const Vec<T>& self) { return self.ptr; }
	template <typename T> const T* end(const Vec<T>& self) { return self.ptr + self.count; }
}
