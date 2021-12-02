#pragma once

// source: https://www.gingerbill.org/article/2015/08/19/defer-in-cpp/

namespace rex::core
{
	template <typename F>
	struct Defer {
		F f;
		Defer(F f) : f(f) {}
		~Defer() { f(); }
	};

	template <typename F>
	Defer<F> make_defer(F f) {
		return Defer<F>(f);
	}
}

#define rex_DEFER_1(x, y) x##y
#define rex_DEFER_2(x, y) rex_DEFER_1(x, y)
#define rex_DEFER_3(x)    rex_DEFER_2(x, __COUNTER__)

#define rex_defer(code)   auto rex_DEFER_3(_defer_) = rex::core::make_defer([&](){ code; })
