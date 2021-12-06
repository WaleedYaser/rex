#pragma once

#include "rex-core/exports.h"
#include "rex-core/types.h"

// define/undefine based on whether you want to enable or disable assertion, most propably you wont need to disable it.
#define REX_ASSERTION_ENABLED (1)

namespace rc
{
	REX_CORE_EXPORT void report_assertion(const char *expression, const char *message, const char *file, i32 line);
}

#if REX_ASSERTION_ENABLED
# if REX_OS_WINDOWS
#  define rex_debug_break() __debugbreak()
# else
#  define rex_debug_break() __builtin_trap()
# endif
# define rex_assert(expr)                                    \
{                                                            \
	if (!(expr))                                             \
	{                                                        \
		rc::report_assertion(#expr, "", __FILE__, __LINE__); \
		rex_debug_break();                                   \
	}                                                        \
}
# define rex_assert_msg(expr, message)                            \
{                                                                 \
	if (!(expr))                                                  \
	{                                                             \
		rc::report_assertion(#expr, message, __FILE__, __LINE__); \
		rex_debug_break();                                        \
	}                                                             \
}
#else
# define rex_assert(expr)              // do nothing
# define rex_assert_msg(expr, message) // do nothing
#endif
