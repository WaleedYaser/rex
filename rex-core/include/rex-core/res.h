#pragma once

// https://github.com/oktal/result/blob/master/result.h

#include "rex-core/str.h"

namespace rex::core
{
	struct Err
	{
		Str msg;

		void destroy();

		explicit operator bool() { return msg.count != 0; }
		bool operator==(bool b)  { return (msg.count != 0) == b; }
		bool operator!=(bool b)  { return (msg.count != 0) != b; }
	};

	void Err::destroy() { msg.destroy(); }

	template <typename T>
	struct Res
	{
		T val;
		Err err;

		static Res<T> error(Str msg);
		static Res<T> ok(T&& val);

		void destroy();

		explicit operator bool() { return err == false; }
		bool operator==(bool b)  { return err != b; }
		bool operator!=(bool b)  { return err == b; }
	};

	template <typename T>
	void Res<T>::destroy() { err.destroy(); }

	template <typename T>
	Res<T> Res<T>::error(Str msg)
	{
		Res<T> self = {};
		self.err.msg = msg;
		return self;
	}

	template <typename T>
	Res<T> Res<T>::error(Str msg)
	{
		Res<T> self = {};
		self.err.msg = msg;
		return self;
	}
}
