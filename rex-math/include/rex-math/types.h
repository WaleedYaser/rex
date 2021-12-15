#pragma once

#include "rex-math/defines.h"
#include <stdint.h>

REX_MSVC_SUPPRESS_WARNING_PUSH
REX_MSVC_SUPPRESS_WARNING(4201) // warning C4201: nonstandard extension used: nameless struct/union

namespace rex::math
{
	template <typename T>
	union Vec2
	{
		struct { T x, y; };
		struct { T u, v; };
		struct { T r, g; };
		struct { T width, height; };
		T values[2];

		inline T& operator[](int index) { return values[index]; }
		inline const T& operator[](int index) const { return values[index]; }
	};

	template <typename T>
	union Vec3
	{
		struct { T x, y, z; };
		struct { T u, v, w; };
		struct { T r, g, b; };
		struct { T width, height, depth; };

		struct { Vec2<T> xy; T _pad0; };
		struct { T _pad1; Vec2<T> yz; };

		struct { Vec2<T> uv; T _pad2; };
		struct { T _pad3; Vec2<T> vw; };

		struct { Vec2<T> rg; T _pad4; };
		struct { T _pad5; Vec2<T> gb; };

		T values[3];

		inline T& operator[](int index) { return values[index]; }
		inline const T& operator[](int index) const { return values[index]; }
	};

	template <typename T>
	union Vec4
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };

		struct { Vec3<T> xyz; T _pad0; };
		struct { T _pad1; Vec3<T> yzw; };

		struct { Vec3<T> rgb; T _pad2; };
		struct { T _pad3; Vec3<T> gba; };

		// struct { V2<T> xy; V2<T> zw[2]; };
		// struct { T _pad4; V2<T> yz; T _pad5; };

		// struct { V2<T> rg; T _pad7[2]; };
		// struct { T _pad8; V2<T> gb; T _pad9; };
		// struct { T _pad10[2]; V2<T> ba; };

		T values[4];

		inline T& operator[](int index) { return values[index]; }
		inline const T& operator[](int index) const { return values[index]; }
	};

	template <typename T>
	struct Mat2
	{
		Vec2<T> rows[2];

		inline Vec2<T> &operator[](int index) { return rows[index]; }
		inline const Vec2<T> &operator[](int index) const { return rows[index]; }
	};

	template <typename T>
	struct Mat3
	{
		Vec3<T> rows[3];

		inline Vec3<T> &operator[](int index) { return rows[index]; }
		inline const Vec3<T> &operator[](int index) const { return rows[index]; }
	};

	template <typename T>
	struct Mat4
	{
		Vec4<T> rows[4];

		inline Vec4<T> &operator[](int index) { return rows[index]; }
		inline const Vec4<T> &operator[](int index) const { return rows[index]; }
	};

	// alias common types
	using V2 = Vec2<float>;
	using V3 = Vec3<float>;
	using V4 = Vec4<float>;

	using V2d = Vec2<double>;
	using V3d = Vec3<double>;
	using V4d = Vec4<double>;

	using V2u = Vec2<uint32_t>;
	using V3u = Vec3<uint32_t>;
	using V4u = Vec4<uint32_t>;

	using V2i = Vec2<int32_t>;
	using V3i = Vec3<int32_t>;
	using V4i = Vec4<int32_t>;

	using Color_U8  = Vec4<uint8_t>;  // 8-bit color
	using Color_F32 = Vec4<float>;    // normalized color

	using M2 = Mat2<float>;
	using M3 = Mat3<float>;
	using M4 = Mat4<float>;

	using M2d = Mat2<double>;
	using M3d = Mat3<double>;
	using M4d = Mat4<double>;

	static_assert(sizeof(V2) == 8);
	static_assert(sizeof(V3) == 12);
	static_assert(sizeof(V4) == 16);

	static_assert(sizeof(V2d) == 16);
	static_assert(sizeof(V3d) == 24);
	static_assert(sizeof(V4d) == 32);

	static_assert(sizeof(V2u) == 8);
	static_assert(sizeof(V3u) == 12);
	static_assert(sizeof(V4u) == 16);

	static_assert(sizeof(V2i) == 8);
	static_assert(sizeof(V3i) == 12);
	static_assert(sizeof(V4i) == 16);

	static_assert(sizeof(Color_U8) == 4);
	static_assert(sizeof(Color_F32) == 16);
}

REX_MSVC_SUPPRESS_WARNING_PUSH
