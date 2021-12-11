#pragma once

#include "rex-core/str.h"
#include "rex-core/exports.h"

namespace rc
{
	// TODO: add unit tests
	REX_CORE_EXPORT Str file_read(Str path, Allocator allocator = rex_allocator());

	// TODO: add unit tests
	inline static Str
	file_read(const char* path, Allocator allocator = rex_allocator())
	{
		return file_read(str_lit(path), allocator);
	}
}
