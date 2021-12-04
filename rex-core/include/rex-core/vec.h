#pragma once

#include "rex-core/memory.h"
#include "rex-core/assert.h"

#include <initializer_list>
#include <type_traits>

namespace rex::core
{
	template <typename T>
	struct Vec
	{
		T* ptr;
		i64 count;
		i64 capacity;
		Allocator allocator;

		static Vec init(Allocator allocator = rex_allocator());
		static Vec with_capacity(i64 capacity, Allocator allocator = rex_allocator());
		static Vec with_count(i64 count, Allocator allocator = rex_allocator());
		static Vec from(const T* first, const T* last, Allocator allocator = rex_allocator());
		static Vec from(std::initializer_list<T> values, Allocator allocator = rex_allocator());

		Vec copy(Allocator allocator = rex_allocator());
		Vec clone(Allocator allocator = rex_allocator());
		void deinit();
		void destroy();

		void clear();
		void reserve(i64 additional_capacity);

		template <typename R> void fill(const R& value);
		template <typename R> void push(const R& value);
		template <typename R> void append(const Vec<R>& other);

		T pop();
		T& top();

		T* begin();
		T* end();
		const T* begin() const;
		const T* end() const;

		T& operator[](i64 index);
		const T& operator[](i64 index) const;
	};

	template <typename T>
	Vec<T> Vec<T>::init(Allocator allocator)
	{
		Vec<T> self = {};
		self.allocator = allocator;
		return self;
	}

	template <typename T>
	Vec<T> Vec<T>::with_capacity(i64 capacity, Allocator allocator)
	{
		Vec<T> self = {};
		self.allocator = allocator;
		self.ptr = (T*)rex_alloc_from(allocator, capacity * sizeof(T));
		rex_assert(self.ptr);
		self.capacity = capacity;
		return self;
	}

	template <typename T>
	Vec<T> Vec<T>::with_count(i64 count, Allocator allocator)
	{
		auto self = Vec<T>::with_capacity(count, allocator);
		self.count = count;
		return self;
	}

	template <typename T>
	Vec<T> Vec<T>::from(const T* first, const T* last, Allocator allocator)
	{
		auto self = Vec<T>::with_capacity(last - first, allocator);
		for (const T* it = first; it != last; ++it)
			self.ptr[self.count++] = *it;
		return self;
	}

	template <typename T>
	Vec<T> Vec<T>::from(std::initializer_list<T> values, Allocator allocator)
	{
		return Vec<T>::from(values.begin(), values.end(), allocator);
	}

	template <typename T>
	Vec<T> Vec<T>::copy(Allocator allocator)
	{
		auto other = Vec<T>::with_capacity(count, allocator);
		for (i64 i = 0; i < count; ++i)
			other[other.count++] = self[i];
		return other;
	}

	template <typename T>
	Vec<T> Vec<T>::clone(Allocator allocator)
	{
		if constexpr (std::is_compound_v<T>)
		{
			auto other = Vec<T>::with_capacity(count, allocator);
			for (i64 i = 0; i < count; ++i)
				other[other.count++] = self[i].clone();
			return other;
		}
		else
		{
			return copy(allocator);
		}
	}

	template <typename T>
	void Vec<T>::deinit()
	{
		rex_dealloc_from(allocator, ptr);
		*this = {};
	}

	template <typename T>
	void Vec<T>::destroy()
	{
		if constexpr (std::is_compound_v<T>)
		{
			for (i64 i = 0; i < count; ++i)
				ptr[i].destroy();
		}
		deinit();
	}

	template <typename T>
	void Vec<T>::clear()
	{
		count = 0;
	}

	template <typename T>
	void Vec<T>::reserve(i64 additional_capacity)
	{
		capacity += additional_capacity;
		auto old_ptr = ptr;
		ptr = (T*)rex_alloc_from(allocator, capacity*sizeof(T));
		rex_assert(ptr);
		for (i64 i = 0; i < count; ++i)
			ptr[i] = old_ptr[i];
		rex_dealloc_from(allocator, old_ptr);
	}

	template <typename T>
	template<typename R>
	void Vec<T>::fill(const R& value)
	{
		for (i64 i = 0; i < count; ++i)
			ptr[i] = value;
	}

	template <typename T>
	template <typename R>
	void Vec<T>::push(const R& value)
	{
		if (count == capacity)
			reserve(capacity > 1 ? capacity / 2 : 8);

		ptr[count++] = value;
	}

	template <typename T>
	template <typename R>
	void Vec<T>::append(const Vec<R>& other)
	{
		if (capacity < (count + other.count))
			reserve(other.count);

		for (i64 i = 0; i < other.count; ++i)
			ptr[count + i] = other[i];
		count += other.count;
	}


	template <typename T>
	T Vec<T>::pop()
	{
		rex_assert(count > 0);
		return ptr[--count];
	}

	template <typename T>
	T& Vec<T>::top()
	{
		rex_assert(count > 0);
		return ptr[count - 1];
	}

	template <typename T>
	T* Vec<T>::begin()
	{
		return ptr;
	}

	template <typename T>
	T* Vec<T>::end()
	{
		return ptr + count;
	}

	template <typename T>
	const T* Vec<T>::begin() const
	{
		return ptr;
	}

	template <typename T>
	const T* Vec<T>::end() const
	{
		return ptr + count;
	}

	template <typename T>
	T& Vec<T>::operator[](i64 index)
	{
		return ptr[index];
	}

	template <typename T>
	const T& Vec<T>::operator[](i64 index) const
	{
		return ptr[index];
	}
}
