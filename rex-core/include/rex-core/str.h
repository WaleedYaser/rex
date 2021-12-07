#pragma once

#include "rex-core/exports.h"
#include "rex-core/vec.h"

namespace rc
{
	using Str = Vec<char>;

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

	REX_CORE_EXPORT bool operator==(Str& self, Str& other);

	inline static bool operator!=(Str& self, Str& other)        { return !(self == other); }
	inline static bool operator==(Str& self, const char* other) { return self == str_lit(other); }
	inline static bool operator!=(Str& self, const char* other) { return !(self == other); }
	inline static bool operator==(const char* self, Str& other) { return str_lit(self) == other; }
	inline static bool operator!=(const char* self, Str& other) { return !(self == other); }
}
