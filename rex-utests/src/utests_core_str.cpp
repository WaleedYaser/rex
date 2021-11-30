#include <rex-core/str.h>

#include "doctest.h"

TEST_CASE("[rex-core]: str")
{
	using namespace rex::core;

	SUBCASE("str creation")
	{
		auto s = str_new();
		CHECK(s.count == 0);
		CHECK(s.capacity == 1);
		CHECK(*s.ptr == '\0');
		str_free(s);

		auto c_str = "hello, world!";
		s = str_from(c_str);
		CHECK(s.count == 13);
		CHECK(s.capacity == 14);
		for (i64 i = 0; i < s.count; ++i)
			CHECK(s[i] == c_str[i]);
		CHECK(s[s.count] == '\0');
		str_free(s);

		s = str_literal(c_str);
		CHECK(s.allocator == nullptr);
		CHECK(s.count == 13);
		CHECK(s.capacity == 14);
		for (i64 i = 0; i < s.count; ++i)
			CHECK(s[i] == c_str[i]);
		CHECK(s[s.count] == '\0');
		str_free(s);

		auto c_str_empty = "";
		s = str_from(c_str_empty);
		CHECK(s.count == 0);
		CHECK(s.capacity == 1);
		CHECK(s[s.count] == '\0');
		str_free(s);

		s = str_literal(c_str_empty);
		CHECK(s.allocator == nullptr);
		CHECK(s.count == 0);
		CHECK(s.capacity == 1);
		CHECK(s[s.count] == '\0');
		str_free(s);
	}

	SUBCASE("str append")
	{
		auto s1 = "Hi, ";
		auto s2 = "There. ";
		auto s3 = str_from("Hamada");

		auto s = str_new();
		str_append(s, s1);
		str_append(s, s2);
		str_append(s, s3);

		CHECK(s.count == 17);
		CHECK(s.capacity == 18);

		auto expected = "Hi, There. Hamada";
		for (i32 i = 0; i < s.count; ++i)
			CHECK(s[i] == expected[i]);
		CHECK(s[s.count] == '\0');

		str_free(s);
		str_free(s3);
	}

	SUBCASE("str equal")
	{
		auto s1 = str_from("hello");
		auto s2 = str_from("world");
		auto s3 = str_from("hello");
		auto s4 = str_from("");

		CHECK(s1 != s2);
		CHECK(s1 == s3);
		CHECK(s1 == "hello");
		CHECK(s1 != "world");
		CHECK("hello" == s1);
		CHECK("world" != s1);
		CHECK(s4 == "");
		CHECK(s4 != "hello");
	}
}
