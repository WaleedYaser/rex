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

inline static REX_KEY
_rc_mouse_button_to_rex_key(rc::MOUSE_BUTTON button)
{
	switch(button)
	{
		case rc::MOUSE_BUTTON_LEFT: return REX_KEY_MOUSE_LEFT;
		case rc::MOUSE_BUTTON_RIGHT: return REX_KEY_MOUSE_RIGHT;
		case rc::MOUSE_BUTTON_MIDDLE: return REX_KEY_MOUSE_MIDDLE;
	}
	return REX_KEY_NONE;
}

inline static REX_KEY
_rc_key_to_rex_key(rc::KEY key)
{
	switch (key)
	{
		case rc::KEY_0: return REX_KEY_0;
		case rc::KEY_1: return REX_KEY_1;
		case rc::KEY_2: return REX_KEY_2;
		case rc::KEY_3: return REX_KEY_3;
		case rc::KEY_4: return REX_KEY_4;
		case rc::KEY_5: return REX_KEY_5;
		case rc::KEY_6: return REX_KEY_6;
		case rc::KEY_7: return REX_KEY_7;
		case rc::KEY_8: return REX_KEY_8;
		case rc::KEY_9: return REX_KEY_9;
		case rc::KEY_A: return REX_KEY_A;
		case rc::KEY_B: return REX_KEY_B;
		case rc::KEY_C: return REX_KEY_C;
		case rc::KEY_D: return REX_KEY_D;
		case rc::KEY_E: return REX_KEY_E;
		case rc::KEY_F: return REX_KEY_F;
		case rc::KEY_G: return REX_KEY_G;
		case rc::KEY_H: return REX_KEY_H;
		case rc::KEY_I: return REX_KEY_I;
		case rc::KEY_J: return REX_KEY_J;
		case rc::KEY_K: return REX_KEY_K;
		case rc::KEY_L: return REX_KEY_L;
		case rc::KEY_M: return REX_KEY_M;
		case rc::KEY_N: return REX_KEY_N;
		case rc::KEY_O: return REX_KEY_O;
		case rc::KEY_P: return REX_KEY_P;
		case rc::KEY_Q: return REX_KEY_Q;
		case rc::KEY_R: return REX_KEY_R;
		case rc::KEY_S: return REX_KEY_S;
		case rc::KEY_T: return REX_KEY_T;
		case rc::KEY_U: return REX_KEY_U;
		case rc::KEY_V: return REX_KEY_V;
		case rc::KEY_W: return REX_KEY_W;
		case rc::KEY_X: return REX_KEY_X;
		case rc::KEY_Y: return REX_KEY_Y;
		case rc::KEY_Z: return REX_KEY_Z;
		case rc::KEY_SPACE: return REX_KEY_SPACE;
		case rc::KEY_ENTER: return REX_KEY_ENTER;
		case rc::KEY_ESCAPE: return REX_KEY_ESCAPE;
	}
	return REX_KEY_NONE;
}


inline static int
_rex_frame(double, void* user_data)
{
	auto window = (rc::Window*)user_data;

	auto rex = load_rex_api();
	window->user_data = rex;

	for (int i = 0; i < REX_KEY_COUNT; ++i)
	{
		rex->input.keys[i].pressed = false;
		rex->input.keys[i].released = false;
		rex->input.keys[i].press_count = 0;
		rex->input.keys[i].release_count = 0;
	}
	rex->input.mouse_wheel = 0.0f;
	rex->input.mouse_dx = 0;
	rex->input.mouse_dy = 0;

	while (rc::window_poll(window))
	{
		if (window->last_event.type == rc::EVENT_TYPE_WINDOW_CLOSE)
			return false;

		switch (window->last_event.type)
		{
			case rc::EVENT_TYPE_MOUSE_BUTTON_PRESS:
			{
				auto button = _rc_mouse_button_to_rex_key(window->last_event.mouse_button_press.button);
				rex->input.keys[button].down = true;
				rex->input.keys[button].pressed = true;
				rex->input.keys[button].press_count++;
				break;
			}
			case rc::EVENT_TYPE_MOUSE_BUTTON_RELEASE:
			{
				auto button = _rc_mouse_button_to_rex_key(window->last_event.mouse_button_release.button);
				rex->input.keys[button].down = false;
				rex->input.keys[button].released = true;
				rex->input.keys[button].release_count++;
				break;
			}
			case rc::EVENT_TYPE_MOUSE_MOVE:
				rex->input.mouse_dx = window->last_event.mouse_move.x - rex->input.mouse_x;
				rex->input.mouse_dy = window->last_event.mouse_move.y - rex->input.mouse_y;
				rex->input.mouse_x = window->last_event.mouse_move.x;
				rex->input.mouse_y = window->last_event.mouse_move.y;
				break;
			case rc::EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN:
				// TODO: handle this properly
				rex->input.mouse_wheel = 1.0f;
				break;
			case rc::EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP:
				rex->input.mouse_wheel = -1.0f;
				break;
			case rc::EVENT_TYPE_KEY_PRESS:
			{
				auto key = _rc_key_to_rex_key(window->last_event.key_press.key);
				rex->input.keys[key].down = true;
				rex->input.keys[key].pressed = true;
				rex->input.keys[key].press_count++;
				break;
			}
			case rc::EVENT_TYPE_KEY_RELEASE:
			{
				auto key = _rc_key_to_rex_key(window->last_event.key_release.key);
				rex->input.keys[key].down = false;
				rex->input.keys[key].released = true;
				rex->input.keys[key].release_count++;
				break;
			}
		}
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