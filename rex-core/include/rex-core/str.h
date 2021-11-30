#pragma once

#include "rex-core/defines.h"
#include "rex-core/vec.h"

namespace rex::core
{
	using Str = Vec<char>;

	REX_API Str str_new(Allocator* allocator = default_allocator());
	REX_API Str str_from(const char* c_str, Allocator* allocator = default_allocator());
	REX_API Str str_literal(const char* c_str);
	REX_API Str str_copy(Str& self, Allocator* allocator = default_allocator());
	REX_API void str_free(Str& self);
	REX_API void str_clear(Str& self);
	REX_API void str_append(Str& self, const Str& other);
	REX_API void str_append(Str& self, const char* other);

	REX_API bool operator==(const Str& self, const Str& other);
	REX_API bool operator!=(const Str& self, const Str& other);
	REX_API bool operator==(const Str& self, const char* other);
	REX_API bool operator!=(const Str& self, const char* other);
	REX_API bool operator==(const char* self, const Str& other);
	REX_API bool operator!=(const char* self, const Str& other);
}
