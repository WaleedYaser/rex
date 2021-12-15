#include <rex-math/vec4.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Vec4")
{
	SUBCASE("init")
	{
		rex::math::V4 v = {};
		CHECK(v.x == 0);
		CHECK(v.y == 0);
		CHECK(v.z == 0);
		CHECK(v.w == 0);

		v = {1.0f, 2.0f, 3.0f, 4.0f};
		CHECK(v.x == 1.0f);
		CHECK(v.y == 2.0f);
		CHECK(v.z == 3.0f);
		CHECK(v.w == 4.0f);
	}

	SUBCASE("equal")
	{
		rex::math::V4 a = { 1.0f,  2.0f,  3.0f,  4.0f};
		rex::math::V4 b = { 1.0f,  2.0f,  3.0f,  4.0f};
		rex::math::V4 c = {11.0f, 12.0f, 13.0f, 14.0f};

		CHECK(a == b);
		CHECK(a != c);
	}

	SUBCASE("add")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		rex::math::V4 c = a + b;
		CHECK(c.x == a.x + b.x);
		CHECK(c.y == a.y + b.y);
		CHECK(c.z == a.z + b.z);
		CHECK(c.w == a.w + b.w);

		a += b;
		CHECK(a == c);
	}

	SUBCASE("unary minus")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};

		rex::math::V4 b = -a;
		CHECK(b.x == -a.x);
		CHECK(b.y == -a.y);
		CHECK(b.z == -a.z);
		CHECK(b.w == -a.w);
	}

	SUBCASE("subtract")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		rex::math::V4 c = a - b;
		CHECK(c.x == a.x - b.x);
		CHECK(c.y == a.y - b.y);
		CHECK(c.z == a.z - b.z);
		CHECK(c.w == a.w - b.w);

		a -= b;
		CHECK(a == c);
	}

	SUBCASE("multiply")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		float f = 5.0f;

		rex::math::V4 b = a * f;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);
		CHECK(b.z == a.z * f);
		CHECK(b.w == a.w * f);

		b = f * a;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);
		CHECK(b.z == a.z * f);
		CHECK(b.w == a.w * f);

		a *= f;
		CHECK(a == b);
	}

	SUBCASE("divide")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		float f = 5.0f;

		rex::math::V4 b = a / f;
		CHECK(b.x == a.x / f);
		CHECK(b.y == a.y / f);
		CHECK(b.z == a.z / f);
		CHECK(b.w == a.w / f);

		a /= f;
		CHECK(a == b);
	}

	SUBCASE("dot")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		CHECK(rex::math::dot(a, a) == 30.0f);
		CHECK(rex::math::dot(b, b) == 630.0f);
		CHECK(rex::math::dot(a, b) == 130.0f);

		rex::math::V4 a_normalized = rex::math::normalize(a);
		rex::math::V4 b_normalized = rex::math::normalize(b);

		CHECK(rex::math::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(a_normalized, b_normalized) == doctest::Approx(0.946f).epsilon(0.001f));
	}

	SUBCASE("norm")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		float a_norm = rex::math::norm(a);
		float b_norm = rex::math::norm(b);

		CHECK(a_norm == doctest::Approx(5.477f).epsilon(0.001f));
		CHECK(b_norm == doctest::Approx(25.1f).epsilon(0.001f));
	}

	SUBCASE("length")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		float a_length = rex::math::length(a);
		float b_length = rex::math::length(b);

		CHECK(a_length == doctest::Approx(5.477f).epsilon(0.001f));
		CHECK(b_length == doctest::Approx(25.1f).epsilon(0.001f));
	}

	SUBCASE("normalize")
	{
		rex::math::V4 a = {1.0f, 2.0f, 3.0f, 4.0f};
		rex::math::V4 b = {11.0f, 12.0f, 13.0f, 14.0f};

		rex::math::V4 a_normalized = rex::math::normalize(a);
		rex::math::V4 b_normalized = rex::math::normalize(b);

		float a_length = rex::math::length(a_normalized);
		float b_length = rex::math::length(b_normalized);

		CHECK(a_length == doctest::Approx(1.0f));
		CHECK(b_length == doctest::Approx(1.0f));
	}
}
