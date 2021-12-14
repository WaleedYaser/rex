#include <rex-core/api.h>
#include <rex-core/version.h>
#include <rex-core/window.h>
#include <rex-core/memory.h>
#include <rex-core/str.h>
#include <rex-core/time.h>
#include <rex-core/log.h>

#if REX_OS_WASM
# include <emscripten/emscripten.h>
# include <emscripten/html5.h>
#endif

inline static int
_rex_frame(double, void* user_data)
{
	auto window = (rc::Window*)user_data;

	auto rex = load_rex_api();
	window->user_data = rex;

	while (rc::window_poll(window))
	{
		if (window->last_event.type == rc::EVENT_TYPE_WINDOW_CLOSE)
			return false;
	}

	auto busy_ms = rc::time_milliseconds();
	if (busy_ms < 33)
		rc::sleep((rc::u32)(33 - busy_ms));

	auto free_ms = rc::time_milliseconds();
	auto frame_ms = busy_ms + free_ms;

	auto title = rc::str_fmt(rc::frame_allocator(), "Rex v%d.%d.%d [frame: %lldms, busy: %lldms, free: %lldms]",
		REX_VERSION_MAJOR, REX_VERSION_MINOR, REX_VERSION_PATCH, frame_ms, busy_ms, free_ms);
	rc::window_title_set(window, title.ptr);

	// rex loop
	rex->dt = frame_ms * 0.001f;
	window->resize_callback(window, window->width, window->height);
	rc::frame_allocator()->clear();

	return true;
}

int main()
{
	// TODO: make sure memory allocators initialized first
	rc::rex_allocator();

	auto rex = load_rex_api();
	rex->init(rex);

	auto window = rc::window_init("Rex", 1280, 720, rex,
		[](rc::Window* window, rc::i32 width, rc::i32 height) {
			auto rex = (Rex_Api*)window->user_data;
			rex->window_width = width;
			rex->window_height = height;
			rex->loop(rex);
			auto pixels = rex_alloc_N_from(rc::frame_allocator(), rc::Color_U8, rex->canvas.width * rex->canvas.height);
			for (rc::i32 i = 0; i < rex->canvas.width * rex->canvas.height; ++i)
			{
				pixels[i].r = (rc::u8)(rex->canvas.pixels[i].r * 255);
				pixels[i].g = (rc::u8)(rex->canvas.pixels[i].g * 255);
				pixels[i].b = (rc::u8)(rex->canvas.pixels[i].b * 255);
				pixels[i].a = (rc::u8)(rex->canvas.pixels[i].a * 255);
			}
			rc::window_blit(window, pixels, rex->canvas.width, rex->canvas.height);
	});

	// init timing
	rc::time_milliseconds();

#if REX_OS_WASM
	emscripten_request_animation_frame_loop(_rex_frame, window);
#else
	while (_rex_frame(0, window));
#endif

	// free resources
	rc::window_deinit(window);
	rex->deinit(rex);
}