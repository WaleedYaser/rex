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
	SDL_Surface* g_screen;
	i32 g_width;
	i32 g_height;

	Window *
	window_init(const char *title, i32 width, i32 height, void* user_data, resize_callback_t resize_callback)
	{
		auto self = rex_alloc_zeroed_T(Window);

		SDL_Init(SDL_INIT_VIDEO);
		SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

		g_screen = screen;
		g_width = width;
		g_height = height;

		self->title = title;
		self->width = width;
		self->height = height;
		self->native_handle = screen;
		self->user_data = user_data;
		self->resize_callback = resize_callback;

		return self;
	}

	void
	window_deinit(Window* self)
	{

	}

	void drawRandomPixels() {
		if (SDL_MUSTLOCK(g_screen)) SDL_LockSurface(g_screen);

		u8* pixels = (u8*)g_screen->pixels;

		for (int i=0; i < g_width * g_height * 4; i++) {
			char randomByte = rand() % 255;
			pixels[i] = randomByte;
		}

		if (SDL_MUSTLOCK(g_screen)) SDL_UnlockSurface(g_screen);

		SDL_Flip(g_screen);
	}

	bool
	window_poll(Window* self)
	{
		// emscripten_set_main_loop(drawRandomPixels, 60, 1);

		return false;
	}

	void
	window_title_set(Window* self, const char* title)
	{

	}

	void
	window_blit(Window* self, Color_U8 *pixels, i32 width, i32 height)
	{
		auto screen = (SDL_Surface*)self->native_handle;

		if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

		auto screen_pixels = screen->pixels;
		for (i32 i = 0; i < width * height * 4; ++i)
			((u8*)screen->pixels)[i] = ((u8*)pixels)[i];

		if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

		SDL_Flip(screen);
	}
}

#endif