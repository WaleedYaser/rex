#pragma once

#include "rex-core/defines.h"
#include "rex-core/types.h"

// define/undefine based on whether you want to enable or disable assertion, most propably you wont need to disable it.
#define REX_ASSERTION_ENABLED (1)

namespace rex::core
{
	// report assertion failure by logging it, most propably you won't need to call this function outside the macros
	// defined bellow.
	//  expression: string for the failed expression.
	//  message: string to log as the reason for the failure.
	//  file: file where assertion failed.
	//  line: line in the file where assertion failed.
	REX_API void
	report_assertion_failure(const char *expression, const char *message, const char *file, i32 line);
}

#if REX_ASSERTION_ENABLED
# if REX_OS_WINDOWS
// __debugbreak: causes a breakpoint in the code where the user will be prompted to run debugger.
#  define rex_debug_break() __debugbreak()
# else
// __builtin_trap: not continue next instruction
#  define rex_debug_break() __builtin_trap()
# endif
// assert that the expression is true
# define rex_assert(expr)                                                   \
{                                                                           \
	if (!(expr))                                                            \
	{                                                                       \
		rex::core::report_assertion_failure(#expr, "", __FILE__, __LINE__); \
		rex_debug_break();                                                  \
	}                                                                       \
}
// assert that the expression is true, report message otherwise
# define rex_assert_msg(expr, message)                                           \
{                                                                                \
	if (!(expr))                                                                 \
	{                                                                            \
		rex::core::report_assertion_failure(#expr, message, __FILE__, __LINE__); \
		rex_debug_break();                                                       \
	}                                                                            \
}
#else
# define rex_assert(expr)              // do nothing
# define rex_assert_msg(expr, message) // do nothing
#endif
