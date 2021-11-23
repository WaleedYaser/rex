#pragma once

// detection macros source: https://github.com/cxxstuff/cxx_detect

// cleanup.
#undef REX_OS_WINDOWS
#undef REX_OS_LINUX

#undef REX_CLANG
#undef REX_GCC
#undef REX_MSC

#undef REX_API

// target operating systems.
#if defined(_WIN32) || defined(_WINDOWS)
# define REX_OS_WINDOWS (1)
#elif defined(__linux__)
# define REX_OS_LINUX   (1)
#else
# error "unsupported platform"
#endif

// target compilers.
#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
# define REX_MSC   (1)
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
# define REX_GCC   (1)
#elif defined(__clang__) && defined(__clang_minor__)
# define REX_CLANG (1)
#else
# error "unsupported compiler"
#endif

// import export.
// prefix any function you want to export with REX_API so any one linking to the dll or shared library can see it.
#if REX_EXPORT
# if REX_MSC
#  define REX_API __declspec(dllexport)
# elif REX_GCC || REX_CLANG
#  define REX_API __attribute__((visibility("default")))
# else
#  error "unsupported compiler"
# endif
#elif REX_IMPORT
// imports
# ifdef REX_MSC
#  define REX_API __declspec(dllimport)
# else
#  define REX_API
# endif
#else
# define REX_API
#endif
