#include <rex-math/vec2.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Vec2")
{
	SUBCASE("init")
	{
		rex::math::V2 v = {};
		CHECK(v.x == 0.0f);
		CHECK(v.y == 0.0f);

		v = {1.0f, 2.0f};
		CHECK(v.x == 1.0f);
		CHECK(v.y == 2.0f);
	}

	SUBCASE("equal")
	{
		rex::math::V2 a = { 1.0f,  2.0f};
		rex::math::V2 b = { 1.0f,  2.0f};
		rex::math::V2 c = {11.0f, 12.0f};

		CHECK(a == b);
		CHECK(a != c);
	}

	SUBCASE("add")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		rex::math::V2 c = a + b;
		CHECK(c.x == a.x + b.x);
		CHECK(c.y == a.y + b.y);

		a += b;
		CHECK(a == c);
	}

	SUBCASE("unary minus")
	{
		rex::math::V2 a = {1.0f, 2.0f};

		rex::math::V2 b = -a;
		CHECK(b.x == -a.x);
		CHECK(b.y == -a.y);
	}

	SUBCASE("subtract")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		rex::math::V2 c = a - b;
		CHECK(c.x == a.x - b.x);
		CHECK(c.y == a.y - b.y);

		a -= b;
		CHECK(a == c);
	}

	SUBCASE("multiply")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		float f = 5.0f;

		rex::math::V2 b = a * f;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);

		b = f * a;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);

		a *= f;
		CHECK(b == b);
	}

	SUBCASE("divide")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		float f = 5.0f;

		rex::math::V2 b = a / f;
		CHECK(b.x == a.x / f);
		CHECK(b.y == a.y / f);

		a /= f;
		CHECK(a == b);
	}

	SUBCASE("dot")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		CHECK(rex::math::dot(a, a) == 5.0f);
		CHECK(rex::math::dot(b, b) == 265.0f);
		CHECK(rex::math::dot(a, b) == 35.0f);

		rex::math::V2 a_normalized = rex::math::normalize(a);
		rex::math::V2 b_normalized = rex::math::normalize(b);

		CHECK(rex::math::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(a_normalized, b_normalized) == doctest::Approx(0.962f).epsilon(0.001f));
	}

	SUBCASE("norm")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		float a_norm = rex::math::norm(a);
		float b_norm = rex::math::norm(b);

		CHECK(a_norm == doctest::Approx(2.236f).epsilon(0.001f));
		CHECK(b_norm == doctest::Approx(16.279f).epsilon(0.001f));
	}

	SUBCASE("length")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		float a_length = rex::math::length(a);
		float b_length = rex::math::length(b);

		CHECK(a_length == doctest::Approx(2.236f).epsilon(0.001f));
		CHECK(b_length == doctest::Approx(16.279f).epsilon(0.001f));
	}

	SUBCASE("normalize")
	{
		rex::math::V2 a = {1.0f, 2.0f};
		rex::math::V2 b = {11.0f, 12.0f};

		rex::math::V2 a_normalized = rex::math::normalize(a);
		rex::math::V2 b_normalized = rex::math::normalize(b);

		float a_length = rex::math::length(a_normalized);
		float b_length = rex::math::length(b_normalized);

		CHECK(a_length == doctest::Approx(1.0f));
		CHECK(b_length == doctest::Approx(1.0f));
	}

	SUBCASE("cross")
	{
		rex::math::V2 x = {1.0f, 0.0f};
		rex::math::V2 y = {0.0f, 1.0f};

		float xy = rex::math::cross(x, y);
		CHECK(xy == doctest::Approx(1.0f));

		float yx = rex::math::cross(y, x);
		CHECK(yx == doctest::Approx(-1.0f));
	}
}
