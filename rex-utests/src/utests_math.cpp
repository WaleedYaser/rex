#include <rex-math/math.h>

#include "doctest.h"
#include <math.h>

TEST_CASE("[rex-math]: consts")
{
	CHECK(doctest::Approx(rex::math::PI) == 3.14159265358979323846);
	CHECK(doctest::Approx(rex::math::PI_DIV_2) == (3.14159265358979323846 / 2.0));
	CHECK(doctest::Approx(rex::math::TAU) == (3.14159265358979323846 * 2.0));

	CHECK(doctest::Approx(rex::math::TO_DEGREE * rex::math::PI) == 180.0);
	CHECK(doctest::Approx(rex::math::TO_RADIAN * 360.0) == rex::math::TAU);
}

TEST_CASE("[rex-math]: math")
{
	SUBCASE("sqrt")
	{
		for (double i = 0.0; i < 100.0; i += 0.1)
		{
			CHECK(doctest::Approx(rex::math::sqrt(i*i)) == i);
			CHECK(doctest::Approx(rex::math::sqrt(float(i*i))) == float(i));
		}
	}

	SUBCASE("sin")
	{
		for (double theta = -2.0 * rex::math::TAU; theta < 2 * rex::math::TAU; theta += 0.1)
		{
			INFO("theta: ", theta);
			CHECK(doctest::Approx(rex::math::sin(theta)) == ::sin(theta));
			CHECK(doctest::Approx(rex::math::sin(float(theta))) == ::sinf(float(theta)));
		}
	}

	SUBCASE("cos")
	{
		for (double theta = -2.0 * rex::math::TAU; theta < 2 * rex::math::TAU; theta += 0.1)
		{
			INFO("theta: ", theta);
			CHECK(doctest::Approx(rex::math::cos(theta)) == ::cos(theta));
			CHECK(doctest::Approx(rex::math::cos(float(theta))) == ::cosf(float(theta)));
		}
	}

	SUBCASE("tan")
	{
		for (double theta = -2.0 * rex::math::TAU; theta < 2 * rex::math::TAU; theta += 0.1)
		{
			INFO("theta: ", theta);
			CHECK(doctest::Approx(rex::math::tan(theta)) == ::tan(theta));
			CHECK(doctest::Approx(rex::math::tan(float(theta))) == ::tanf(float(theta)));
		}
	}
}
