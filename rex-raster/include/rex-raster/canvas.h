#pragma once

#include <rex-core/vec.h>
#include <rex-math/types.h>

namespace rex::raster
{
	struct Canvas
	{
		rc::Vec<math::Color_F32> color;
		rc::Vec<float> depth;
		int width, height;
	};

	inline static Canvas
	canvas_init()
	{
		Canvas self = {};
		self.color = rc::vec_init<math::Color_F32>();
		self.depth = rc::vec_init<float>();
		return self;
	}

	inline static void
	canvas_deinit(Canvas& self)
	{
		rc::vec_deinit(self.color);
		rc::vec_deinit(self.depth);
		self = {};
	}

	inline static void
	canvas_resize(Canvas& self, int width, int height)
	{
		if (self.width == width && self.height == height)
			return;

		if (width == 0 || height == 0)
			return;

		rc::vec_resize(self.color, width * height);
		rc::vec_resize(self.depth, width * height);

		self.width = width;
		self.height = height;
	}

	inline static void
	canvas_clear(Canvas& self, math::Color_F32 color, float depth)
	{
		rc::vec_fill(self.color, color);
		rc::vec_fill(self.depth, depth);
	}

	inline static math::Color_F32&
	canvas_color(Canvas& self, int x, int y)
	{
		return self.color[y * self.width + x];
	}

	inline static const math::Color_F32&
	canvas_color(const Canvas& self, int x, int y)
	{
		return self.color[y * self.width + x];
	}

	inline static float&
	canvas_depth(Canvas& self, int x, int y)
	{
		return self.depth[y * self.width + x];
	}

	inline static float
	canvas_depth(const Canvas& self, int x, int y)
	{
		return self.depth[y * self.width + x];
	}
}
