#include <rex-math/mat2.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Mat2")
{
	SUBCASE("init")
	{
		rex::math::M2 M = {};

		CHECK(M[0][0] == 0.0f); CHECK(M[0][1] == 0.0f);
		CHECK(M[1][0] == 0.0f); CHECK(M[1][1] == 0.0f);

		M = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		CHECK(M[0][0] == 1.0f); CHECK(M[0][1] == 2.0f);
		CHECK(M[1][0] == 3.0f); CHECK(M[1][1] == 4.0f);
	}

	SUBCASE("equal")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 C = {
			11.0f, 12.0f,
			13.0f, 14.0f
		};

		CHECK(A == B);
		CHECK(A != C);
	}

	SUBCASE("add")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = {
			11.0f, 12.0f,
			13.0f, 14.0f
		};

		rex::math::M2 C = A + B;
		CHECK(C[0][0] == A[0][0] + B[0][0]); CHECK(C[0][1] == A[0][1] + B[0][1]);
		CHECK(C[1][0] == A[1][0] + B[1][0]); CHECK(C[1][1] == A[1][1] + B[1][1]);

		A += B;
		CHECK(A == C);
	}

	SUBCASE("unary minus")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = -A;
		CHECK(B[0][0] == -A[0][0]); CHECK(B[0][1] == -A[0][1]);
		CHECK(B[1][0] == -A[1][0]); CHECK(B[1][1] == -A[1][1]);
	}

	SUBCASE("subtract")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = {
			11.0f, 12.0f,
			13.0f, 14.0f
		};

		rex::math::M2 C = A - B;
		CHECK(C[0][0] == A[0][0] - B[0][0]); CHECK(C[0][1] == A[0][1] - B[0][1]);
		CHECK(C[1][0] == A[1][0] - B[1][0]); CHECK(C[1][1] == A[1][1] - B[1][1]);

		A -= B;
		CHECK(A == C);
	}

	SUBCASE("multiply scalar")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		float f = 5.0f;

		rex::math::M2 B = A * f;
		CHECK(B[0][0] == f * A[0][0]); CHECK(B[0][1] == f * A[0][1]);
		CHECK(B[1][0] == f * A[1][0]); CHECK(B[1][1] == f * A[1][1]);

		B = f * A;
		CHECK(B[0][0] == f * A[0][0]); CHECK(B[0][1] == f * A[0][1]);
		CHECK(B[1][0] == f * A[1][0]); CHECK(B[1][1] == f * A[1][1]);

		A *= f;
		CHECK(A == B);
	}

	SUBCASE("multiply vector")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::M2 M = {
			11.0f, 12.0f,
			13.0f, 14.0f
		};

		rex::math::V2 b = a * M;
		CHECK(b.x == 37.0f);
		CHECK(b.y == 40.0f);
	}

	SUBCASE("multiply matrix")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = {
			11.0f, 12.0f,
			13.0f, 14.0f
		};

		rex::math::M2 C = A * B;
		CHECK(C[0][0] == 37.0f); CHECK(C[0][1] == 40.0f);
		CHECK(C[1][0] == 85.0f); CHECK(C[1][1] == 92.0f);
	}

	SUBCASE("divide")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		float f = 5.0f;

		rex::math::M2 C = A / f;
		CHECK(C[0][0] == A[0][0] / f); CHECK(C[0][1] == A[0][1] / f);
		CHECK(C[1][0] == A[1][0] / f); CHECK(C[1][1] == A[1][1] / f);

		A /= f;
		CHECK(A == C);
	}

	SUBCASE("identity")
	{
		rex::math::M2 I = rex::math::mat2_identity<float>();
		CHECK(I[0][0] == 1.0f); CHECK(I[0][1] == 0.0f);
		CHECK(I[1][0] == 0.0f); CHECK(I[1][1] == 1.0f);

		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = a * I;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);

		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = I * A;
		CHECK(B[0][0] == A[0][0]); CHECK(B[0][1] == A[0][1]);
		CHECK(B[1][0] == A[1][0]); CHECK(B[1][1] == A[1][1]);

		B = A * I;
		CHECK(B == A);
	}

	SUBCASE("transpose")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = rex::math::transpose(A);
		CHECK(B[0][0] == A[0][0]); CHECK(B[0][1] == A[1][0]);
		CHECK(B[1][0] == A[0][1]); CHECK(B[1][1] == A[1][1]);
	}

	SUBCASE("trace")
	{
		rex::math::M2 M = {
			0.0f, 2.0f,
			3.0f, 4.0f
		};

		CHECK(rex::math::trace(M) == 4.0f);
	}

	SUBCASE("determinant")
	{
		rex::math::M2 I = rex::math::mat2_identity<float>();
		CHECK(rex::math::det(I) == 1.0f);

		rex::math::M2 M = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		CHECK(rex::math::det(M) == -2.0f);
	}

	SUBCASE("invertible")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		CHECK(rex::math::invertible(A) == true);

		rex::math::M2 B = {
			1.0f, 1.0f,
			1.0f, 1.0f
		};

		CHECK(rex::math::invertible(B) == false);
	}

	SUBCASE("inverse")
	{
		rex::math::M2 A = {
			1.0f, 2.0f,
			3.0f, 4.0f
		};

		rex::math::M2 B = rex::math::inverse(A);

		rex::math::M2 C = A * B;
		CHECK(C[0][0] == 1.0f); CHECK(C[0][1] == 0.0f);
		CHECK(C[1][0] == 0.0f); CHECK(C[1][1] == 1.0f);

		C = B * A;
		CHECK(C[0][0] == 1.0f); CHECK(C[0][1] == 0.0f);
		CHECK(C[1][0] == 0.0f); CHECK(C[1][1] == 1.0f);

		rex::math::V2 a = {11.0f, 12.0f};
		rex::math::V2 b = a * A * B;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y);
	}

	SUBCASE("rotation")
	{
		rex::math::M2 R = rex::math::mat2_rotation((float)rex::math::PI_DIV_2);
		rex::math::V2 a = {1.0f, 1.0f};

		rex::math::V2 b = a * R;
		CHECK(b.x == doctest::Approx(-a.x));
		CHECK(b.y == doctest::Approx(a.y));

		rex::math::V2 c = b * rex::math::inverse(R);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
	}

	SUBCASE("scaling")
	{
		rex::math::V2 s = {10.0f, 20.0f};

		rex::math::M2 S = rex::math::mat2_scaling(s.x, s.y);
		rex::math::V2 a = {1.0f, 2.0f};

		rex::math::V2 b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);

		S = rex::math::mat2_scaling(s);

		b = a * S;
		CHECK(b.x == a.x * s.x);
		CHECK(b.y == a.y * s.y);

		rex::math::V2 c = b * rex::math::inverse(S);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
	}

	SUBCASE("shearing x")
	{
		float s = 10.0f;

		rex::math::M2 H = rex::math::mat2_shearing_x(s);
		rex::math::V2 a = {1.0f, 2.0f};

		rex::math::V2 b = a * H;
		CHECK(b.x == a.x + a.y*s);
		CHECK(b.y == a.y);

		rex::math::V2 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
	}

	SUBCASE("shearing y")
	{
		float s = 10.0f;

		rex::math::M2 H = rex::math::mat2_shearing_y(s);
		rex::math::V2 a = {1.0f, 2.0f};

		rex::math::V2 b = a * H;
		CHECK(b.x == a.x);
		CHECK(b.y == a.y + a.x*s);

		rex::math::V2 c = b * rex::math::inverse(H);
		CHECK(c.x == doctest::Approx(a.x));
		CHECK(c.y == doctest::Approx(a.y));
	}
}
