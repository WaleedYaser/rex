#if REX_OS_WASM

#include "rex-core/window.h"
#include "rex-core/assert.h"
#include "rex-core/memory.h"
#include "rex-core/str.h"

// TODO: use emscripten/html5.h instead
#include <emscripten.h>
#include <SDL/SDL.h>
#include <stdlib.h>

namespace rc
{
	EM_JS(int, get_canvas_width, (), { return canvas.width; });
	EM_JS(int, get_canvas_height, (), { return canvas.height; });

	Window *
	window_init(const char *title, i32 width, i32 height, void* user_data, resize_callback_t resize_callback)
	{
		auto self = rex_alloc_zeroed_T(Window);

		// override width and height
		width = get_canvas_width();
		height = get_canvas_height();

		SDL_Init(SDL_INIT_VIDEO);
		SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_RESIZABLE);

		self->title = title;
		self->width = width;
		self->height = height;
		self->native_handle = window;
		self->user_data = user_data;
		self->resize_callback = resize_callback;

		return self;
	}

	void
	window_deinit(Window* self)
	{

	}

	bool
	window_poll(Window* self)
	{
		// TODO: workaround to handle resizing
		int width = get_canvas_width();
		int height = get_canvas_height();

		if (self->width != width || self->height != height)
		{
			auto window = (SDL_Window*)self->native_handle;
			SDL_SetWindowSize(window, width, height);

			self->last_event.type = EVENT_TYPE_WINDOW_RESIZE;
			self->last_event.window_resize.width = width;
			self->last_event.window_resize.height = height;

			self->width = width;
			self->height = height;

			if (self->resize_callback)
				self->resize_callback(self, self->width, self->height);

			return true;
		}

		SDL_Event event = {};
		bool pending = SDL_PollEvent(&event);
		if (pending == false)
			return false;

		switch (event.type)
		{
			case SDL_WINDOWEVENT:
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					// auto width = event.window.data1;
					// auto height = event.window.data2;
				}
				break;
			}
			case SDL_QUIT:
			{
				self->last_event.type = EVENT_TYPE_WINDOW_CLOSE;
				break;
			}
			default:
			{
				// do nothing
				break;
			}
		}

		return true;
	}

	void
	window_title_set(Window* self, const char* title)
	{

	}

	void
	window_blit(Window* self, Color_U8 *pixels, i32 width, i32 height)
	{
		auto window = (SDL_Window*)self->native_handle;
		auto screen = SDL_GetWindowSurface(window);

		auto screen_pixels = screen->pixels;
		for (i32 i = 0; i < width * height * 4; ++i)
			((u8*)screen->pixels)[i] = ((u8*)pixels)[i];

		SDL_UpdateWindowSurface(window);
	}
}

#endif