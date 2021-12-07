#include <rex-core/str.h>
#include <rex-core/defer.h>

#include "doctest.h"

TEST_CASE("[rex-core]: str")
{
	using namespace rc;

	SUBCASE("str initialization")
	{
		auto s = str_init();
		rex_defer(str_deinit(s));

		CHECK(s.ptr == nullptr);
		CHECK(s.count == 0);
		CHECK(s.capacity == 0);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str initialization with allocator")
	{
		auto s = str_init(frame_allocator());
		rex_defer(str_deinit(s));

		CHECK(s.ptr == nullptr);
		CHECK(s.count == 0);
		CHECK(s.capacity == 0);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str length")
	{
		CHECK(str_len("") == 0);
		CHECK(str_len("H") == 1);
		CHECK(str_len("Hello, World!") == 13);
	}

	SUBCASE("str from range")
	{
		const char* c_str = "Hello, World!";

		auto s = str_from(c_str + 7, c_str + 12);
		rex_defer(str_deinit(s));

		CHECK(s == "World");
		CHECK(s[s.count] == '\0');
		CHECK(s.ptr != c_str);
		CHECK(s.count == 5);
		CHECK(s.capacity == s.count + 1);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str from range with allocator")
	{
		const char* c_str = "Hello, World!";

		auto s = str_from(c_str + 7, c_str + 12, frame_allocator());
		rex_defer(str_deinit(s));

		CHECK(s == "World");
		CHECK(s[s.count] == '\0');
		CHECK(s.ptr != c_str);
		CHECK(s.count == 5);
		CHECK(s.capacity == s.count + 1);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str from c string")
	{
		const char* c_str = "Hello, World!";

		auto s = str_from(c_str);
		rex_defer(str_deinit(s));

		CHECK(s == c_str);
		CHECK(s[s.count] == '\0');
		CHECK(s.ptr != c_str);
		CHECK(s.count == str_len(c_str));
		CHECK(s.capacity == s.count + 1);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str from c string with allocator")
	{
		const char* c_str = "Hello, World!";

		auto s = str_from(c_str, frame_allocator());
		rex_defer(str_deinit(s));

		CHECK(s == c_str);
		CHECK(s[s.count] == '\0');
		CHECK(s.ptr != c_str);
		CHECK(s.count == str_len(c_str));
		CHECK(s.capacity == s.count + 1);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str literal")
	{
		const char* c_str = "Hello, World!";

		auto s = str_lit(c_str);
		rex_defer(str_deinit(s));

		CHECK(s == c_str);
		CHECK(s[s.count] == '\0');
		CHECK(s.ptr == c_str);
		CHECK(s.count == str_len(c_str));
		CHECK(s.capacity == s.count + 1);
		CHECK(s.allocator == nullptr);
	}

	SUBCASE("str copy")
	{
		const char* c_str = "Hello, World!";

		auto s1 = str_from("Hello, World!");
		rex_defer(str_deinit(s1));

		auto s2 = str_copy(s1);
		rex_defer(str_deinit(s2));

		CHECK(s2 == s1);
		CHECK(s2[s2.count] == '\0');
		CHECK(s2.ptr != s1.ptr);
		CHECK(s2.count == s1.count);
		CHECK(s2.capacity == s2.count + 1);
		CHECK(s2.allocator != nullptr);
	}

	SUBCASE("str copy with allocator")
	{
		const char* c_str = "Hello, World!";

		auto s1 = str_from("Hello, World!");
		rex_defer(str_deinit(s1));

		auto s2 = str_copy(s1, frame_allocator());
		rex_defer(str_deinit(s2));

		CHECK(s2 == s1);
		CHECK(s2[s2.count] == '\0');
		CHECK(s2.ptr != s1.ptr);
		CHECK(s2.count == s1.count);
		CHECK(s2.capacity == s2.count + 1);
		CHECK(s2.allocator != nullptr);
	}

	SUBCASE("str clone/destroy")
	{
		const char* c_str = "Hello, World!";

		auto s1 = str_from("Hello, World!");
		rex_defer(destroy(s1));

		auto s2 = clone(s1);
		rex_defer(destroy(s2));

		CHECK(s2 == s1);
		CHECK(s2[s2.count] == '\0');
		CHECK(s2.ptr != s1.ptr);
		CHECK(s2.count == s1.count);
		CHECK(s2.capacity == s2.count + 1);
		CHECK(s2.allocator != nullptr);
	}

	SUBCASE("str clone/destroy with allocator")
	{
		const char* c_str = "Hello, World!";

		auto s1 = str_from("Hello, World!");
		rex_defer(destroy(s1));

		auto s2 = clone(s1, frame_allocator());
		rex_defer(destroy(s2));

		CHECK(s2 == s1);
		CHECK(s2[s2.count] == '\0');
		CHECK(s2.ptr != s1.ptr);
		CHECK(s2.count == s1.count);
		CHECK(s2.capacity == s2.count + 1);
		CHECK(s2.allocator != nullptr);
	}

	SUBCASE("str format")
	{
		auto s = str_fmt("Hello, %s! %d", "World", 2021);
		rex_defer(str_deinit(s));

		CHECK(s == "Hello, World! 2021");
		CHECK(s[s.count] == '\0');
		CHECK(s.count == 18);
		CHECK(s.capacity == 19);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str format with allocator")
	{
		auto s = str_fmt(frame_allocator(), "Hello, %s! %d", "World", 2021);
		rex_defer(str_deinit(s));

		CHECK(s == "Hello, World! 2021");
		CHECK(s[s.count] == '\0');
		CHECK(s.count == 18);
		CHECK(s.capacity == 19);
		CHECK(s.allocator != nullptr);
	}

	SUBCASE("str clear")
	{
		auto s = str_from("Hello, World!");
		rex_defer(str_deinit(s));

		str_clear(s);
		CHECK(s == "");
		CHECK(s[s.count] == '\0');
		CHECK(s.count == 0);
		CHECK(s.capacity == 14);
	}

	SUBCASE("str append")
	{
		auto s = str_init();
		rex_defer(str_deinit(s));

		str_append(s, str_lit("Hello, "));
		str_append(s, str_lit("World!"));

		CHECK(s == "Hello, World!");
		CHECK(s[s.count] == '\0');
		CHECK(s.count == 13);
		CHECK(s.capacity > s.count);
	}

	SUBCASE("str append formated")
	{
		auto s = str_init();
		rex_defer(str_deinit(s));

		str_append(s, "Hello");
		str_append(s, ", %s! %d", "World", 2021);

		CHECK(s == "Hello, World! 2021");
		CHECK(s[s.count] == '\0');
		CHECK(s.count == 18);
		CHECK(s.capacity > s.count);
	}

	SUBCASE("str equal")
	{
		auto s1 = str_from("Hello");
		auto s2 = str_from("Hello");
		auto s3 = str_from("World");

		rex_defer({
			rex_defer(str_deinit(s1));
			rex_defer(str_deinit(s2));
			rex_defer(str_deinit(s3));
		});

		CHECK(s1 == s2);
		CHECK(s1 != s3);
		CHECK(s1 == "Hello");
		CHECK(s1 != "World");
		CHECK("Hello" == s1);
		CHECK("World" != s1);
	}
}
