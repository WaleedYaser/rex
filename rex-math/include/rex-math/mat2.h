#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Mat2<T> &A, const Mat2<T> &B)
	{
		return
			A[0][0] == B[0][0] && A[0][1] == B[0][1] &&
			A[1][0] == B[1][0] && A[1][1] == B[1][1];
	}

	template <typename T>
	inline static bool
	operator!=(const Mat2<T> &A, const Mat2<T> &B)
	{
		return !(A == B);
	}

	template <typename T>
	inline static Mat2<T>
	operator+(const Mat2<T> &A, const Mat2<T> &B)
	{
		return {
			A[0][0] + B[0][0], A[0][1] + B[0][1],
			A[1][0] + B[1][0], A[1][1] + B[1][1]
		};
	}

	template <typename T>
	inline static Mat2<T> &
	operator+=(Mat2<T> &A, const Mat2<T> &B)
	{
		A = A + B;
		return A;
	}

	template <typename T>
	inline static Mat2<T>
	operator-(const Mat2<T> &M)
	{
		return {
			-M[0][0], -M[0][1],
			-M[1][0], -M[1][1]
		};
	}

	template <typename T>
	inline static Mat2<T>
	operator-(const Mat2<T> &A, const Mat2<T> &B)
	{
		return {
			A[0][0] - B[0][0], A[0][1] - B[0][1],
			A[1][0] - B[1][0], A[1][1] - B[1][1]
		};
	}

	template <typename T>
	inline static Mat2<T> &
	operator-=(Mat2<T> &A, const Mat2<T> &B)
	{
		A = A - B;
		return A;
	}

	template <typename T>
	inline static Mat2<T>
	operator*(const Mat2<T> &M, T f)
	{
		return {
			M[0][0] * f, M[0][1] * f,
			M[1][0] * f, M[1][1] * f
		};
	}

	template <typename T>
	inline static Mat2<T>
	operator*(T f, const Mat2<T> &M)
	{
		return M * f;
	}

	template <typename T>
	inline static Mat2<T> &
	operator*=(Mat2<T> &M, T f)
	{
		M = M * f;
		return M;
	}

	template <typename T>
	inline static Vec2<T>
	operator*(const Vec2<T> &v, const Mat2<T> &M)
	{
		return {
			v.x * M[0][0] + v.y * M[1][0],
			v.x * M[0][1] + v.y * M[1][1]
		};
	}

	template <typename T>
	inline static Mat2<T>
	operator*(const Mat2<T> &A, const Mat2<T> &B)
	{
		Mat2<T> C;

		C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
		C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];

		C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
		C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];

		return C;
	}

	template <typename T>
	inline static Mat2<T>
	operator/(const Mat2<T> &M, T f)
	{
		return Mat2<T>{
			M[0][0] / f, M[0][1] / f,
			M[1][0] / f, M[1][1] / f
		};
	}

	template <typename T>
	inline static Mat2<T>
	operator/(T f, const Mat2<T> &M)
	{
		return M / f;
	}

	template <typename T>
	inline static Mat2<T> &
	operator/=(Mat2<T> &M, T f)
	{
		M = M / f;
		return M;
	}

	template <typename T>
	inline static Mat2<T>
	mat2_identity()
	{
		return {
			1, 0,
			0, 1
		};
	}

	template <typename T>
	inline static Mat2<T>
	mat2_rotation(T theta)
	{
		T c = cos(theta);
		T s = sin(theta);

		return {
			 c, s,
			-s, c
		};
	}

	template <typename T>
	inline static Mat2<T>
	mat2_scaling(T sx, T sy)
	{
		return {
			sx,  0,
			 0, sy
		};
	}

	template <typename T>
	inline static Mat2<T>
	mat2_scaling(const Vec2<T> &s)
	{
		return mat2_scaling(s.x, s.y);
	}

	template <typename T>
	inline static Mat2<T>
	mat2_shearing_x(T s)
	{
		return {
			1, 0,
			s, 1
		};
	}

	template <typename T>
	inline static Mat2<T>
	mat2_shearing_y(T s)
	{
		return {
			1, s,
			0, 1
		};
	}

	template <typename T>
	inline static Mat2<T>
	transpose(const Mat2<T> &M)
	{
		return {
			M[0][0], M[1][0],
			M[0][1], M[1][1]
		};
	}

	template <typename T>
	inline static T
	trace(const Mat2<T> &M)
	{
		return M[0][0] + M[1][1];
	}

	template <typename T>
	inline static T
	det(const Mat2<T> &M)
	{
		return M[0][0] * M[1][1] - M[0][1] * M[1][0];
	}

	template <typename T>
	inline static bool
	invertible(const Mat2<T> &M)
	{
		return det(M) != 0.0f;
	}

	template <typename T>
	inline static Mat2<T>
	inverse(const Mat2<T> &M)
	{
		T d = det(M);
		if (d == 0.0f)
			return {};

		return (1.0f / d) * Mat2<T>{
			 M[1][1], -M[0][1],
			-M[1][0],  M[0][0]
		};
	}
}
