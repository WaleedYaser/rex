#pragma once

#include "rex-core/exports.h"
#include "rex-core/vec.h"

#include <stdint.h>

namespace rc
{
	using Str   = Vec<char>;
	using Str32 = Vec<int>;

	REX_CORE_EXPORT Str str_init(Allocator allocator = rex_allocator());
	REX_CORE_EXPORT void str_deinit(Str& self);

	REX_CORE_EXPORT sz str_len(const char* c_str);
	REX_CORE_EXPORT Str str_from(const char* fist, const char* last, Allocator allocator = rex_allocator());
	REX_CORE_EXPORT Str str_from(const char* c_str, Allocator allocator = rex_allocator());
	REX_CORE_EXPORT Str str_lit(const char* c_str);
	REX_CORE_EXPORT Str str_copy(const Str& self, Allocator allocator = rex_allocator());

	inline static Str
	clone(Str& self, Allocator allocator = rex_allocator())
	{
		return str_copy(self, allocator);
	}

	REX_CORE_EXPORT Str str_fmt(Allocator allocator, const char* format, ...);

	template <typename...Args>
	inline static Str
	str_fmt(const char* format, Args...args)
	{
		return str_fmt(rex_allocator(), format, args...);
	}

	REX_CORE_EXPORT void str_clear(Str& self);
	REX_CORE_EXPORT void str_append(Str& self, const Str& other);
	REX_CORE_EXPORT void str_append(Str& self, const char* format, ...);
	// TODO: write unit tests
	REX_CORE_EXPORT Vec<Str> str_lines(const Str& self, Allocator allocator = rex_allocator());
	REX_CORE_EXPORT Vec<Str> str_split(const Str& self, char c, Allocator allocator = rex_allocator());
	// TODO: use Res instead of boolean pointer
	REX_CORE_EXPORT double str_to_double(const Str& self, bool* succeeded);
	REX_CORE_EXPORT int64_t str_to_int(const Str& self, bool* succeeded);

	REX_CORE_EXPORT bool operator==(const Str& self, const Str& other);

	inline static bool operator!=(const Str& self, const Str& other)  { return !(self == other); }
	inline static bool operator==(const Str& self, const char* other) { return self == str_lit(other); }
	inline static bool operator!=(const Str& self, const char* other) { return !(self == other); }
	inline static bool operator==(const char* self, const Str& other) { return str_lit(self) == other; }
	inline static bool operator!=(const char* self, const Str& other) { return !(self == other); }

	struct Str32_Stream { Str utf8; };
	struct REX_CORE_EXPORT Str32_Iterator
	{
		const char* it;
		i32 value;
		i32 width;

		const i32& operator*() const;
		const i32* operator->() const;
		Str32_Iterator& operator++();
		Str32_Iterator operator++(int);
		bool operator==(const Str32_Iterator& other) const;
		bool operator!=(const Str32_Iterator& other) const;
	};

	REX_CORE_EXPORT Str32_Iterator begin(const Str32_Stream& self);
	REX_CORE_EXPORT Str32_Iterator end(const Str32_Stream& self);

	inline static Str32_Stream str32_stream(const Str& self) { return Str32_Stream{self}; }
	REX_CORE_EXPORT Str32 str_to_utf32(const Str& self, Allocator allocator = rex_allocator());
}
