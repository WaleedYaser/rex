#pragma once

#include "rex-math/types.h"
#include "rex-math/math.h"

namespace rex::math
{
	template <typename T>
	inline static bool
	operator==(const Mat4<T> &A, const Mat4<T> &B)
	{
		return
			A[0][0] == B[0][0] && A[0][1] == B[0][1] && A[0][2] == B[0][2] && A[0][3] == B[0][3] &&
			A[1][0] == B[1][0] && A[1][1] == B[1][1] && A[1][2] == B[1][2] && A[1][3] == B[1][3] &&
			A[2][0] == B[2][0] && A[2][1] == B[2][1] && A[2][2] == B[2][2] && A[2][3] == B[2][3] &&
			A[3][0] == B[3][0] && A[3][1] == B[3][1] && A[3][2] == B[3][2] && A[3][3] == B[3][3];
	}

	template <typename T>
	inline static bool
	operator!=(const Mat4<T> &A, const Mat4<T> &B)
	{
		return !(A == B);
	}

	template <typename T>
	inline static Mat4<T>
	operator+(const Mat4<T> &A, const Mat4<T> &B)
	{
		return {
			A[0][0] + B[0][0], A[0][1] + B[0][1], A[0][2] + B[0][2], A[0][3] + B[0][3],
			A[1][0] + B[1][0], A[1][1] + B[1][1], A[1][2] + B[1][2], A[1][3] + B[1][3],
			A[2][0] + B[2][0], A[2][1] + B[2][1], A[2][2] + B[2][2], A[2][3] + B[2][3],
			A[3][0] + B[3][0], A[3][1] + B[3][1], A[3][2] + B[3][2], A[3][3] + B[3][3]
		};
	}

	template <typename T>
	inline static Mat4<T> &
	operator+=(Mat4<T> &A, const Mat4<T> &B)
	{
		A = A + B;
		return A;
	}

	template <typename T>
	inline static Mat4<T>
	operator-(const Mat4<T> &M)
	{
		return {
			-M[0][0], -M[0][1], -M[0][2], -M[0][3],
			-M[1][0], -M[1][1], -M[1][2], -M[1][3],
			-M[2][0], -M[2][1], -M[2][2], -M[2][3],
			-M[3][0], -M[3][1], -M[3][2], -M[3][3]
		};
	}

	template <typename T>
	inline static Mat4<T>
	operator-(const Mat4<T> &A, const Mat4<T> &B)
	{
		return {
			A[0][0] - B[0][0], A[0][1] - B[0][1], A[0][2] - B[0][2], A[0][3] - B[0][3],
			A[1][0] - B[1][0], A[1][1] - B[1][1], A[1][2] - B[1][2], A[1][3] - B[1][3],
			A[2][0] - B[2][0], A[2][1] - B[2][1], A[2][2] - B[2][2], A[2][3] - B[2][3],
			A[3][0] - B[3][0], A[3][1] - B[3][1], A[3][2] - B[3][2], A[3][3] - B[3][3]
		};
	}

	template <typename T>
	inline static Mat4<T> &
	operator-=(Mat4<T> &A, const Mat4<T> &B)
	{
		A = A - B;
		return A;
	}

	template <typename T>
	inline static Mat4<T>
	operator*(const Mat4<T> &M, T f)
	{
		return {
			M[0][0] * f, M[0][1] * f, M[0][2] * f, M[0][3] * f,
			M[1][0] * f, M[1][1] * f, M[1][2] * f, M[1][3] * f,
			M[2][0] * f, M[2][1] * f, M[2][2] * f, M[2][3] * f,
			M[3][0] * f, M[3][1] * f, M[3][2] * f, M[3][3] * f
		};
	}

	template <typename T>
	inline static Mat4<T>
	operator*(T f, const Mat4<T> &M)
	{
		return M * f;
	}

	template <typename T>
	inline static Mat4<T> &
	operator*=(Mat4<T> &M, T f)
	{
		M = M * f;
		return M;
	}

	template <typename T>
	inline static Vec4<T>
	operator*(const Vec4<T> &v, const Mat4<T> &M)
	{
		return {
			v.x * M[0][0] + v.y * M[1][0] + v.z * M[2][0] + v.w * M[3][0],
			v.x * M[0][1] + v.y * M[1][1] + v.z * M[2][1] + v.w * M[3][1],
			v.x * M[0][2] + v.y * M[1][2] + v.z * M[2][2] + v.w * M[3][2],
			v.x * M[0][3] + v.y * M[1][3] + v.z * M[2][3] + v.w * M[3][3]
		};
	}

	template <typename T>
	inline static Mat4<T>
	operator*(const Mat4<T> &A, const Mat4<T> &B)
	{
		Mat4<T> C;

		C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];
		C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];
		C[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];
		C[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];

		C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];
		C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];
		C[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];
		C[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];

		C[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];
		C[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];
		C[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];
		C[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];

		C[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];
		C[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];
		C[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];
		C[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];

		return C;
	}

	template <typename T>
	inline static Mat4<T>
	operator/(const Mat4<T> &M, T f)
	{
		return {
			M[0][0] / f, M[0][1] / f, M[0][2] / f, M[0][3] / f,
			M[1][0] / f, M[1][1] / f, M[1][2] / f, M[1][3] / f,
			M[2][0] / f, M[2][1] / f, M[2][2] / f, M[2][3] / f,
			M[3][0] / f, M[3][1] / f, M[3][2] / f, M[3][3] / f
		};
	}

	template <typename T>
	inline static Mat4<T>
	operator/(T f, const Mat4<T> &M)
	{
		return M / f;
	}

	template <typename T>
	inline static Mat4<T> &
	operator/=(Mat4<T> &M, T f)
	{
		M = M / f;
		return M;
	}

	template <typename T>
	inline static Mat4<T>
	mat4_identity()
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_translation(T tx, T ty, T tz)
	{
		return {
			1,  0,  0, 0,
			0,  1,  0, 0,
			0,  0,  1, 0,
			tx, ty, tz, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_translation(const Vec3<T> &translation)
	{
		return mat4_translation(translation.x, translation.y, translation.z);
	}

	template <typename T>
	inline static Mat4<T>
	mat4_rotation_x(T pitch)
	{
		T c = cos(pitch);
		T s = sin(pitch);

		return {
			1,  0, 0, 0,
			0,  c, s, 0,
			0, -s, c, 0,
			0,  0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_rotation_y(T head)
	{
		T c = cos(head);
		T s = sin(head);

		return {
			c, 0, -s, 0,
			0, 1,  0, 0,
			s, 0,  c, 0,
			0, 0,  0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_rotation_z(T roll)
	{
		T c = cos(roll);
		T s = sin(roll);

		return {
			 c, s, 0, 0,
			-s, c, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_rotation_axis(const Vec3<T> &axis, T angle)
	{
		T c = cos(angle);
		T s = sin(angle);

		T x = axis.x;
		T y = axis.y;
		T z = axis.z;

		return {
			c + (1-c)*x*x  , (1-c)*x*y + s*z, (1-c)*x*z - s*y, 0.0f,
			(1-c)*x*y - s*z, c + (1-c)*y*y  , (1-c)*y*z + s*x, 0.0f,
			(1-c)*x*z + s*y, (1-c)*y*z - s*x, c + (1-c)*z*z  , 0.0f,
			0.0f           ,            0.0f,            0.0f, 1.0f
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_euler(T pitch, T head, T roll)
	{
		T sh = sin(head);
		T ch = cos(head);
		T sp = sin(pitch);
		T cp = cos(pitch);
		T sr = sin(roll);
		T cr = cos(roll);

		// order yxz
		return {
			 cr*ch - sr*sp*sh, sr*ch + cr*sp*sh, -cp*sh, 0.0f,
			-sr*cp           , cr*cp           ,  sp   , 0.0f,
			 cr*sh + sr*sp*ch, sr*sh - cr*sp*ch,  cp*ch, 0.0f,
			 0.0f            , 0.0f            ,  0.0f  ,1.0f
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_euler(const Vec3<T> &rotation)
	{
		return mat4_euler(rotation.x, rotation.y, rotation.z);
	}

	template <typename T>
	inline static Vec3<T>
	mat4_euler_angles(const Mat4<T> &E)
	{
		return {
			asin(E[1][2]),
			atan2(-E[0][2], E[2][2]),
			atan2(-E[1][0], E[1][1])
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_scaling(T sx, T sy, T sz)
	{
		return {
			sx,  0,  0, 0,
			 0, sy,  0, 0,
			 0,  0, sz, 0,
			 0,  0,  0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_scaling(const Vec3<T> &scaling)
	{
		return mat4_scaling(scaling.x, scaling.y, scaling.z);
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_xy(T s)
	{
		return {
			1, 0, 0, 0,
			s, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_xz(T s)
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			s, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_yx(T s)
	{
		return {
			1, s, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_yz(T s)
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, s, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_zx(T s)
	{
		return {
			1, 0, s, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_shearing_zy(T s)
	{
		return {
			1, 0, 0, 0,
			0, 1, s, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	inline static Mat4<T>
	mat4_look_at(const Vec3<T> &eye, const Vec3<T> &target, const Vec3<T> &up)
	{
		Vec3<T> axis_z = normalize(eye - target);
		Vec3<T> axis_x = normalize(cross(up, axis_z));
		Vec3<T> axis_y = cross(axis_z, axis_x);

		Vec3<T> t = {
			-dot(eye, axis_x),
			-dot(eye, axis_y),
			-dot(eye, axis_z)
		};

		return {
			axis_x.x, axis_y.x, axis_z.x, 0.0f,
			axis_x.y, axis_y.y, axis_z.y, 0.0f,
			axis_x.z, axis_y.z, axis_z.z, 0.0f,
				 t.x,      t.y,      t.z, 1.0f
		};
	}

	// TODO[Waleed]: add unittests
	template <typename T>
	inline static Mat4<T>
	mat4_ortho(T left, T right, T bottom, T top, T znear, T zfar)
	{
		Mat4<T> M{};

		M[0][0] = 2.0f / (right - left);
		M[3][0] = -(right + left) / (right - left);

		M[1][1] = 2.0f / (top - bottom);
		M[3][1] = -(top + bottom) / (top - bottom);

		// to map z to [-1, 1] use
		// M[2][2] = -2.0f / (zfar - znear)
		// M[3][2] = -(zfar + znear) / (zfar - znear)
		M[2][2] = -1.0f / (zfar - znear);
		M[3][2] = -znear / (zfar - znear);

		M[3][3] = 1.0f;

		return M;
	}

	// TODO[Waleed]: add unittests
	template <typename T>
	inline static Mat4<T>
	mat4_perspective(T fovy, T aspect, T znear, T zfar)
	{
		Mat4<T> M{};

		T h = tan(fovy / 2.0f);
		T w = aspect * h;

		M[0][0] = 1.0f / w;
		M[1][1] = 1.0f / h;

		// to map z to [-1, 1] use
		// M[2][2] = -(zfar + znear) / (zfar - znear)
		// M[3][2] = -(2.0f * zfar * znear) / (zfar - znear)
		M[2][2] = -zfar / (zfar - znear);
		M[2][3] = -1.0f;
		M[3][2] = -(zfar * znear) / (zfar - znear);

		return M;
	}

	// TODO: add unittets
	template <typename T>
	inline static Mat4<T>
	mat4_viewport(T width, T height)
	{
		// TODO:
		return mat4_identity<T>();
	}


	template <typename T>
	inline static Mat4<T>
	transpose(const Mat4<T> &M)
	{
		return {
			M[0][0], M[1][0], M[2][0], M[3][0],
			M[0][1], M[1][1], M[2][1], M[3][1],
			M[0][2], M[1][2], M[2][2], M[3][2],
			M[0][3], M[1][3], M[2][3], M[3][3]
		};
	}

	template <typename T>
	inline static T
	trace(const Mat4<T> &M)
	{
		return M[0][0] + M[1][1] + M[2][2] + M[3][3];
	}

	template <typename T>
	inline static T
	det(const Mat4<T> &M)
	{
		/*
		* before optimization:
			+ M[0][0] * (
				+ M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
				- M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
				+ M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]))
			- M[0][1] * (
				+ M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
				- M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
				+ M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]))
			+ M[0][2] * (
				+ M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
				- M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
				+ M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]))
			- M[0][3] * (
				+ M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])
				- M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])
				+ M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
		*/
		return
			(M[0][0] * M[1][1] - M[0][1] * M[1][0]) * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			(M[0][0] * M[1][2] - M[0][2] * M[1][0]) * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
			(M[0][0] * M[1][3] - M[0][3] * M[1][0]) * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) +
			(M[0][1] * M[1][2] - M[0][2] * M[1][1]) * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) -
			(M[0][1] * M[1][3] - M[0][3] * M[1][1]) * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
			(M[0][2] * M[1][3] - M[0][3] * M[1][2]) * (M[2][0] * M[3][1] - M[2][1] * M[3][0]);
	}

	template <typename T>
	inline static Mat4<T>
	adj(const Mat4<T> &M)
	{
		return {
			// m00
			+ M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
			- M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
			+ M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]),
			// m10
			- M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
			+ M[0][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
			- M[0][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]),
			// m20
			+ M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2])
			- M[0][2] * (M[1][1] * M[3][3] - M[1][3] * M[3][1])
			+ M[0][3] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]),
			// m30
			- M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
			+ M[0][2] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
			- M[0][3] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]),

			// m01
			- M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
			+ M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
			- M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]),
			// m11
			+ M[0][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
			- M[0][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
			+ M[0][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]),
			// m21
			- M[0][0] * (M[1][2] * M[3][3] - M[1][3] * M[3][2])
			+ M[0][2] * (M[1][0] * M[3][3] - M[1][3] * M[3][0])
			- M[0][3] * (M[1][0] * M[3][2] - M[1][2] * M[3][0]),
			// m31
			+ M[0][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
			- M[0][2] * (M[1][0] * M[2][3] - M[1][3] * M[2][0])
			+ M[0][3] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]),

			// m02
			+ M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
			- M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
			+ M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]),
			// m12
			- M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
			+ M[0][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
			- M[0][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]),
			// m22
			+ M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1])
			- M[0][1] * (M[1][0] * M[3][3] - M[1][3] * M[3][0])
			+ M[0][3] * (M[1][0] * M[3][1] - M[1][1] * M[3][0]),
			// m32
			- M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
			+ M[0][1] * (M[1][0] * M[2][3] - M[1][3] * M[2][0])
			- M[0][3] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]),

			// m03
			- M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])
			+ M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])
			- M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]),
			// m13
			+ M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])
			- M[0][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])
			+ M[0][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]),
			// m23
			- M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1])
			+ M[0][1] * (M[1][0] * M[3][2] - M[1][2] * M[3][0])
			- M[0][2] * (M[1][0] * M[3][1] - M[1][1] * M[3][0]),
			// m33
			+ M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
			- M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0])
			+ M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0])
		};
	}

	template <typename T>
	inline static bool
	invertible(const Mat4<T> &M)
	{
		return det(M) != 0.0f;
	}

	template <typename T>
	inline static Mat4<T>
	inverse(const Mat4<T> &M)
	{
		T d = det(M);
		if (d == 0)
			return {};

		return (1.0f / d) * adj(M);
	}
}
