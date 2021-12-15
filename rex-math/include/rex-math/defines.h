#pragma once

// compiler warnings helpers are taken from: https://github.com/onqtam/doctest

#if REX_REX_COMPILER_MSVC
	#define REX_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
	#define REX_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
	#define REX_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#else
	#define REX_MSVC_SUPPRESS_WARNING_PUSH
	#define REX_MSVC_SUPPRESS_WARNING(w)
	#define REX_MSVC_SUPPRESS_WARNING_POP
#endif

#if REX_COMPILER_GNU
	#define REX_PRAGMA_TO_STR(x) _Pragma(#x)
	#define REX_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
	#define REX_GCC_SUPPRESS_WARNING(w) REX_PRAGMA_TO_STR(GCC diagnostic ignored w)
	#define REX_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#else
	#define REX_GCC_SUPPRESS_WARNING_PUSH
	#define REX_GCC_SUPPRESS_WARNING(w)
	#define REX_GCC_SUPPRESS_WARNING_POP
#endif

#if REX_COMPILER_CLANG
	#define REX_PRAGMA_TO_STR(x) _Pragma(#x)
	#define REX_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
	#define REX_CLANG_SUPPRESS_WARNING(w) REX_PRAGMA_TO_STR(clang diagnostic ignored w)
	#define REX_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#else
	#define REX_CLANG_SUPPRESS_WARNING_PUSH
	#define REX_CLANG_SUPPRESS_WARNING(w)
	#define REX_CLANG_SUPPRESS_WARNING_POP
#endif
