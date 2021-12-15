#include <rex-math/mat4.h>
#include <rex-math/vec3.h>
#include <rex-math/vec4.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Mat4")
{
	SUBCASE("init")
	{
		rex::math::M4 M = {};

		CHECK(M[0][0] == 0.0f); CHECK(M[0][1] == 0.0f); CHECK(M[0][2] == 0.0f); CHECK(M[0][3] == 0.0f);
		CHECK(M[1][0] == 0.0f); CHECK(M[1][1] == 0.0f); CHECK(M[1][2] == 0.0f); CHECK(M[1][3] == 0.0f);
		CHECK(M[2][0] == 0.0f); CHECK(M[2][1] == 0.0f); CHECK(M[2][2] == 0.0f); CHECK(M[2][3] == 0.0f);
		CHECK(M[3][0] == 0.0f); CHECK(M[3][1] == 0.0f); CHECK(M[3][2] == 0.0f); CHECK(M[3][3] == 0.0f);

		M = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		CHECK(M[0][0] ==  1.0f); CHECK(M[0][1] ==  2.0f); CHECK(M[0][2] ==  3.0f); CHECK(M[0][3] ==  4.0f);
		CHECK(M[1][0] ==  5.0f); CHECK(M[1][1] ==  6.0f); CHECK(M[1][2] ==  7.0f); CHECK(M[1][3] ==  8.0f);
		CHECK(M[2][0] ==  9.0f); CHECK(M[2][1] == 10.0f); CHECK(M[2][2] == 11.0f); CHECK(M[2][3] == 12.0f);
		CHECK(M[3][0] == 13.0f); CHECK(M[3][1] == 14.0f); CHECK(M[3][2] == 15.0f); CHECK(M[3][3] == 16.0f);
	}

	SUBCASE("equal")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 C = {
			21.0f, 22.0f, 23.0f, 24.0f,
			25.0f, 26.0f, 27.0f, 28.0f,
			29.0f, 30.0f, 31.0f, 32.0f,
			33.0f, 34.0f, 35.0f, 36.0f
		};

		CHECK(A == B);
		CHECK(A != C);
	}

	SUBCASE("add")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = {
			21.0f, 22.0f, 23.0f, 24.0f,
			25.0f, 26.0f, 27.0f, 28.0f,
			29.0f, 30.0f, 31.0f, 32.0f,
			33.0f, 34.0f, 35.0f, 36.0f
		};

		rex::math::M4 C = A + B;
		CHECK(C[0][0] == A[0][0] + B[0][0]);
		CHECK(C[0][1] == A[0][1] + B[0][1]);
		CHECK(C[0][2] == A[0][2] + B[0][2]);
		CHECK(C[0][3] == A[0][3] + B[0][3]);

		CHECK(C[1][0] == A[1][0] + B[1][0]);
		CHECK(C[1][1] == A[1][1] + B[1][1]);
		CHECK(C[1][2] == A[1][2] + B[1][2]);
		CHECK(C[1][3] == A[1][3] + B[1][3]);

		CHECK(C[2][0] == A[2][0] + B[2][0]);
		CHECK(C[2][1] == A[2][1] + B[2][1]);
		CHECK(C[2][2] == A[2][2] + B[2][2]);
		CHECK(C[2][3] == A[2][3] + B[2][3]);

		CHECK(C[3][0] == A[3][0] + B[3][0]);
		CHECK(C[3][1] == A[3][1] + B[3][1]);
		CHECK(C[3][2] == A[3][2] + B[3][2]);
		CHECK(C[3][3] == A[3][3] + B[3][3]);

		A += B;
		CHECK(A == C);
	}

	SUBCASE("unary minus")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = -A;
		CHECK(B[0][0] == -A[0][0]); CHECK(B[0][1] == -A[0][1]); CHECK(B[0][2] == -A[0][2]); CHECK(B[0][3] == -A[0][3]);
		CHECK(B[1][0] == -A[1][0]); CHECK(B[1][1] == -A[1][1]); CHECK(B[1][2] == -A[1][2]); CHECK(B[1][3] == -A[1][3]);
		CHECK(B[2][0] == -A[2][0]); CHECK(B[2][1] == -A[2][1]); CHECK(B[2][2] == -A[2][2]); CHECK(B[2][3] == -A[2][3]);
		CHECK(B[3][0] == -A[3][0]); CHECK(B[3][1] == -A[3][1]); CHECK(B[3][2] == -A[3][2]); CHECK(B[3][3] == -A[3][3]);
	}

	SUBCASE("subtract")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = {
			21.0f, 22.0f, 23.0f, 24.0f,
			25.0f, 26.0f, 27.0f, 28.0f,
			29.0f, 30.0f, 31.0f, 32.0f,
			33.0f, 34.0f, 35.0f, 36.0f
		};

		rex::math::M4 C = A - B;
		CHECK(C[0][0] == A[0][0] - B[0][0]);
		CHECK(C[0][1] == A[0][1] - B[0][1]);
		CHECK(C[0][2] == A[0][2] - B[0][2]);
		CHECK(C[0][3] == A[0][3] - B[0][3]);

		CHECK(C[1][0] == A[1][0] - B[1][0]);
		CHECK(C[1][1] == A[1][1] - B[1][1]);
		CHECK(C[1][2] == A[1][2] - B[1][2]);
		CHECK(C[1][3] == A[1][3] - B[1][3]);

		CHECK(C[2][0] == A[2][0] - B[2][0]);
		CHECK(C[2][1] == A[2][1] - B[2][1]);
		CHECK(C[2][2] == A[2][2] - B[2][2]);
		CHECK(C[2][3] == A[2][3] - B[2][3]);

		CHECK(C[3][0] == A[3][0] - B[3][0]);
		CHECK(C[3][1] == A[3][1] - B[3][1]);
		CHECK(C[3][2] == A[3][2] - B[3][2]);
		CHECK(C[3][3] == A[3][3] - B[3][3]);

		A -= B;
		CHECK(A == C);
	}

	SUBCASE("multiply scalar")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		float f = 5.0f;

		rex::math::M4 B = A * f;
		CHECK(B[0][0] == f * A[0][0]);
		CHECK(B[0][1] == f * A[0][1]);
		CHECK(B[0][2] == f * A[0][2]);
		CHECK(B[0][3] == f * A[0][3]);

		CHECK(B[1][0] == f * A[1][0]);
		CHECK(B[1][1] == f * A[1][1]);
		CHECK(B[1][2] == f * A[1][2]);
		CHECK(B[1][3] == f * A[1][3]);

		CHECK(B[2][0] == f * A[2][0]);
		CHECK(B[2][1] == f * A[2][1]);
		CHECK(B[2][2] == f * A[2][2]);
		CHECK(B[2][3] == f * A[2][3]);

		CHECK(B[3][0] == f * A[3][0]);
		CHECK(B[3][1] == f * A[3][1]);
		CHECK(B[3][2] == f * A[3][2]);
		CHECK(B[3][3] == f * A[3][3]);

		B = f * A;
		CHECK(B[0][0] == f * A[0][0]);
		CHECK(B[0][1] == f * A[0][1]);
		CHECK(B[0][2] == f * A[0][2]);
		CHECK(B[0][3] == f * A[0][3]);

		CHECK(B[1][0] == f * A[1][0]);
		CHECK(B[1][1] == f * A[1][1]);
		CHECK(B[1][2] == f * A[1][2]);
		CHECK(B[1][3] == f * A[1][3]);

		CHECK(B[2][0] == f * A[2][0]);
		CHECK(B[2][1] == f * A[2][1]);
		CHECK(B[2][2] == f * A[2][2]);
		CHECK(B[2][3] == f * A[2][3]);

		CHECK(B[3][0] == f * A[3][0]);
		CHECK(B[3][1] == f * A[3][1]);
		CHECK(B[3][2] == f * A[3][2]);
		CHECK(B[3][3] == f * A[3][3]);

		A *= f;
		CHECK(A == B);
	}

	SUBCASE("multiply vector")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::M4 M = {
			11.0f, 12.0f, 13.0f, 14.0f,
			15.0f, 16.0f, 17.0f, 18.0f,
			19.0f, 20.0f, 21.0f, 22.0f,
			23.0f, 24.0f, 25.0f, 26.0f
		};

		rex::math::V4 b = a * M;
		CHECK(b.x == 190.0f);
		CHECK(b.y == 200.0f);
		CHECK(b.z == 210.0f);
		CHECK(b.w == 220.0f);
	}

	SUBCASE("multiply matrix")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = {
			21.0f, 22.0f, 23.0f, 24.0f,
			25.0f, 26.0f, 27.0f, 28.0f,
			29.0f, 30.0f, 31.0f, 32.0f,
			33.0f, 34.0f, 35.0f, 36.0f
		};

		rex::math::M4 C = A * B;
		CHECK(C[0][0] ==  290.0f); CHECK(C[0][1] ==  300.0f); CHECK(C[0][2] ==  310.0f); CHECK(C[0][3] ==  320.0f);
		CHECK(C[1][0] ==  722.0f); CHECK(C[1][1] ==  748.0f); CHECK(C[1][2] ==  774.0f); CHECK(C[1][3] ==  800.0f);
		CHECK(C[2][0] == 1154.0f); CHECK(C[2][1] == 1196.0f); CHECK(C[2][2] == 1238.0f); CHECK(C[2][3] == 1280.0f);
		CHECK(C[3][0] == 1586.0f); CHECK(C[3][1] == 1644.0f); CHECK(C[3][2] == 1702.0f); CHECK(C[3][3] == 1760.0f);
	}

	SUBCASE("divide")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		float f = 5.0f;

		rex::math::M4 B = A / f;
		CHECK(B[0][0] == A[0][0] / f);
		CHECK(B[0][1] == A[0][1] / f);
		CHECK(B[0][2] == A[0][2] / f);
		CHECK(B[0][3] == A[0][3] / f);

		CHECK(B[1][0] == A[1][0] / f);
		CHECK(B[1][1] == A[1][1] / f);
		CHECK(B[1][2] == A[1][2] / f);
		CHECK(B[1][3] == A[1][3] / f);

		CHECK(B[2][0] == A[2][0] / f);
		CHECK(B[2][1] == A[2][1] / f);
		CHECK(B[2][2] == A[2][2] / f);
		CHECK(B[2][3] == A[2][3] / f);

		CHECK(B[3][0] == A[3][0] / f);
		CHECK(B[3][1] == A[3][1] / f);
		CHECK(B[3][2] == A[3][2] / f);
		CHECK(B[3][3] == A[3][3] / f);

		A /= f;
		CHECK(A == B);
	}

	SUBCASE("identity")
	{
		rex::math::M4 I = rex::math::mat4_identity<float>();
		CHECK(I[0][0] == 1.0f); CHECK(I[0][1] == 0.0f); CHECK(I[0][2] == 0.0f); CHECK(I[0][3] == 0.0f);
		CHECK(I[1][0] == 0.0f); CHECK(I[1][1] == 1.0f); CHECK(I[1][2] == 0.0f); CHECK(I[1][3] == 0.0f);
		CHECK(I[2][0] == 0.0f); CHECK(I[2][1] == 0.0f); CHECK(I[2][2] == 1.0f); CHECK(I[2][3] == 0.0f);
		CHECK(I[3][0] == 0.0f); CHECK(I[3][1] == 0.0f); CHECK(I[3][2] == 0.0f); CHECK(I[3][3] == 1.0f);

		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = a * I;
		CHECK(b == a);

		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = I * A;
		CHECK(B == A);

		B = A * I;
		CHECK(B == A);
	}

	SUBCASE("transpose")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		rex::math::M4 B = rex::math::transpose(A);
		CHECK(B[0][0] == A[0][0]); CHECK(B[0][1] == A[1][0]); CHECK(B[0][2] == A[2][0]); CHECK(B[0][3] == A[3][0]);
		CHECK(B[1][0] == A[0][1]); CHECK(B[1][1] == A[1][1]); CHECK(B[1][2] == A[2][1]); CHECK(B[1][3] == A[3][1]);
		CHECK(B[2][0] == A[0][2]); CHECK(B[2][1] == A[1][2]); CHECK(B[2][2] == A[2][2]); CHECK(B[2][3] == A[3][2]);
		CHECK(B[3][0] == A[0][3]); CHECK(B[3][1] == A[1][3]); CHECK(B[3][2] == A[2][3]); CHECK(B[3][3] == A[3][3]);
	}

	SUBCASE("trace")
	{
		rex::math::M4 M = {
			 0.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		CHECK(rex::math::trace(M) == 33.0f);
	}

	SUBCASE("determinant")
	{
		rex::math::M4 I = rex::math::mat4_identity<float>();
		CHECK(rex::math::det(I) == 1.0f);

		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		CHECK(rex::math::det(A) == 0.0f);

		rex::math::M4 B = {
			 3.0f, 2.0f, -1.0f,  4.0f,
			 2.0f, 1.0f,  5.0f,  7.0f,
			 0.0f, 5.0f,  2.0f, -6.0f,
			-1.0f, 2.0f,  1.0f,  0.0f
		};

		CHECK(rex::math::det(B) == -418.0f);
	}

	SUBCASE("adjoint")
	{
		rex::math::M4 A = {
			 3.0f, 2.0f, -1.0f,  4.0f,
			 2.0f, 1.0f,  5.0f,  7.0f,
			 0.0f, 5.0f,  2.0f, -6.0f,
			-1.0f, 2.0f,  1.0f,  0.0f
		};

		rex::math::M4 B = rex::math::adj(A);
		CHECK(B[0][0] == -47.0f); CHECK(B[0][1] == -28.0f); CHECK(B[0][2] == -64.0f); CHECK(B[0][3] ==  221.0f);
		CHECK(B[1][0] == -56.0f); CHECK(B[1][1] ==  20.0f); CHECK(B[1][2] == -14.0f); CHECK(B[1][3] == -128.0f);
		CHECK(B[2][0] ==  65.0f); CHECK(B[2][1] == -68.0f); CHECK(B[2][2] == -36.0f); CHECK(B[2][3] ==   59.0f);
		CHECK(B[3][0] == -25.0f); CHECK(B[3][1] ==  -6.0f); CHECK(B[3][2] ==  46.0f); CHECK(B[3][3] ==  -87.0f);
	}

	SUBCASE("invertible")
	{
		rex::math::M4 A = {
			 1.0f,  2.0f,  3.0f,  4.0f,
			 5.0f,  6.0f,  7.0f,  8.0f,
			 9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		CHECK(rex::math::invertible(A) == false);

		rex::math::M4 B = {
			 3.0f, 2.0f, -1.0f,  4.0f,
			 2.0f, 1.0f,  5.0f,  7.0f,
			 0.0f, 5.0f,  2.0f, -6.0f,
			-1.0f, 2.0f,  1.0f,  0.0f
		};

		CHECK(rex::math::invertible(B) == true);
	}

	SUBCASE("inverse")
	{
		rex::math::M4 A = {
			 3.0f, 2.0f, -1.0f,  4.0f,
			 2.0f, 1.0f,  5.0f,  7.0f,
			 0.0f, 5.0f,  2.0f, -6.0f,
			-1.0f, 2.0f,  1.0f,  0.0f
		};

		rex::math::M4 B = rex::math::inverse(A);

		rex::math::M4 C = A * B;
		CHECK(C[0][0] == doctest::Approx(1.0f));
		CHECK(C[0][1] == doctest::Approx(0.0f));
		CHECK(C[0][2] == doctest::Approx(0.0f));
		CHECK(C[0][3] == doctest::Approx(0.0f));

		CHECK(C[1][0] == doctest::Approx(0.0f));
		CHECK(C[1][1] == doctest::Approx(1.0f));
		CHECK(C[1][2] == doctest::Approx(0.0f));
		CHECK(C[1][3] == doctest::Approx(0.0f));

		CHECK(C[2][0] == doctest::Approx(0.0f));
		CHECK(C[2][1] == doctest::Approx(0.0f));
		CHECK(C[2][2] == doctest::Approx(1.0f));
		CHECK(C[2][3] == doctest::Approx(0.0f));

		CHECK(C[3][0] == doctest::Approx(0.0f));
		CHECK(C[3][1] == doctest::Approx(0.0f));
		CHECK(C[3][2] == doctest::Approx(0.0f));
		CHECK(C[3][3] == doctest::Approx(1.0f));

		C = B * A;
		CHECK(C[0][0] == doctest::Approx(1.0f));
		CHECK(C[0][1] == doctest::Approx(0.0f));
		CHECK(C[0][2] == doctest::Approx(0.0f));
		CHECK(C[0][3] == doctest::Approx(0.0f));

		CHECK(C[1][0] == doctest::Approx(0.0f));
		CHECK(C[1][1] == doctest::Approx(1.0f));
		CHECK(C[1][2] == doctest::Approx(0.0f));
		CHECK(C[1][3] == doctest::Approx(0.0f));

		CHECK(C[2][0] == doctest::Approx(0.0f));
		CHECK(C[2][1] == doctest::Approx(0.0f));
		CHECK(C[2][2] == doctest::Approx(1.0f));
		CHECK(C[2][3] == doctest::Approx(0.0f));

		CHECK(C[3][0] == doctest::Approx(0.0f));
		CHECK(C[3][1] == doctest::Approx(0.0f));
		CHECK(C[3][2] == doctest::Approx(0.0f));
		CHECK(C[3][3] == doctest::Approx(1.0f));

		rex::math::V4 a = {11.0f, 12.0f, 13.0f, 14.0f};
		rex::math::V4 b = a * A * B;
		CHECK(b.x == doctest::Approx(a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(a.z));
		CHECK(b.w == doctest::Approx(a.w));
	}

	SUBCASE("translation")
	{
		rex::math::V3 t = {10.0f, 20.0f, 30.0f};

		rex::math::M4 T = rex::math::mat4_translation(t.x, t.y, t.z);
		rex::math::V4 a = {1.0f, 1.0f, 1.0f, 1.0f};

		rex::math::V4 b = a * T;
		CHECK(b.x == a.x + t.x);
		CHECK(b.y == a.y + t.y);
		CHECK(b.z == a.z + t.z);
		CHECK(b.w == 1.0f);

		T = rex::math::mat4_translation(t);

		b = a * T;
		CHECK(b.x == a.x + t.x);
		CHECK(b.y == a.y + t.y);
		CHECK(b.z == a.z + t.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(T);
		CHECK(c == a);
	}

	SUBCASE("rotation x")
	{
		rex::math::M4 R = rex::math::mat4_rotation_x((float)rex::math::PI_DIV_2);
		rex::math::V4 a = {1.0f, 1.0f, 1.0f, 1.0f};

		rex::math::V4 b = a * R;
		CHECK(b.x == doctest::Approx(a.x));
		CHECK(b.y == doctest::Approx(-a.y));
		CHECK(b.z == doctest::Approx(a.z));
		CHECK(b.w == doctest::Approx(1.0f));

		rex::math::V4 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("rotation y")
	{
		rex::math::M4 R = rex::math::mat4_rotation_y((float)rex::math::PI_DIV_2);
		rex::math::V4 a = {1.0f, 1.0f, 1.0f, 1.0f};

		rex::math::V4 b = a * R;
		CHECK(b.x == doctest::Approx(a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(-a.z));
		CHECK(b.w == doctest::Approx(1.0f));

		rex::math::V4 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("rotation z")
	{
		rex::math::M4 R = rex::math::mat4_rotation_z((float)rex::math::PI_DIV_2);
		rex::math::V4 a = {1.0f, 1.0f, 1.0f, 1.0f};

		rex::math::V4 b = a * R;
		CHECK(b.x == doctest::Approx(-a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(a.z));
		CHECK(b.w == doctest::Approx(1.0f));

		rex::math::V4 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("rotation axis")
	{
		rex::math::M4 RX = rex::math::mat4_rotation_x((float)rex::math::PI_DIV_2);
		rex::math::M4 R = rex::math::mat4_rotation_axis(rex::math::V3{1.0f, 0.0f, 0.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RX[0][0]));
		CHECK(R[0][1] == doctest::Approx(RX[0][1]));
		CHECK(R[0][2] == doctest::Approx(RX[0][2]));
		CHECK(R[0][3] == doctest::Approx(RX[0][3]));

		CHECK(R[1][0] == doctest::Approx(RX[1][0]));
		CHECK(R[1][1] == doctest::Approx(RX[1][1]));
		CHECK(R[1][2] == doctest::Approx(RX[1][2]));
		CHECK(R[1][3] == doctest::Approx(RX[1][3]));

		CHECK(R[2][0] == doctest::Approx(RX[2][0]));
		CHECK(R[2][1] == doctest::Approx(RX[2][1]));
		CHECK(R[2][2] == doctest::Approx(RX[2][2]));
		CHECK(R[2][3] == doctest::Approx(RX[2][3]));

		CHECK(R[3][0] == doctest::Approx(RX[3][0]));
		CHECK(R[3][1] == doctest::Approx(RX[3][1]));
		CHECK(R[3][2] == doctest::Approx(RX[3][2]));
		CHECK(R[3][3] == doctest::Approx(RX[3][3]));

		rex::math::M4 RY = rex::math::mat4_rotation_y((float)rex::math::PI_DIV_2);
		R = rex::math::mat4_rotation_axis(rex::math::V3{0.0f, 1.0f, 0.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RY[0][0]));
		CHECK(R[0][1] == doctest::Approx(RY[0][1]));
		CHECK(R[0][2] == doctest::Approx(RY[0][2]));
		CHECK(R[0][3] == doctest::Approx(RY[0][3]));

		CHECK(R[1][0] == doctest::Approx(RY[1][0]));
		CHECK(R[1][1] == doctest::Approx(RY[1][1]));
		CHECK(R[1][2] == doctest::Approx(RY[1][2]));
		CHECK(R[1][3] == doctest::Approx(RY[1][3]));

		CHECK(R[2][0] == doctest::Approx(RY[2][0]));
		CHECK(R[2][1] == doctest::Approx(RY[2][1]));
		CHECK(R[2][2] == doctest::Approx(RY[2][2]));
		CHECK(R[2][3] == doctest::Approx(RY[2][3]));

		CHECK(R[3][0] == doctest::Approx(RY[3][0]));
		CHECK(R[3][1] == doctest::Approx(RY[3][1]));
		CHECK(R[3][2] == doctest::Approx(RY[3][2]));
		CHECK(R[3][3] == doctest::Approx(RY[3][3]));

		rex::math::M4 RZ = rex::math::mat4_rotation_z((float)rex::math::PI_DIV_2);
		R = rex::math::mat4_rotation_axis(rex::math::V3{0.0f, 0.0f, 1.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RZ[0][0]));
		CHECK(R[0][1] == doctest::Approx(RZ[0][1]));
		CHECK(R[0][2] == doctest::Approx(RZ[0][2]));
		CHECK(R[0][3] == doctest::Approx(RZ[0][3]));

		CHECK(R[1][0] == doctest::Approx(RZ[1][0]));
		CHECK(R[1][1] == doctest::Approx(RZ[1][1]));
		CHECK(R[1][2] == doctest::Approx(RZ[1][2]));
		CHECK(R[1][3] == doctest::Approx(RZ[1][3]));

		CHECK(R[2][0] == doctest::Approx(RZ[2][0]));
		CHECK(R[2][1] == doctest::Approx(RZ[2][1]));
		CHECK(R[2][2] == doctest::Approx(RZ[2][2]));
		CHECK(R[2][3] == doctest::Approx(RZ[2][3]));

		CHECK(R[3][0] == doctest::Approx(RZ[3][0]));
		CHECK(R[3][1] == doctest::Approx(RZ[3][1]));
		CHECK(R[3][2] == doctest::Approx(RZ[3][2]));
		CHECK(R[3][3] == doctest::Approx(RZ[3][3]));

		rex::math::V4 a = {0.0f ,1.0f, 2.0f, 0.0f};
		R = rex::math::mat4_rotation_axis(
				rex::math::normalize(rex::math::V3{1.0f, 1.0f, 1.0f}),
				(float)rex::math::PI / 4.0f
			);

		// online calculator: https://www.vcalc.com/wiki/vCalc/V3+-+Vector+Rotation
		rex::math::V4 b = a * R;
		CHECK(b.x == doctest::Approx(0.70114f));
		CHECK(b.y == doctest::Approx(0.1835f));
		CHECK(b.z == doctest::Approx(2.11536f));
		CHECK(b.w == doctest::Approx(0.0f));

		rex::math::V4 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(a.w));
	}

	// TODO[Waleed]: invistigate euler test case
	SUBCASE("euler")
	{
		// rex::math::V3 e = float(rex::math::TO_RADIAN) * rex::math::V3{30.0f, 45.0f, 0.0f};

		// rex::math::M4 E = rex::math::mat4_euler(e.x, e.y, e.z);
		// rex::math::V4 a = {0.0f, 0.0f, 1.0f, 1.0f};

		// rex::math::V4 b = a * E;
		// CHECK(b.x == 0.0f);
		// CHECK(b.y == doctest::Approx(-0.5f));
		// CHECK(b.z == 0.0f);
		// CHECK(b.w == 1.0f);
	}

	SUBCASE("scaling")
	{
		rex::math::V3 s = {10.0f, 20.0f, 30.0f};

		rex::math::M4 S = rex::math::mat4_scaling(s.x, s.y, s.z);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);
		CHECK(b.z == a.z * s.z);
		CHECK(b.w == 1.0f);

		S = rex::math::mat4_scaling(s);

		b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);
		CHECK(b.z == a.z * s.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(S);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing xy")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_xy(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x + a.y*s);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing xz")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_xz(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x + a.z*s);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing yx")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_yx(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y + a.x*s);
		CHECK(b.z == a.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing yz")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_yz(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y + a.z*s);
		CHECK(b.z == a.z);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing zx")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_zx(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z + a.x*s);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("shearing zy")
	{
		float s = 10.0f;

		rex::math::M4 H = rex::math::mat4_shearing_zy(s);
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 1.0f};

		rex::math::V4 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z + a.y*s);
		CHECK(b.w == 1.0f);

		rex::math::V4 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
		CHECK(c.w == doctest::Approx(1.0f));
	}

	SUBCASE("look at")
	{
		rex::math::M4 M = rex::math::mat4_look_at(
			rex::math::V3{0.0f, 0.0f, 10.0f}, rex::math::V3{}, rex::math::V3{0.0f, 1.0f, 0.0f});
		rex::math::V4 a = {0.0f, 0.0f, 0.0f, 1.0f};

		rex::math::V4 b = a * M;
		CHECK(b.x == doctest::Approx(0.0f));
		CHECK(b.y == doctest::Approx(0.0f));
		CHECK(b.z == doctest::Approx(-10.0f));
		CHECK(b.w == doctest::Approx(1.0f));

		M = rex::math::mat4_look_at(rex::math::V3{0.0f, 10.0f, 0.0f}, rex::math::V3{}, rex::math::V3{1.0f, 0.0f, 0.0f});

		b = a * M;
		CHECK(b.x == doctest::Approx(0.0f));
		CHECK(b.y == doctest::Approx(0.0f));
		CHECK(b.z == doctest::Approx(-10.0f));
		CHECK(b.w == doctest::Approx(1.0f));

		M = rex::math::mat4_look_at(rex::math::V3{10.0f, 0.0f, 0.0f}, rex::math::V3{}, rex::math::V3{0.0f, 1.0f, 0.0f});

		b = a * M;
		CHECK(b.x == doctest::Approx(0.0f));
		CHECK(b.y == doctest::Approx(0.0f));
		CHECK(b.z == doctest::Approx(-10.0f));
		CHECK(b.w == doctest::Approx(1.0f));
	}

	SUBCASE("orthographic projection")
	{
		// TODO[Waleed]: implement
	}

	SUBCASE("perspective projection")
	{
		// TODO[Waleed]: implement
	}
}
