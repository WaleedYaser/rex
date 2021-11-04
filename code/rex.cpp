#include "rex.h"

static constexpr Pixel color_palette[] = {
	{0.19f, 0.30f, 0.39f, 1.0f}, // dark blue
	{0.70f, 0.91f, 0.91f, 1.0f}, // light blue
	{0.56f, 0.73f, 0.67f, 1.0f}, // green
	{0.95f, 0.82f, 0.59f, 1.0f}, // yellow
	{0.93f, 0.54f, 0.46f, 1.0f}, // orange
};

inline static void
init(Rex* self)
{
	*self = Rex{};
}

inline static void
destroy(Rex* self)
{
	self->free(self->canvas.pixels);
}

inline static void
reload(Rex* self)
{

}

inline static void
loop(Rex* self)
{
	auto& canvas = self->canvas;

	if (canvas.width != self->window_width || canvas.height != self->window_height)
	{
		self->free(canvas.pixels);
		canvas.pixels = (Pixel*)self->alloc(self->window_width * self->window_height * sizeof(Pixel));
		canvas.width = self->window_width;
		canvas.height = self->window_height;
	}

	// clear
	for (int y = 0; y < canvas.height; ++y)
	{
		for (int x = 0; x < canvas.width; ++x)
		{
			canvas.pixels[y * canvas.width + x] = color_palette[0];
		}
	}

	// draw animated circle in the middle
	static bool reverse = false;
	static float t = 0;
	int mid_x = canvas.width / 2;
	int mid_y = canvas.height / 2;
	float radius_squared = 100 * 100 + t*t * 100 * 100;
	for (int y = 0; y < canvas.height; ++y)
	{
		for (int x = 0; x < canvas.width; ++x)
		{
			int d = (x - mid_x) * (x - mid_x) + (y - mid_y) * (y - mid_y);
			if (d <= radius_squared)
				canvas.pixels[y * canvas.width + x] = color_palette[4];
		}
	}

	// reverse animation
	if (t >= 1)
		reverse = true;
	else if (t <= 0)
		reverse = false;

	// update t
	t = reverse ? t - self->dt : t + self->dt;
}

Rex_Api*
rex_api()
{
	static Rex_Api api;
	api.init = init;
	api.destory = destroy;
	api.reload = reload;
	api.loop = loop;
	return &api;
}