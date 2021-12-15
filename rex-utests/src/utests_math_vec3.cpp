#include <rex-math/vec3.h>

#include "doctest.h"

TEST_CASE("[rex-math]: Vec3")
{
	SUBCASE("init")
	{
		rex::math::V3 v = {};
		CHECK(v.x == 0);
		CHECK(v.y == 0);
		CHECK(v.z == 0);

		v = {1.0f, 2.0f, 3.0f};
		CHECK(v.x == 1.0f);
		CHECK(v.y == 2.0f);
		CHECK(v.z == 3.0f);
	}

	SUBCASE("equal")
	{
		rex::math::V3 a = { 1.0f,  2.0f,  3.0f};
		rex::math::V3 b = { 1.0f,  2.0f,  3.0f};
		rex::math::V3 c = {11.0f, 12.0f, 13.0f};

		CHECK(a == b);
		CHECK(a != c);
	}

	SUBCASE("add")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		rex::math::V3 c = a + b;
		CHECK(c.x == a.x + b.x);
		CHECK(c.y == a.y + b.y);
		CHECK(c.z == a.z + b.z);

		a += b;
		CHECK(a == c);
	}

	SUBCASE("unary minus")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};

		rex::math::V3 b = -a;
		CHECK(b.x == -a.x);
		CHECK(b.y == -a.y);
		CHECK(b.z == -a.z);
	}

	SUBCASE("subtract")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		rex::math::V3 c = a - b;
		CHECK(c.x == a.x - b.x);
		CHECK(c.y == a.y - b.y);
		CHECK(c.z == a.z - b.z);

		a -= b;
		CHECK(a == c);
	}

	SUBCASE("multiply")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		float f = 5.0f;

		rex::math::V3 b = a * f;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);
		CHECK(b.z == a.z * f);

		b = f * a;
		CHECK(b.x == a.x * f);
		CHECK(b.y == a.y * f);
		CHECK(b.z == a.z * f);

		a *= f;
		CHECK(a == b);
	}

	SUBCASE("divide")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		float f = 5.0f;

		rex::math::V3 b = a / f;
		CHECK(b.x == a.x / f);
		CHECK(b.y == a.y / f);
		CHECK(b.z == a.z / f);

		a /= f;
		CHECK(a == b);
	}

	SUBCASE("dot")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		CHECK(rex::math::dot(a, a) == 14.0f);
		CHECK(rex::math::dot(b, b) == 434.0f);
		CHECK(rex::math::dot(a, b) == 74.0f);

		rex::math::V3 a_normalized = rex::math::normalize(a);
		rex::math::V3 b_normalized = rex::math::normalize(b);

		CHECK(rex::math::dot(a_normalized, a_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(b_normalized, b_normalized) == doctest::Approx(1.0f));
		CHECK(rex::math::dot(a_normalized, b_normalized) == doctest::Approx(0.949f).epsilon(0.001f));
	}

	SUBCASE("norm")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		float a_norm = rex::math::norm(a);
		float b_norm = rex::math::norm(b);

		CHECK(a_norm == doctest::Approx(3.742f).epsilon(0.001f));
		CHECK(b_norm == doctest::Approx(20.833f).epsilon(0.001f));
	}

	SUBCASE("length")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		float a_length = rex::math::length(a);
		float b_length = rex::math::length(b);

		CHECK(a_length == doctest::Approx(3.742f).epsilon(0.001f));
		CHECK(b_length == doctest::Approx(20.833f).epsilon(0.001f));
	}

	SUBCASE("normalize")
	{
		rex::math::V3 a = {1.0f, 2.0f, 3.0f};
		rex::math::V3 b = {11.0f, 12.0f, 13.0f};

		rex::math::V3 a_normalized = rex::math::normalize(a);
		rex::math::V3 b_normalized = rex::math::normalize(b);

		float a_length = rex::math::length(a_normalized);
		float b_length = rex::math::length(b_normalized);

		CHECK(a_length == doctest::Approx(1.0f));
		CHECK(b_length == doctest::Approx(1.0f));
	}

	SUBCASE("cross")
	{
		rex::math::V3 x = {1.0f, 0.0f, 0.0f};
		rex::math::V3 y = {0.0f, 1.0f, 0.0f};
		rex::math::V3 z = {0.0f, 0.0f, 1.0f};

		rex::math::V3 xy = rex::math::cross(x, y);
		CHECK(xy.x == doctest::Approx(z.x));
		CHECK(xy.y == doctest::Approx(z.y));
		CHECK(xy.z == doctest::Approx(z.z));

		rex::math::V3 yx = rex::math::cross(y, x);
		CHECK(yx.x == doctest::Approx(-z.x));
		CHECK(yx.y == doctest::Approx(-z.y));
		CHECK(yx.z == doctest::Approx(-z.z));

		rex::math::V3 yz = rex::math::cross(y, z);
		CHECK(yz.x == doctest::Approx(x.x));
		CHECK(yz.y == doctest::Approx(x.y));
		CHECK(yz.z == doctest::Approx(x.z));

		rex::math::V3 zy = rex::math::cross(z, y);
		CHECK(zy.x == doctest::Approx(-x.x));
		CHECK(zy.y == doctest::Approx(-x.y));
		CHECK(zy.z == doctest::Approx(-x.z));

		rex::math::V3 zx = rex::math::cross(z, x);
		CHECK(zx.x == doctest::Approx(y.x));
		CHECK(zx.y == doctest::Approx(y.y));
		CHECK(zx.z == doctest::Approx(y.z));

		rex::math::V3 xz = rex::math::cross(x, z);
		CHECK(xz.x == doctest::Approx(-y.x));
		CHECK(xz.y == doctest::Approx(-y.y));
		CHECK(xz.z == doctest::Approx(-y.z));
	}
}
