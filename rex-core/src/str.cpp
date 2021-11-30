#include "rex-core/str.h"
#include "rex-core/assert.h"

namespace rex::core
{
	inline static i64
	_str_len(const char* s)
	{
		i64 l = 0;
		while (*s++)
			l++;
		return l;
	}

	// API

	Str
	str_new(Allocator* allocator)
	{
		auto self = vec_with_capacity<char>(1, allocator);
		self[0] = '\0';
		return self;
	}

	Str
	str_from(const char* c_str, Allocator* allocator)
	{
		auto l = _str_len(c_str);
		auto self = vec_with_capacity<char>(l + 1, allocator);
		self.count = l;
		for (i64 i = 0; i < l; ++i)
			self[i] = c_str[i];
		self[self.count] = '\0';
		return self;
	}

	Str
	str_literal(const char* c_str)
	{
		auto self = Str{};
		self.ptr = (char*)c_str;
		self.count = _str_len(c_str);
		self.capacity = self.count + 1;
		return self;
	}

	Str
	str_copy(Str& self, Allocator* allocator)
	{
		auto other = vec_with_capacity<char>(self.count + 1, allocator);
		for (auto c: self)
			vec_push(other, c);
		other[other.count] = '\0';
		return other;
	}

	void
	str_free(Str& self)
	{
		if (self.allocator)
			vec_free(self);
	}

	void
	str_append(Str& self, const Str& other)
	{
		vec_reserve(self, other.count);
		for (auto c: other)
			vec_push(self, c);
		self[self.count] = '\0';
	}

	void
	str_append(Str& self, const char* other)
	{
		rex_assert_msg(self.allocator, "can't append to string literal");
		str_append(self, str_literal(other));
	}

	bool
	operator==(const Str& self, const Str& other)
	{
		if (self.count != other.count)
			return false;

		for (i64 i = 0; i < self.count; ++i)
			if (self[i] != other[i])
				return false;

		return true;
	}

	bool
	operator!=(const Str& self, const Str& other)
	{
		return !(self == other);
	}

	bool
	operator==(const Str& self, const char* other)
	{
		return self == str_literal(other);
	}

	bool
	operator!=(const Str& self, const char* other)
	{
		return self != str_literal(other);
	}

	bool
	operator==(const char* self, const Str& other)
	{
		return str_literal(self) == other;
	}

	bool
	operator!=(const char* self, const Str& other)
	{
		return str_literal(self) != other;
	}
}
