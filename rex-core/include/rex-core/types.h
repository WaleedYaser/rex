#pragma once

// unsigned int types
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

// signed int types
using i8  = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

// floating point types
using f32  = float;
using f64  = double;

// check that the types has the correct size
static_assert(sizeof(u8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(i8)  == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

// types minimum limits
inline static constexpr u8  U8_MIN  = 0;
inline static constexpr u16 U16_MIN = 0;
inline static constexpr u32 U32_MIN = 0;
inline static constexpr u64 U64_MIN = 0;

inline static constexpr i8  I8_MIN  = -127 - 1;
inline static constexpr i16 I16_MIN = -32'767 - 1;
inline static constexpr i32 I32_MIN = -2'147'483'647 - 1;
inline static constexpr i64 I64_MIN = -9'223'372'036'854'775'807ll - 1;

inline static constexpr f32 F32_MIN = 1.175'494'351e-38f;           // min normalized positive value
inline static constexpr f64 F64_MIN = 2.225'073'858'507'201'4e-308; // min normalized positive value

// types maximum limits
inline static constexpr u8  U8_MAX  = 255;
inline static constexpr u16 U16_MAX = 65'535;
inline static constexpr u32 U32_MAX = 4'294'967'295u;
inline static constexpr u64 U64_MAX = 18'446'744'073'709'551'615ull;

inline static constexpr i8  I8_MAX  = 127;
inline static constexpr i16 I16_MAX = 32'767;
inline static constexpr i32 I32_MAX = 2'147'483'647;
inline static constexpr i64 I64_MAX = 9'223'372'036'854'775'807ll;

inline static constexpr f32 F32_MAX = 3.402823466e+38f;        // max value
inline static constexpr f64 F64_MAX = 1.7976931348623158e+308; // max value

// vector types
template <typename T>
union V2
{
	struct { T x, y; };
	struct { T u, v; };
	struct { T r, g; };
	struct { T width, height; };
	T values[2];

	inline T &operator[](u8 index) { return values[index]; }
	inline const T &operator[](u8 index) const { return values[index]; }
};

template <typename T>
union V3
{
	struct { T x, y, z; };
	struct { T u, v, w; };
	struct { T r, g, b; };
	struct { T width, height, depth; };

	struct { V2<T> xy; T _pad0; };
	struct { T _pad1; V2<T> yz; };

	struct { V2<T> uv; T _pad2; };
	struct { T _pad3; V2<T> vw; };

	struct { V2<T> rg; T _pad4; };
	struct { T _pad5; V2<T> gb; };

	T values[3];

	inline T &operator[](u8 index) { return values[index]; }
	inline const T &operator[](u8 index) const { return values[index]; }
};

template <typename T>
union V4
{
	struct { T x, y, z, w; };
	struct { T r, g, b, a; };

	struct { V3<T> xyz; T _pad0; };
	struct { T _pad1; V3<T> yzw; };

	struct { V3<T> rgb; T _pad2; };
	struct { T _pad3; V3<T> gba; };

	struct { V2<T> xy; V2<T> zw[2]; };
	struct { T _pad4; V2<T> yz; T _pad5; };

	struct { V2<T> rg; T _pad7[2]; };
	struct { T _pad8; V2<T> gb; T _pad9; };
	struct { T _pad10[2]; V2<T> ba; };

	T values[4];

	inline T &operator[](u8 index) { return values[index]; }
	inline const T &operator[](u8 index) const { return values[index]; }
};

// alias common vector types
using v2 = V2<f32>;
using v3 = V3<f32>;
using v4 = V4<f32>;

using v2d = V2<f64>;
using v3d = V3<f64>;
using v4d = V4<f64>;

using v2u = V2<u32>;
using v3u = V3<u32>;
using v4u = V4<u32>;

using v2i = V2<i32>;
using v3i = V3<i32>;
using v4i = V4<i32>;

// colors
using Color_U8  = V4<u8>;  // 8-bit color
using Color_F32 = V4<f32>; // normalized color
