#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Vec3<T> &a, const Vec3<T> &b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template <typename T>
	inline static bool
	operator!=(const Vec3<T> &a, const Vec3<T> &b)
	{
		return !(a == b);
	}

	template <typename T>
	inline static Vec3<T>
	operator+(const Vec3<T> &a, const Vec3<T> &b)
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z};
	}

	template <typename T>
	inline static Vec3<T> &
	operator+=(Vec3<T> &a, const Vec3<T> &b)
	{
		a = a + b;
		return a;
	}

	template <typename T>
	inline static Vec3<T>
	operator-(const Vec3<T> &v)
	{
		return {-v.x, -v.y, -v.z};
	}

	template <typename T>
	inline static Vec3<T>
	operator-(const Vec3<T> &a, const Vec3<T> &b)
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z};
	}

	template <typename T>
	inline static Vec3<T> &
	operator-=(Vec3<T> &a, const Vec3<T> &b)
	{
		a = a - b;
		return a;
	}

	template <typename T>
	inline static Vec3<T>
	operator*(const Vec3<T> &v, T f)
	{
		return {v.x * f, v.y * f, v.z * f};
	}

	template <typename T>
	inline static Vec3<T>
	operator*(T f, const Vec3<T> &v)
	{
		return v * f;
	}

	template <typename T>
	inline static Vec3<T> &
	operator*=(Vec3<T> &v, T f)
	{
		v = v * f;
		return v;
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec3<T>
	operator*(const Vec3<T> a, const Vec3<T> &b)
	{
		return {a.x * b.x, a.y * b.y, a.z * b.z};
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec3<T> &
	operator*=(Vec3<T> &a, const Vec3<T> b)
	{
		a = a * b;
		return a;
	}


	template <typename T>
	inline static Vec3<T>
	operator/(const Vec3<T> &v, T f)
	{
		return v * (1.0f / f);
	}

	template <typename T>
	inline static Vec3<T> &
	operator/=(Vec3<T> &v, T f)
	{
		v = v / f;
		return v;
	}

	template <typename T>
	inline static T
	dot(const Vec3<T> &a, const Vec3<T> &b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	template <typename T>
	inline static T
	norm(const Vec3<T> &v)
	{
		return sqrt(dot(v, v));
	}

	template <typename T>
	inline static T
	length(const Vec3<T> &v)
	{
		return norm(v);
	}

	template <typename T>
	inline static Vec3<T>
	normalize(const Vec3<T> &v)
	{
		return v / length(v);
	}

	template <typename T>
	inline static Vec3<T>
	cross(const Vec3<T> &a, const Vec3<T> &b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec3<T>
	min(const Vec3<T> &a, const Vec3<T> &b)
	{
		return {
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z
		};
	}

	// TODO: add unittest
	template <typename T>
	inline static Vec3<T>
	max(const Vec3<T> &a, const Vec3<T> &b)
	{
		return {
			b.x < a.x ? a.x : b.x,
			b.y < a.y ? a.y : b.y,
			b.z < a.z ? a.z : b.z
		};
	}
}
