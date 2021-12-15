#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Vec2<T> &a, const Vec2<T> &b)
	{
		return a.x == b.x && a.y == b.y;
	}

	template <typename T>
	inline static bool
	operator!=(const Vec2<T> &a, const Vec2<T> &b)
	{
		return !(a == b);
	}

	template <typename T>
	inline static Vec2<T>
	operator+(const Vec2<T> &a, const Vec2<T> &b)
	{
		return {a.x + b.x, a.y + b.y};
	}

	template <typename T>
	inline static Vec2<T> &
	operator+=(Vec2<T> &a, const Vec2<T> &b)
	{
		a = a + b;
		return a;
	}

	template <typename T>
	inline static Vec2<T>
	operator-(const Vec2<T> &v)
	{
		return {-v.x, -v.y};
	}

	template <typename T>
	inline static Vec2<T>
	operator-(const Vec2<T> &a, const Vec2<T> &b)
	{
		return {a.x - b.x, a.y - b.y};
	}

	template <typename T>
	inline static Vec2<T> &
	operator-=(Vec2<T> &a, const Vec2<T> &b)
	{
		a = a - b;
		return a;
	}

	template <typename T>
	inline static Vec2<T>
	operator*(const Vec2<T> &v, T f)
	{
		return {v.x * f, v.y * f};
	}

	template <typename T>
	inline static Vec2<T>
	operator*(T f, const Vec2<T> &v)
	{
		return v * f;
	}

	template <typename T>
	inline static Vec2<T> &
	operator*=(Vec2<T> &v, T f)
	{
		v = v * f;
		return v;
	}

	template <typename T>
	inline static Vec2<T>
	operator/(const Vec2<T> &v, T f)
	{
		return v * (1.0f / f);
	}

	template <typename T>
	inline static Vec2<T> &
	operator/=(Vec2<T> &v, T f)
	{
		v = v / f;
		return v;
	}

	template <typename T>
	inline static float
	dot(const Vec2<T> &a, const Vec2<T> &b)
	{
		return (a.x * b.x + a.y * b.y);
	}

	template <typename T>
	inline static float
	norm(const Vec2<T> &v)
	{
		return sqrt(dot(v, v));
	}

	template <typename T>
	inline static float
	length(const Vec2<T> &v)
	{
		return norm(v);
	}

	template <typename T>
	inline static Vec2<T>
	normalize(const Vec2<T> &v)
	{
		return v / length(v);
	}

	template <typename T>
	inline static float
	cross(const Vec2<T> &a, const Vec2<T> &b)
	{
		return (a.x * b.y - a.y * b.x);
	}
}
