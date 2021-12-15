#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Mat3<T> &A, const Mat3<T> &B)
	{
		return
			A[0][0] == B[0][0] && A[0][1] == B[0][1] && A[0][2] == B[0][2] &&
			A[1][0] == B[1][0] && A[1][1] == B[1][1] && A[1][2] == B[1][2] &&
			A[2][0] == B[2][0] && A[2][1] == B[2][1] && A[2][2] == B[2][2];
	}

	template <typename T>
	inline static bool
	operator!=(const Mat3<T> &A, const Mat3<T> &B)
	{
		return !(A == B);
	}

	template <typename T>
	inline static Mat3<T>
	operator+(const Mat3<T> &A, const Mat3<T> &B)
	{
		return {
			A[0][0] + B[0][0], A[0][1] + B[0][1], A[0][2] + B[0][2],
			A[1][0] + B[1][0], A[1][1] + B[1][1], A[1][2] + B[1][2],
			A[2][0] + B[2][0], A[2][1] + B[2][1], A[2][2] + B[2][2]
		};
	}

	template <typename T>
	inline static Mat3<T> &
	operator+=(Mat3<T> &A, const Mat3<T> &B)
	{
		A = A + B;
		return A;
	}

	template <typename T>
	inline static Mat3<T>
	operator-(const Mat3<T> &M)
	{
		return {
			-M[0][0], -M[0][1], -M[0][2],
			-M[1][0], -M[1][1], -M[1][2],
			-M[2][0], -M[2][1], -M[2][2]
		};
	}

	template <typename T>
	inline static Mat3<T>
	operator-(const Mat3<T> &A, const Mat3<T> &B)
	{
		return {
			A[0][0] - B[0][0], A[0][1] - B[0][1], A[0][2] - B[0][2],
			A[1][0] - B[1][0], A[1][1] - B[1][1], A[1][2] - B[1][2],
			A[2][0] - B[2][0], A[2][1] - B[2][1], A[2][2] - B[2][2]
		};
	}

	template <typename T>
	inline static Mat3<T> &
	operator-=(Mat3<T> &A, const Mat3<T> &B)
	{
		A = A - B;
		return A;
	}

	template <typename T>
	inline static Mat3<T>
	operator*(const Mat3<T> &M, T f)
	{
		return {
			M[0][0] * f, M[0][1] * f, M[0][2] * f,
			M[1][0] * f, M[1][1] * f, M[1][2] * f,
			M[2][0] * f, M[2][1] * f, M[2][2] * f
		};
	}

	template <typename T>
	inline static Mat3<T>
	operator*(T f, const Mat3<T> &M)
	{
		return M * f;
	}

	template <typename T>
	inline static Mat3<T> &
	operator*=(Mat3<T> &M, T f)
	{
		M = M * f;
		return M;
	}

	template <typename T>
	inline static Vec3<T>
	operator*(const Vec3<T> &v, const Mat3<T> &M)
	{
		return {
			v.x * M[0][0] + v.y * M[1][0] + v.z * M[2][0],
			v.x * M[0][1] + v.y * M[1][1] + v.z * M[2][1],
			v.x * M[0][2] + v.y * M[1][2] + v.z * M[2][2]
		};
	}

	template <typename T>
	inline static Mat3<T>
	operator*(const Mat3<T> &A, const Mat3<T> &B)
	{
		Mat3<T> C;

		C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0];
		C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1];
		C[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2];

		C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0];
		C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1];
		C[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2];

		C[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0];
		C[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1];
		C[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2];

		return C;
	}

	template <typename T>
	inline static Mat3<T>
	operator/(const Mat3<T> &M, T f)
	{
		return {
			M[0][0] / f, M[0][1] / f, M[0][2] / f,
			M[1][0] / f, M[1][1] / f, M[1][2] / f,
			M[2][0] / f, M[2][1] / f, M[2][2] / f
		};
	}

	template <typename T>
	inline static Mat3<T>
	operator/(T f, const Mat3<T> &M)
	{
		return M / f;
	}

	template <typename T>
	inline static Mat3<T> &
	operator/=(Mat3<T> &M, T f)
	{
		M = M / f;
		return M;
	}

	template <typename T>
	inline static Mat3<T>
	mat3_identity()
	{
		return {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_translation_2d(T dx, T dy)
	{
		return {
			 1,  0, 0,
			 0,  1, 0,
			dx, dy, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_translation_2d(const Vec2<T> &translation)
	{
		return mat3_translation_2d(translation.x, translation.y);
	}

	template <typename T>
	inline static Mat3<T>
	mat3_rotation_2d(T theta)
	{
		T c = cos(theta);
		T s = sin(theta);

		return {
			 c, s, 0,
			-s, c, 0,
			 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_scaling_2d(T sx, T sy)
	{
		return {
			sx,  0, 0,
			 0, sy, 0,
			 0,  0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_scaling_2d(Vec2<T> s)
	{
		return mat3_scaling_2d(s.x, s.y);
	}

	template <typename T>
	inline static Mat3<T>
	mat3_rotation_x(T pitch)
	{
		T c = cos(pitch);
		T s = sin(pitch);

		return {
			1,  0, 0,
			0,  c, s,
			0, -s, c};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_rotation_y(T yaw)
	{
		T c = cos(yaw);
		T s = sin(yaw);

		return {
			c, 0, -s,
			0, 1,  0,
			s, 0,  c};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_rotation_z(T roll)
	{
		T c = cos(roll);
		T s = sin(roll);

		return {
			 c, s, 0,
			-s, c, 0,
			 0, 0, 1};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_rotation_axis(const Vec3<T> &axis, T angle)
	{
		T c = cos(angle);
		T s = sin(angle);

		T x = axis.x;
		T y = axis.y;
		T z = axis.z;

		return {
			c + (1-c)*x*x  , (1-c)*x*y + s*z, (1-c)*x*z - s*y,
			(1-c)*x*y - s*z, c + (1-c)*y*y  , (1-c)*y*z + s*x,
			(1-c)*x*z + s*y, (1-c)*y*z - s*x, c + (1-c)*z*z
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_euler(T pitch, T head, T roll)
	{
		T sh = sin(head);
		T ch = cos(head);
		T sp = sin(pitch);
		T cp = cos(pitch);
		T sr = sin(roll);
		T cr = cos(roll);

		// order yxz
		return {
			 cr*ch - sr*sp*sh, sr*ch + cr*sp*sh, -cp*sh,
			-sr*cp           , cr*cp           ,  sp   ,
			 cr*sh + sr*sp*ch, sr*sh - cr*sp*ch,  cp*ch
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_euler(const Vec3<T> &rotation)
	{
		return mat3_euler(rotation.x, rotation.y, rotation.z);
	}

	template <typename T>
	inline static Vec3<T>
	mat3_euler_angles(const Mat3<T> &E)
	{
		return {
			asin(E[1][2]),
			atan2(-E[0][2], E[2][2]),
			atan2(-E[1][0], E[1][1])
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_scaling(T sx, T sy, T sz)
	{
		return {
			sx,  0,  0,
			 0, sy,  0,
			 0,  0, sz};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_scaling(const Vec3<T> &scaling)
	{
		return mat3_scaling(scaling.x, scaling.y, scaling.z);
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_xy(T s)
	{
		return {
			1, 0, 0,
			s, 1, 0,
			0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_xz(T s)
	{
		return {
			1, 0, 0,
			0, 1, 0,
			s, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_yx(T s)
	{
		return {
			1, s, 0,
			0, 1, 0,
			0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_yz(T s)
	{
		return {
			1, 0, 0,
			0, 1, 0,
			0, s, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_zx(T s)
	{
		return {
			1, 0, s,
			0, 1, 0,
			0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	mat3_shearing_zy(T s)
	{
		return {
			1, 0, 0,
			0, 1, s,
			0, 0, 1
		};
	}

	template <typename T>
	inline static Mat3<T>
	transpose(const Mat3<T> &M)
	{
		return {
			M[0][0], M[1][0], M[2][0],
			M[0][1], M[1][1], M[2][1],
			M[0][2], M[1][2], M[2][2]
		};
	}

	template <typename T>
	inline static T
	trace(const Mat3<T> &M)
	{
		return M[0][0] + M[1][1] + M[2][2];
	}

	template <typename T>
	inline static T
	det(const Mat3<T> &M)
	{
		return
			+ M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
			- M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0])
			+ M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
	}

	template <typename T>
	inline static Mat3<T>
	adj(const Mat3<T> &M)
	{
		return {
			//[0][0]
			+ (M[1][1] * M[2][2] - M[1][2] * M[2][1]),
			//[1][0]
			- (M[0][1] * M[2][2] - M[0][2] * M[2][1]),
			//[2][0]
			+ (M[0][1] * M[1][2] - M[0][2] * M[1][1]),

			//[0][1]
			- (M[1][0] * M[2][2] - M[1][2] * M[2][0]),
			//[1][1]
			+ (M[0][0] * M[2][2] - M[0][2] * M[2][0]),
			//[2][1]
			- (M[0][0] * M[1][2] - M[0][2] * M[1][0]),

			//[0][2]
			+ (M[1][0] * M[2][1] - M[1][1] * M[2][0]),
			//[1][2]
			- (M[0][0] * M[2][1] - M[0][1] * M[2][0]),
			//[2][2]
			+ (M[0][0] * M[1][1] - M[0][1] * M[1][0])
		};
	}

	template <typename T>
	inline static bool
	invertible(const Mat3<T> &M)
	{
		return det(M) != 0.0f;
	}

	template <typename T>
	inline static Mat3<T>
	inverse(const Mat3<T> &M)
	{
		T d = det(M);
		if (d == 0)
			return {};

		return (1.0f / d) * adj(M);
	}
}
