#include "rex-core/str.h"
#include "rex-core/assert.h"

#include <cstdarg>
#include <cstdio>

namespace rc
{
	Str
	str_init(Allocator allocator)
	{
		return vec_init<char>(allocator);
	}

	void
	str_deinit(Str& self)
	{
		vec_deinit(self);
	}

	sz
	str_len(const char* c_str)
	{
		sz len = 0;
		while (*c_str++)
			len++;
		return len;
	}

	Str
	str_from(const char* first, const char* last, Allocator allocator)
	{
		auto len = last - first;
		rex_assert(len != 0);

		auto self = vec_with_capacity<char>(len + 1, allocator);
		vec_append(self, first, last);
		self[len] = '\0';

		return self;
	}

	Str
	str_from(const char* c_str, Allocator allocator)
	{
		return str_from(c_str, c_str + str_len(c_str), allocator);
	}

	Str
	str_lit(const char* c_str)
	{
		Str self = {};
		self.ptr = (char*)c_str;
		self.count = str_len(c_str);
		self.capacity = self.count + 1;
		return self;
	}

	Str
	str_copy(const Str& self, Allocator allocator)
	{
		auto other = self;
		other.count++;
		other = vec_copy(other, allocator);
		other.count--;
		return other;
	}

	Str
	str_fmt(Allocator allocator, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		i32 len = ::vsnprintf(nullptr, 0, format, args);
		va_end(args);

		if (len < 0)
		{
			rex_assert_msg(false, "[str_fmt]: failed to format strring");
			return {};
		}

		auto self = vec_with_capacity<char>(len + 1, allocator);
		va_start(args, format);
		::vsnprintf(self.ptr, self.capacity, format, args);
		va_end(args);

		self.count = len;
		return self;
	}

	void
	str_clear(Str& self)
	{
		vec_clear(self);
		self[0] = '\0';
	}

	void
	str_append(Str& self, const Str& other)
	{
		rex_assert(other.ptr);
		rex_assert_msg(self.allocator, "[str_append]: can't append to string literal")

		vec_append(self, other.ptr, other.ptr + other.count + 1);
		self.count--;
	}

	void
	str_append(Str& self, const char* format, ...)
	{
		rex_assert_msg(self.allocator, "[str_append]: can't append to string literal")

		va_list args;
		va_start(args, format);
		i32 len = ::vsnprintf(nullptr, 0, format, args);
		va_end(args);

		if (len < 0)
		{
			rex_assert_msg(false, "[str_fmt]: failed to format strring");
			return;
		}

		auto min_capacity = self.count + len + 1;
		if (self.capacity < min_capacity)
			vec_reserve(self, min_capacity - self.capacity);

		va_start(args, format);
		::vsnprintf(self.ptr + self.count, len + 1, format, args);
		va_end(args);

		self.count += len;
	}

	bool
	operator==(Str& self, Str& other)
	{
		if (self.count != other.count)
			return false;

		for (sz i = 0; i < self.count; ++i)
			if (self[i] != other[i])
				return false;

		return true;
	}

	// resource: https://github.com/RandyGaul/cute_headers/blob/master/cute_utf.h
	// TODO: handle errors 0xFFFD <?>
	inline static i32
	_decode_utf8(const char* it, i32* cp)
	{
		u8 c = *it++;
		i32 extra = 0;
		     if (c >= 0xF0) { *cp = c & 0x07; extra = 3; }
		else if (c >= 0xE0) { *cp = c & 0x0F; extra = 2; }
		else if (c >= 0xC0) { *cp = c & 0x1F; extra = 1; }
		else                { *cp = c; }

		i32 count = extra + 1;
		while (extra--)
		{
			c = *it++;
			*cp = ((*cp) << 6) | (c & 0x3F);
		}

		return count;
	}


	const i32&
	Str32_Iterator::operator*() const
	{
		return value;
	}

	const i32*
	Str32_Iterator::operator->() const
	{
		return &value;
	}

	Str32_Iterator&
	Str32_Iterator::operator++()
	{
		it += width;
		width = _decode_utf8(it , &value);
		return *this;
	}

	Str32_Iterator
	Str32_Iterator::operator++(int)
	{
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	bool
	Str32_Iterator::operator==(const Str32_Iterator& other) const
	{
		return it == other.it;
	}

	bool
	Str32_Iterator::operator!=(const Str32_Iterator& other) const
	{
		return it != other.it;
	}

	Str32_Iterator
	begin(const Str32_Stream& self)
	{
		Str32_Iterator res = {};
		res.it = begin(self.utf8);
		res.width = _decode_utf8(res.it, &res.value);
		return res;
	}

	Str32_Iterator
	end(const Str32_Stream& self)
	{
		return { end(self.utf8) };
	}

	Str32
	str_to_utf32(const Str& self, Allocator allocator)
	{
		auto res = vec_init<i32>(allocator);
		for (auto c: str32_stream(self))
			vec_push(res, c);
		return res;
	}
}
