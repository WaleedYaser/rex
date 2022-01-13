#if REX_OS_WASM

#include "rex-core/window.h"
#include "rex-core/assert.h"
#include "rex-core/memory.h"
#include "rex-core/str.h"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace rc
{
	inline static KEY
	_key_from_code(const char* code)
	{
		auto code_str = str_lit(code);

		// numbers
		if (code_str == "Digit0") return KEY_0;
		if (code_str == "Digit1") return KEY_1;
		if (code_str == "Digit2") return KEY_2;
		if (code_str == "Digit3") return KEY_3;
		if (code_str == "Digit4") return KEY_4;
		if (code_str == "Digit5") return KEY_5;
		if (code_str == "Digit6") return KEY_6;
		if (code_str == "Digit7") return KEY_7;
		if (code_str == "Digit8") return KEY_8;
		if (code_str == "Digit9") return KEY_9;
		// letters
		if (code_str == "KeyA") return KEY_A;
		if (code_str == "KeyB") return KEY_B;
		if (code_str == "KeyC") return KEY_C;
		if (code_str == "KeyD") return KEY_D;
		if (code_str == "KeyE") return KEY_E;
		if (code_str == "KeyF") return KEY_F;
		if (code_str == "KeyG") return KEY_G;
		if (code_str == "KeyH") return KEY_H;
		if (code_str == "KeyI") return KEY_I;
		if (code_str == "KeyJ") return KEY_J;
		if (code_str == "KeyK") return KEY_K;
		if (code_str == "KeyL") return KEY_L;
		if (code_str == "KeyM") return KEY_M;
		if (code_str == "KeyN") return KEY_N;
		if (code_str == "KeyO") return KEY_O;
		if (code_str == "KeyP") return KEY_P;
		if (code_str == "KeyQ") return KEY_Q;
		if (code_str == "KeyR") return KEY_R;
		if (code_str == "KeyS") return KEY_S;
		if (code_str == "KeyT") return KEY_T;
		if (code_str == "KeyU") return KEY_U;
		if (code_str == "KeyV") return KEY_V;
		if (code_str == "KeyW") return KEY_W;
		if (code_str == "KeyX") return KEY_X;
		if (code_str == "KeyY") return KEY_Y;
		if (code_str == "KeyZ") return KEY_Z;
		// special keys
		if (code_str == "Enter")  return KEY_ENTER;
		if (code_str == "Escape") return KEY_ESCAPE;
		if (code_str == "Space")  return KEY_SPACE;

		return KEY_NONE;
	}
	EM_BOOL
	_resize_callback(int event_type, const EmscriptenUiEvent* ui_event, void* user_data)
	{
		auto self = (Window*)user_data;

		double canvas_width, canvas_height;
		emscripten_get_element_css_size("#canvas", &canvas_width, &canvas_height);

		i32 width = (i32)canvas_width;
		i32 height = (i32)canvas_height;

		if (self->width != width || self->height != height)
		{
			self->width = width;
			self->height = height;
			emscripten_set_canvas_element_size("#canvas", width, height);

			if (self->event_callback)
			{
				Event event = {};
				event.type = EVENT_TYPE_WINDOW_RESIZE;
				event.window_resize.width = width;
				event.window_resize.height = height;
				self->event_callback(self, event);
			}
		}

		return true;
	}

	EM_BOOL
	_mouse_callback(int event_type, const EmscriptenMouseEvent* mouse_event, void* user_data)
	{
		auto self = (Window*)user_data;

		if (self->event_callback)
		{
			switch (event_type)
			{
				case EMSCRIPTEN_EVENT_MOUSEMOVE:
				{
					Event event = {};
					event.type = EVENT_TYPE_MOUSE_MOVE;
					event.mouse_move.x = mouse_event->targetX;
					event.mouse_move.y = mouse_event->targetY;
					self->event_callback(self, event);
					break;
				}
				case EMSCRIPTEN_EVENT_MOUSEDOWN:
				case EMSCRIPTEN_EVENT_MOUSEUP:
				{
					Event event = {};
					if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
						event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
					else
						event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;

					if (mouse_event->button == 0)
						event.mouse_button_press.button = MOUSE_BUTTON_LEFT;
					else if (mouse_event->button == 1)
						event.mouse_button_press.button = MOUSE_BUTTON_MIDDLE;
					else if (mouse_event->button == 2)
						event.mouse_button_press.button = MOUSE_BUTTON_RIGHT;

					self->event_callback(self, event);
					break;
				}
				default:
					return true;
			}
		}
		return true;
	}

	EM_BOOL
	_wheel_callback(int event_type, const EmscriptenWheelEvent* wheel_event, void* user_data)
	{
		auto self = (Window*)user_data;

		if (self->event_callback)
		{
			Event event = {};
			if (wheel_event->deltaY < 0)
			{
				event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP;
				self->event_callback(self, event);
			}
			else if (wheel_event->deltaY > 0)
			{
				event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN;
				self->event_callback(self, event);
			}
		}

		return true;
	}

	EM_BOOL
	_key_callback(int event_type, const EmscriptenKeyboardEvent* key_event, void* user_data)
	{
		auto self = (Window*)user_data;

		if (self->event_callback)
		{
			if (event_type == EMSCRIPTEN_EVENT_KEYDOWN)
			{
				Event event = {};
				event.type = EVENT_TYPE_KEY_PRESS;
				event.key_press.key = _key_from_code(key_event->code);
				self->event_callback(self, event);
			}
			else if (event_type == EMSCRIPTEN_EVENT_KEYUP)
			{
				Event event = {};
				event.type = EVENT_TYPE_KEY_RELEASE;
				event.key_press.key = _key_from_code(key_event->code);
				self->event_callback(self, event);
			}
		}
		return true;
	}

	Window*
	window_init(const char *title, i32 width, i32 height, void* user_data, event_callback_t event_callback)
	{
		auto self = rex_alloc_zeroed_T(Window);

		// override width and height
		double canvas_width, canvas_height;
		emscripten_get_element_css_size("#canvas", &canvas_width, &canvas_height);
		width = (i32)canvas_width;
		height = (i32)canvas_height;
		emscripten_set_canvas_element_size("#canvas", width, height);

		self->title = title;
		self->width = width;
		self->height = height;
		self->user_data = user_data;
		self->event_callback = event_callback;

		// TODO: what is useCapture?!
		emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _resize_callback);
		emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _mouse_callback);
		emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _mouse_callback);
		emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _mouse_callback);
		emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _wheel_callback);
		emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _key_callback);
		emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, true, _key_callback);

		return self;
	}

	void
	window_deinit(Window* self)
	{

	}

	void
	window_poll(Window* self)
	{
		// do nothing
	}

	void
	window_title_set(Window* self, const char* title)
	{

	}

	void
	window_blit(Window* self, u32* pixels, i32 width, i32 height)
	{
		// source: https://stackoverflow.com/a/50832655
		EM_ASM_({
			let data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
			let context = Module['canvas'].getContext('2d');
			let imageData = context.getImageData(0, 0, $1, $2);
			imageData.data.set(data);
			context.putImageData(imageData, 0, 0);
		}, pixels, width, height);
	}
}

#endif