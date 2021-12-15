#include <rex-math/mat3.h>
#include <rex-math/vec3.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Mat3")
{
	SUBCASE("init")
	{
		rex::math::M3 M = {};

		CHECK(M[0][0] == 0.0f); CHECK(M[0][1] == 0.0f); CHECK(M[0][2] == 0.0f);
		CHECK(M[1][0] == 0.0f); CHECK(M[1][1] == 0.0f); CHECK(M[1][2] == 0.0f);
		CHECK(M[2][0] == 0.0f); CHECK(M[2][1] == 0.0f); CHECK(M[2][2] == 0.0f);

		M = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		CHECK(M[0][0] == 1.0f); CHECK(M[0][1] == 2.0f); CHECK(M[0][2] == 3.0f);
		CHECK(M[1][0] == 4.0f); CHECK(M[1][1] == 5.0f); CHECK(M[1][2] == 6.0f);
		CHECK(M[2][0] == 7.0f); CHECK(M[2][1] == 8.0f); CHECK(M[2][2] == 9.0f);
	}

	SUBCASE("equal")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 C = {
			11.0f, 12.0f, 13.0f,
			14.0f, 15.0f, 16.0f,
			17.0f, 18.0f, 19.0f
		};

		CHECK(A == B);
		CHECK(A != C);
	}

	SUBCASE("add")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = {
			11.0f, 12.0f, 13.0f,
			14.0f, 15.0f, 16.0f,
			17.0f, 18.0f, 19.0f
		};

		rex::math::M3 C = A + B;
		CHECK(C[0][0] == A[0][0] + B[0][0]); CHECK(C[0][1] == A[0][1] + B[0][1]); CHECK(C[0][2] == A[0][2] + B[0][2]);
		CHECK(C[1][0] == A[1][0] + B[1][0]); CHECK(C[1][1] == A[1][1] + B[1][1]); CHECK(C[1][2] == A[1][2] + B[1][2]);
		CHECK(C[2][0] == A[2][0] + B[2][0]); CHECK(C[2][1] == A[2][1] + B[2][1]); CHECK(C[2][2] == A[2][2] + B[2][2]);

		A += B;
		CHECK(A == C);
	}

	SUBCASE("unary minus")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = -A;
		CHECK(B[0][0] == -A[0][0]); CHECK(B[0][1] == -A[0][1]); CHECK(B[0][2] == -A[0][2]);
		CHECK(B[1][0] == -A[1][0]); CHECK(B[1][1] == -A[1][1]); CHECK(B[1][2] == -A[1][2]);
		CHECK(B[2][0] == -A[2][0]); CHECK(B[2][1] == -A[2][1]); CHECK(B[2][2] == -A[2][2]);
	}

	SUBCASE("subtract")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = {
			11.0f, 12.0f, 13.0f,
			14.0f, 15.0f, 16.0f,
			17.0f, 18.0f, 19.0f
		};

		rex::math::M3 C = A - B;
		CHECK(C[0][0] == A[0][0] - B[0][0]); CHECK(C[0][1] == A[0][1] - B[0][1]); CHECK(C[0][2] == A[0][2] - B[0][2]);
		CHECK(C[1][0] == A[1][0] - B[1][0]); CHECK(C[1][1] == A[1][1] - B[1][1]); CHECK(C[1][2] == A[1][2] - B[1][2]);
		CHECK(C[2][0] == A[2][0] - B[2][0]); CHECK(C[2][1] == A[2][1] - B[2][1]); CHECK(C[2][2] == A[2][2] - B[2][2]);

		A -= B;
		CHECK(A == C);
	}

	SUBCASE("multiply scalar")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		float f = 5.0f;

		rex::math::M3 B = A * f;
		CHECK(B[0][0] == f * A[0][0]); CHECK(B[0][1] == f * A[0][1]); CHECK(B[0][2] == f * A[0][2]);
		CHECK(B[1][0] == f * A[1][0]); CHECK(B[1][1] == f * A[1][1]); CHECK(B[1][2] == f * A[1][2]);
		CHECK(B[2][0] == f * A[2][0]); CHECK(B[2][1] == f * A[2][1]); CHECK(B[2][2] == f * A[2][2]);

		B = f * A;
		CHECK(B[0][0] == f * A[0][0]); CHECK(B[0][1] == f * A[0][1]); CHECK(B[0][2] == f * A[0][2]);
		CHECK(B[1][0] == f * A[1][0]); CHECK(B[1][1] == f * A[1][1]); CHECK(B[1][2] == f * A[1][2]);
		CHECK(B[2][0] == f * A[2][0]); CHECK(B[2][1] == f * A[2][1]); CHECK(B[2][2] == f * A[2][2]);

		A *= f;
		CHECK(A == B);
	}

	SUBCASE("multiply vector")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::M3 M = {
			11.0f, 12.0f, 13.0f,
			14.0f, 15.0f, 16.0f,
			17.0f, 18.0f, 19.0f
		};

		rex::math::V3 b = a * M;
		CHECK(b.x == 90.0f);
		CHECK(b.y == 96.0f);
		CHECK(b.z == 102.0f);
	}

	SUBCASE("multiply matrix")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = {
			11.0f, 12.0f, 13.0f,
			14.0f, 15.0f, 16.0f,
			17.0f, 18.0f, 19.0f
		};

		rex::math::M3 C = A * B;
		CHECK(C[0][0] ==  90.0f); CHECK(C[0][1] ==  96.0f); CHECK(C[0][2] == 102.0f);
		CHECK(C[1][0] == 216.0f); CHECK(C[1][1] == 231.0f); CHECK(C[1][2] == 246.0f);
		CHECK(C[2][0] == 342.0f); CHECK(C[2][1] == 366.0f); CHECK(C[2][2] == 390.0f);
	}

	SUBCASE("divide")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		float f = 5.0f;

		rex::math::M3 B = A / f;
		CHECK(B[0][0] == A[0][0] / f); CHECK(B[0][1] == A[0][1] / f); CHECK(B[0][2] == A[0][2] / f);
		CHECK(B[1][0] == A[1][0] / f); CHECK(B[1][1] == A[1][1] / f); CHECK(B[1][2] == A[1][2] / f);
		CHECK(B[2][0] == A[2][0] / f); CHECK(B[2][1] == A[2][1] / f); CHECK(B[2][2] == A[2][2] / f);

		A /= f;
		CHECK(A == B);
	}

	SUBCASE("identity")
	{
		rex::math::M3 I = rex::math::mat3_identity<float>();
		CHECK(I[0][0] == 1.0f); CHECK(I[0][1] == 0.0f); CHECK(I[0][2] == 0.0f);
		CHECK(I[1][0] == 0.0f); CHECK(I[1][1] == 1.0f); CHECK(I[1][2] == 0.0f);
		CHECK(I[2][0] == 0.0f); CHECK(I[2][1] == 0.0f); CHECK(I[2][2] == 1.0f);

		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = a * I;
		CHECK(b == a);

		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = I * A;
		CHECK(B == A);

		B = A * I;
		CHECK(B == A);
	}

	SUBCASE("transpose")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		rex::math::M3 B = rex::math::transpose(A);
		CHECK(B[0][0] == A[0][0]); CHECK(B[0][1] == A[1][0]); CHECK(B[0][2] == A[2][0]);
		CHECK(B[1][0] == A[0][1]); CHECK(B[1][1] == A[1][1]); CHECK(B[1][2] == A[2][1]);
		CHECK(B[2][0] == A[0][2]); CHECK(B[2][1] == A[1][2]); CHECK(B[2][2] == A[2][2]);
	}

	SUBCASE("trace")
	{
		rex::math::M3 M = {
			0.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		CHECK(rex::math::trace(M) == 14.0f);
	}

	SUBCASE("determinant")
	{
		rex::math::M3 I = rex::math::mat3_identity<float>();
		CHECK(rex::math::det(I) == 1.0f);

		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		CHECK(rex::math::det(A) == 0.0f);

		rex::math::M3 B = {
			3.0f, 2.0f, -1.0f,
			2.0f, 1.0f,  5.0f,
			0.0f, 5.0f,  2.0f
		};

		CHECK(rex::math::det(B) == -87.0f);
	}

	SUBCASE("adjoint")
	{
		rex::math::M3 A = {
			 3.0f, 2.0f, -1.0f,
			 2.0f, 1.0f,  5.0f,
			 0.0f, 5.0f,  2.0f
		};

		rex::math::M3 B = rex::math::adj(A);
		CHECK(B[0][0] == -23.0f); CHECK(B[0][1] ==  -9.0f); CHECK(B[0][2] ==  11.0f);
		CHECK(B[1][0] ==  -4.0f); CHECK(B[1][1] ==   6.0f); CHECK(B[1][2] == -17.0f);
		CHECK(B[2][0] ==  10.0f); CHECK(B[2][1] == -15.0f); CHECK(B[2][2] ==  -1.0f);
	}


	SUBCASE("invertible")
	{
		rex::math::M3 A = {
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f
		};

		CHECK(rex::math::invertible(A) == false);

		rex::math::M3 B = {
			3.0f, 2.0f, -1.0f,
			2.0f, 1.0f,  5.0f,
			0.0f, 5.0f,  2.0f
		};

		CHECK(rex::math::invertible(B) == true);
	}

	SUBCASE("inverse")
	{
		rex::math::M3 A = {
			3.0f, 2.0f, -1.0f,
			2.0f, 1.0f,  5.0f,
			0.0f, 5.0f,  2.0f
		};

		rex::math::M3 B = rex::math::inverse(A);

		rex::math::M3 C = A * B;
		CHECK(C[0][0] == doctest::Approx(1.0f));
		CHECK(C[0][1] == doctest::Approx(0.0f));
		CHECK(C[0][2] == doctest::Approx(0.0f));

		CHECK(C[1][0] == doctest::Approx(0.0f));
		CHECK(C[1][1] == doctest::Approx(1.0f));
		CHECK(C[1][2] == doctest::Approx(0.0f));

		CHECK(C[2][0] == doctest::Approx(0.0f));
		CHECK(C[2][1] == doctest::Approx(0.0f));
		CHECK(C[2][2] == doctest::Approx(1.0f));

		C = B * A;
		CHECK(C[0][0] == doctest::Approx(1.0f));
		CHECK(C[0][1] == doctest::Approx(0.0f));
		CHECK(C[0][2] == doctest::Approx(0.0f));

		CHECK(C[1][0] == doctest::Approx(0.0f));
		CHECK(C[1][1] == doctest::Approx(1.0f));
		CHECK(C[1][2] == doctest::Approx(0.0f));

		CHECK(C[2][0] == doctest::Approx(0.0f));
		CHECK(C[2][1] == doctest::Approx(0.0f));
		CHECK(C[2][2] == doctest::Approx(1.0f));

		rex::math::V3 a = {11.0f, 12.0f, 13.0f};
		rex::math::V3 b = a * A * B;
		CHECK(b == a);
	}

	SUBCASE("translation 2d")
	{
		rex::math::V2 p = {10.0f, 20.0f};

		rex::math::M3 P = rex::math::mat3_translation_2d(p.x, p.y);
		rex::math::V3 a = {1.0f, 1.0f, 1.0f};

		rex::math::V3 b = a * P;
		CHECK(b.x == a.x + p.x);
		CHECK(b.y == a.y + p.y);
		CHECK(b.z == 1.0f);

		P = rex::math::mat3_translation_2d(p);

		b = a * P;
		CHECK(b.x == a.x + p.x);
		CHECK(b.y == a.y + p.y);
		CHECK(b.z == 1.0f);

		rex::math::V3 c = b * rex::math::inverse(P);
		CHECK(c == a);
	}

	SUBCASE("rotation 2d")
	{
		rex::math::M3 R = rex::math::mat3_rotation_2d((float)rex::math::PI_DIV_2);
		rex::math::V3 a = {1.0f, 1.0f, 1.0f};

		rex::math::V3 b = a * R;
		CHECK(b.x == doctest::Approx(-a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(1.0f));

		rex::math::V3 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.y == doctest::Approx(1.0f));
	}

	SUBCASE("scaling 2d")
	{
		rex::math::V2 s = {10.0f, 20.0f};

		rex::math::M3 S = rex::math::mat3_scaling_2d(s.x, s.y);
		rex::math::V3 a = {1.0f, 2.0f, 1.0f};

		rex::math::V3 b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);

		S = rex::math::mat3_scaling_2d(s);

		b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);

		rex::math::V3 c = b * rex::math::inverse(S);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(1.0f));
	}

	SUBCASE("rotation x")
	{
		rex::math::M3 R = rex::math::mat3_rotation_x((float)rex::math::PI_DIV_2);
		rex::math::V3 a = {1.0f, 1.0f, 1.0f};

		rex::math::V3 b = a * R;
		CHECK(b.x == doctest::Approx(a.x));
		CHECK(b.y == doctest::Approx(-a.y));
		CHECK(b.z == doctest::Approx(a.z));

		rex::math::V3 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("rotation y")
	{
		rex::math::M3 R = rex::math::mat3_rotation_y((float)rex::math::PI_DIV_2);
		rex::math::V3 a = {1.0f, 1.0f, 1.0f};

		rex::math::V3 b = a * R;
		CHECK(b.x == doctest::Approx(a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(-a.z));

		rex::math::V3 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("rotation z")
	{
		rex::math::M3 R = rex::math::mat3_rotation_z((float)rex::math::PI_DIV_2);
		rex::math::V3 a = {1.0f, 1.0f, 1.0f};

		rex::math::V3 b = a * R;
		CHECK(b.x == doctest::Approx(-a.x));
		CHECK(b.y == doctest::Approx(a.y));
		CHECK(b.z == doctest::Approx(a.z));

		rex::math::V3 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("rotation axis")
	{
		rex::math::M3 RX = rex::math::mat3_rotation_x((float)rex::math::PI_DIV_2);
		rex::math::M3 R = rex::math::mat3_rotation_axis({1.0f, 0.0f, 0.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RX[0][0]));
		CHECK(R[0][1] == doctest::Approx(RX[0][1]));
		CHECK(R[0][2] == doctest::Approx(RX[0][2]));

		CHECK(R[1][0] == doctest::Approx(RX[1][0]));
		CHECK(R[1][1] == doctest::Approx(RX[1][1]));
		CHECK(R[1][2] == doctest::Approx(RX[1][2]));

		CHECK(R[2][0] == doctest::Approx(RX[2][0]));
		CHECK(R[2][1] == doctest::Approx(RX[2][1]));
		CHECK(R[2][2] == doctest::Approx(RX[2][2]));

		rex::math::M3 RY = rex::math::mat3_rotation_y((float)rex::math::PI_DIV_2);
		R = rex::math::mat3_rotation_axis({0.0f, 1.0f, 0.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RY[0][0]));
		CHECK(R[0][1] == doctest::Approx(RY[0][1]));
		CHECK(R[0][2] == doctest::Approx(RY[0][2]));

		CHECK(R[1][0] == doctest::Approx(RY[1][0]));
		CHECK(R[1][1] == doctest::Approx(RY[1][1]));
		CHECK(R[1][2] == doctest::Approx(RY[1][2]));

		CHECK(R[2][0] == doctest::Approx(RY[2][0]));
		CHECK(R[2][1] == doctest::Approx(RY[2][1]));
		CHECK(R[2][2] == doctest::Approx(RY[2][2]));

		rex::math::M3 RZ = rex::math::mat3_rotation_z((float)rex::math::PI_DIV_2);
		R = rex::math::mat3_rotation_axis({0.0f, 0.0f, 1.0f}, (float)rex::math::PI_DIV_2);
		CHECK(R[0][0] == doctest::Approx(RZ[0][0]));
		CHECK(R[0][1] == doctest::Approx(RZ[0][1]));
		CHECK(R[0][2] == doctest::Approx(RZ[0][2]));

		CHECK(R[1][0] == doctest::Approx(RZ[1][0]));
		CHECK(R[1][1] == doctest::Approx(RZ[1][1]));
		CHECK(R[1][2] == doctest::Approx(RZ[1][2]));

		CHECK(R[2][0] == doctest::Approx(RZ[2][0]));
		CHECK(R[2][1] == doctest::Approx(RZ[2][1]));
		CHECK(R[2][2] == doctest::Approx(RZ[2][2]));

		rex::math::V3 a = {0.0f ,1.0f, 2.0f};
		R = rex::math::mat3_rotation_axis(
				rex::math::normalize(rex::math::V3{1.0f, 1.0f, 1.0f}),
				(float)rex::math::PI / 4.0f
			);

		// online calculator: https://www.vcalc.com/wiki/vCalc/V3+-+Vector+Rotation
		rex::math::V3 b = a * R;
		CHECK(b.x == doctest::Approx(0.70114f));
		CHECK(b.y == doctest::Approx(0.1835f));
		CHECK(b.z == doctest::Approx(2.11536f));

		rex::math::V3 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("scaling")
	{
		rex::math::V3 s = {10.0f, 20.0f, 30.0f};

		rex::math::M3 S = rex::math::mat3_scaling(s.x, s.y, s.z);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);
		CHECK(b.z == a.z * s.z);

		S = rex::math::mat3_scaling(s);

		b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);
		CHECK(b.z == a.z * s.z);

		rex::math::V3 c = b * rex::math::inverse(S);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing xy")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_xy(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x + a.y*s);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing xz")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_xz(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x + a.z*s);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing yx")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_yx(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y + a.x*s);
		CHECK(b.z == a.z);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing yz")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_yz(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y + a.z*s);
		CHECK(b.z == a.z);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing zx")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_zx(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z + a.x*s);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}

	SUBCASE("shearing zy")
	{
		float s = 10.0f;

		rex::math::M3 H = rex::math::mat3_shearing_zy(s);
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);
		CHECK(b.z == a.z + a.y*s);

		rex::math::V3 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
		CHECK(c.z == doctest::Approx(a.z));
	}
}
