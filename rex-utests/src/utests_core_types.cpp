#include <rex-core/types.h>

#include "doctest.h"
#include <climits>
#include <cfloat>

TEST_CASE("[rex-core]: types")
{
	using namespace rc;

	SUBCASE("limits")
	{
		CHECK(U8_MIN  == 0);
		CHECK(U16_MIN == 0);
		CHECK(U32_MIN == 0);
		CHECK(U64_MIN == 0);

		CHECK(I8_MIN  == SCHAR_MIN);
		CHECK(I16_MIN == SHRT_MIN);
		CHECK(I32_MIN == INT_MIN);
		CHECK(I64_MIN == LLONG_MIN);

		CHECK(F32_MIN == FLT_MIN);
		CHECK(F64_MIN == DBL_MIN);

		CHECK(U8_MAX  == UCHAR_MAX);
		CHECK(U16_MAX == USHRT_MAX);
		CHECK(U32_MAX == UINT_MAX);
		CHECK(U64_MAX == ULLONG_MAX);

		CHECK(I8_MAX  == SCHAR_MAX);
		CHECK(I16_MAX == SHRT_MAX);
		CHECK(I32_MAX == INT_MAX);
		CHECK(I64_MAX == LLONG_MAX);

		CHECK(F32_MAX == FLT_MAX);
		CHECK(F64_MAX == DBL_MAX);
	}

	SUBCASE("V2<i32>")
	{
		V2<i32> v = {};
		CHECK((v.x == 0 && v.y == 0));

		v = {{1, 2}};
		CHECK((v.x == v.u && v.u == v.r && v.r == v.width && v.width == 1));
		CHECK((v.y == v.v && v.v == v.g && v.g == v.height && v.height == 2));

		const V2<i32> v1 = {{3, 4}};
		v[0] = 5;
		v[1] = v1[1];
		CHECK((v[0] == 5 && v[1] == 4));
	}

	SUBCASE("V3<f32>")
	{
		V3<f32> v = {};
		CHECK((v.x == 0 && v.y == 0 && v.z == 0));

		v = {{1, 2, 3}};
		CHECK((v.x == v.u && v.u == v.r && v.r == v.width && v.width == 1));
		CHECK((v.y == v.v && v.v == v.g && v.g == v.height && v.height == 2));
		CHECK((v.z == v.w && v.w == v.b && v.b == v.depth && v.depth == 3));

		V2<f32> xy = v.xy;
		V2<f32> uv = v.uv;
		V2<f32> rg = v.rg;
		CHECK((xy.x == uv.u && uv.u == rg.r && rg.r == 1));
		CHECK((xy.y == uv.v && uv.v == rg.g && rg.g == 2));

		const V3<f32> v1 = {{4, 5, 6}};
		v[0] = 7;
		v[1] = v1[1];
		v[2] = v1[2];
		CHECK((v[0] == 7 && v[1] == 5 && v[2] == 6));
	}

	SUBCASE("V4<f64>")
	{
		V4<f64> v = {};
		CHECK((v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0));

		v = {{1, 2, 3, 4}};
		CHECK((v.x == v.r && v.r == 1));
		CHECK((v.y == v.g && v.g == 2));
		CHECK((v.z == v.b && v.b == 3));
		CHECK((v.w == v.a && v.a == 4));

		V3<f64> xyz = v.xyz;
		V3<f64> rgb = v.rgb;
		CHECK((xyz.x == rgb.r && rgb.r == 1));
		CHECK((xyz.y == rgb.g && rgb.g == 2));
		CHECK((xyz.z == rgb.b && rgb.b == 3));

		const V4<f64> v1 = {{5, 6, 7, 8}};
		v[0] = 9;
		v[1] = v1[1];
		v[2] = v1[2];
		v[3] = v1[3];
		CHECK((v[0] == 9 && v[1] == 6 && v[2] == 7 && v[3] == 8));
	}
}
