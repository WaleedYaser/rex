#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Vec4<T> &a, const Vec4<T> &b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	template <typename T>
	inline static bool
	operator!=(const Vec4<T> &a, const Vec4<T> &b)
	{
		return !(a == b);
	}

	template <typename T>
	inline static Vec4<T>
	operator+(const Vec4<T> &a, const Vec4<T> &b)
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
	}

	template <typename T>
	inline static Vec4<T> &
	operator+=(Vec4<T> &a, const Vec4<T> &b)
	{
		a = a + b;
		return a;
	}

	template <typename T>
	inline static Vec4<T>
	operator-(const Vec4<T> &v)
	{
		return {-v.x, -v.y, -v.z, -v.w};
	}

	template <typename T>
	inline static Vec4<T>
	operator-(const Vec4<T> &a, const Vec4<T> &b)
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
	}

	template <typename T>
	inline static Vec4<T> &
	operator-=(Vec4<T> &a, const Vec4<T> &b)
	{
		a = a - b;
		return a;
	}

	template <typename T>
	inline static Vec4<T>
	operator*(const Vec4<T> &v, T f)
	{
		return {v.x * f, v.y * f, v.z * f, v.w * f};
	}

	template <typename T>
	inline static Vec4<T>
	operator*(T f, const Vec4<T> &v)
	{
		return v * f;
	}

	template <typename T>
	inline static Vec4<T> &
	operator*=(Vec4<T> &v, T f)
	{
		v = v * f;
		return v;
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec4<T>
	operator*(const Vec4<T> a, const Vec4<T> &b)
	{
		return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec4<T> &
	operator*=(Vec4<T> &a, const Vec4<T> b)
	{
		a = a * b;
		return a;
	}

	template <typename T>
	inline static Vec4<T>
	operator/(const Vec4<T> &v, T f)
	{
		return v * (1.0f / f);
	}

	template <typename T>
	inline static Vec4<T> &
	operator/=(Vec4<T> &v, T f)
	{
		v = v / f;
		return v;
	}

	template <typename T>
	inline static T
	dot(const Vec4<T> &a, const Vec4<T> &b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}

	template <typename T>
	inline static T
	norm(const Vec4<T> &v)
	{
		return sqrt(dot(v, v));
	}

	template <typename T>
	inline static T
	length(const Vec4<T> &v)
	{
		return norm(v);
	}

	template <typename T>
	inline static Vec4<T>
	normalize(const Vec4<T> &v)
	{
		return v / length(v);
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec4<T>
	min(const Vec4<T> &a, const Vec4<T> &b)
	{
		return {
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z,
			a.w < b.w ? a.w : b.w
		};
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec4<T>
	max(const Vec4<T> &a, const Vec4<T> &b)
	{
		return {
			b.x < a.x ? a.x : b.x,
			b.y < a.y ? a.y : b.y,
			b.z < a.z ? a.z : b.z,
			b.w < a.w ? a.w : b.w
		};
	}
}
