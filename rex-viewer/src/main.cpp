#include <rex-core/api.h>
#include <rex-core/version.h>
#include <rex-core/window.h>
#include <rex-core/memory.h>
#include <rex-core/str.h>
#include <rex-core/time.h>

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
		[](rc::Window* window, int32_t width, int32_t height) {
			auto rex = (Rex_Api*)window->user_data;
			rex->screen_width = width;
			rex->screen_height = height;

			rex_dealloc(rex->screen);
			rex->screen = rex_alloc_N(Rex_Pixel, width * height);

			rex->loop(rex);
			rc::window_blit(window, (uint32_t*)rex->screen, rex->screen_width, rex->screen_height);
	});

	// init timing
	rc::time_milliseconds();

#if REX_OS_WASM
	emscripten_request_animation_frame_loop(_rex_frame, window);
	// TODO: we don't free resources here
#else
	while (_rex_frame(0, window));

	// free resources
	rex_dealloc(rex->screen);
	rc::window_deinit(window);
	rex->deinit(rex);
#endif
}