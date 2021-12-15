#include <rex-math/types.h>

#include "doctest.h"

TEST_CASE("[rex-math]: types")
{
	SUBCASE("Vec2<int>")
	{
		rex::math::Vec2<int> v = {};
		CHECK((v.x == 0 && v.y == 0));

		v = {{1, 2}};
		CHECK((v.x == v.u && v.u == v.r && v.r == v.width && v.width == 1));
		CHECK((v.y == v.v && v.v == v.g && v.g == v.height && v.height == 2));

		const rex::math::Vec2<int> v1 = {{3, 4}};
		v[0] = 5;
		v[1] = v1[1];
		CHECK((v[0] == 5 && v[1] == 4));
	}

	SUBCASE("Vec3<float>")
	{
		rex::math::Vec3<float> v = {};
		CHECK((v.x == 0 && v.y == 0 && v.z == 0));

		v = {{1, 2, 3}};
		CHECK((v.x == v.u && v.u == v.r && v.r == v.width && v.width == 1));
		CHECK((v.y == v.v && v.v == v.g && v.g == v.height && v.height == 2));
		CHECK((v.z == v.w && v.w == v.b && v.b == v.depth && v.depth == 3));

		rex::math::Vec2<float> xy = v.xy;
		rex::math::Vec2<float> uv = v.uv;
		rex::math::Vec2<float> rg = v.rg;
		CHECK((xy.x == uv.u && uv.u == rg.r && rg.r == 1));
		CHECK((xy.y == uv.v && uv.v == rg.g && rg.g == 2));

		const rex::math::Vec3<float> v1 = {{4, 5, 6}};
		v[0] = 7;
		v[1] = v1[1];
		v[2] = v1[2];
		CHECK((v[0] == 7 && v[1] == 5 && v[2] == 6));
	}

	SUBCASE("Vec4<double>")
	{
		rex::math::Vec4<double> v = {};
		CHECK((v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0));

		v = {{1, 2, 3, 4}};
		CHECK((v.x == v.r && v.r == 1));
		CHECK((v.y == v.g && v.g == 2));
		CHECK((v.z == v.b && v.b == 3));
		CHECK((v.w == v.a && v.a == 4));

		rex::math::Vec3<double> xyz = v.xyz;
		rex::math::Vec3<double> rgb = v.rgb;
		CHECK((xyz.x == rgb.r && rgb.r == 1));
		CHECK((xyz.y == rgb.g && rgb.g == 2));
		CHECK((xyz.z == rgb.b && rgb.b == 3));

		const rex::math::Vec4<double> v1 = {{5, 6, 7, 8}};
		v[0] = 9;
		v[1] = v1[1];
		v[2] = v1[2];
		v[3] = v1[3];
		CHECK((v[0] == 9 && v[1] == 6 && v[2] == 7 && v[3] == 8));
	}
}
