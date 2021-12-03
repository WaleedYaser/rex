#pragma once

#include "rex-core/defines.h"
#include "rex-core/memory.h"

namespace rex::core
{
	struct Str
	{
		char* ptr;
		i64 count;
		i64 capacity;
		Allocator* allocator;

		REX_API static Str init(Allocator* allocator = default_allocator());
		REX_API static Str from(const char* c_str, Allocator* allocator = default_allocator());
		REX_API static Str lit(const char* c_str);

		REX_API Str clone(Allocator* allocator = default_allocator());
		REX_API void dispose();
		REX_API void destroy();

		REX_API void append(const Str& other);
		REX_API void append(const char* other);

		REX_API char* begin();
		REX_API char* end();
		REX_API const char* begin() const;
		REX_API const char* end() const;

		REX_API char& operator[](i64 index);
		REX_API const char& operator[](i64 index) const;

		REX_API bool operator==(const Str& other);
		REX_API bool operator!=(const Str& other);
		REX_API bool operator==(const char* other);
		REX_API bool operator!=(const char* other);
	};
}
