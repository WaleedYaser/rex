#pragma once

// unsigned int types
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

// signed int types
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

// floating point types
typedef float              f32;
typedef double             f64;

// boolean types
typedef i8                 b8;
typedef i32                b32;

#undef TRUE
#undef FALSE

#define TRUE               (1)
#define FALSE              (0)

// types minimum limits
#undef I8_MIN
#undef I16_MIN
#undef I32_MIN
#undef I64_MIN
#undef U8_MIN
#undef U16_MIN
#undef U32_MIN
#undef U64_MIN
#undef F32_MIN
#undef F64_MIN

#define I8_MIN  ((i8)(-128))
#define I16_MIN ((i16)(-32767-1))
#define I32_MIN ((i32)(-2147483647-1))
#define I64_MIN ((i64)(-9223372036854775807ll-1))
#define U8_MIN  ((u8)0u)
#define U16_MIN ((u16)0u)
#define U32_MIN ((u32)0u)
#define U64_MIN ((u64)0ull)
#define F32_MIN ((f32)1.175494351e-38f)          // min normalized positive value
#define F64_MIN ((f64)2.2250738585072014e-308)   // min normalized positive value

// types maximum limits
#undef I8_MAX
#undef I16_MAX
#undef I32_MAX
#undef I64_MAX
#undef U8_MAX
#undef U16_MAX
#undef U32_MAX
#undef U64_MAX
#undef F32_MAX
#undef F64_MAX

#define I8_MAX  ((i8)127)
#define I16_MAX ((i16)32767)
#define I32_MAX ((i32)2147483647)
#define I64_MAX ((i64)9223372036854775807ll)
#define U8_MAX  ((u8)255u)
#define U16_MAX ((u16)65535u)
#define U32_MAX ((u32)4294967295u)
#define U64_MAX ((u64)18446744073709551615ull)
#define F32_MAX ((f32)3.402823466e+38f)          // max value
#define F64_MAX ((f64)1.7976931348623158e+308)   // max value

// detection macros source: https://github.com/cxxstuff/cxx_detect
// target operating systems
#undef REX_OS_WINDOWS
#undef REX_OS_LINUX

#if defined(_WIN32) || defined(_WINDOWS)
# define REX_OS_WINDOWS (1)
# pragma message("REX_OS_WINDOWS")
#elif defined(__linux__)
# define REX_OS_LINUX   (1)
# pragma message("REX_OS_LINUX")
#else
# error "unsupported platform"
#endif

// target compilers
#undef REX_CLANG
#undef REX_GCC
#undef REX_MSC

#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
# define REX_MSC   (1)
# pragma message("REX_MSC")
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
# define REX_GCC   (1)
# pragma message("REX_GCC")
#elif defined(__clang__) && defined(__clang_minor__)
# define REX_CLANG (1)
# pragma message("REX_CLANG")
#else
# error "unsupported compiler"
#endif

// import export
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
#endif

// static assertion tests an assertion at compile time
#if REX_CLANG || REX_GCC
# define rex_static_assert _Static_assert
#else
# define rex_static_assert static_assert
#endif

// make sure all types has the correct size
rex_static_assert(sizeof(u8)  == 1, "u8 size expected to be 1 byte");
rex_static_assert(sizeof(u16) == 2, "u16 size expected to be 2 byte");
rex_static_assert(sizeof(u32) == 4, "u32 size expected to be 4 byte");
rex_static_assert(sizeof(u64) == 8, "u64 size expected to be 8 byte");

rex_static_assert(sizeof(i8)  == 1, "i8 size expected to be 1 byte");
rex_static_assert(sizeof(i16) == 2, "i16 size expected to be 2 byte");
rex_static_assert(sizeof(i32) == 4, "i32 size expected to be 4 byte");
rex_static_assert(sizeof(i64) == 8, "i64 size expected to be 8 byte");

rex_static_assert(sizeof(f32) == 4, "f32 size expected to be 4 byte");
rex_static_assert(sizeof(f64) == 8, "f64 size expected to be 8 byte");

rex_static_assert(sizeof(b8)  == 1, "b8 size expected to be 1 byte");
rex_static_assert(sizeof(b32) == 4, "b32 size expected to be 4 byte");
