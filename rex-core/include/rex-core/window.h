#pragma once

#include "rex-core/exports.h"
#include "rex-core/types.h"

namespace rc
{
	enum MOUSE_BUTTON
	{
		MOUSE_BUTTON_NONE,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,
	};

	enum KEY
	{
		KEY_NONE,
		// numbers
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		// letters
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		// special keys
		KEY_SPACE,
		KEY_ENTER,
		KEY_ESCAPE,
	};

	enum EVENT_TYPE
	{
		EVENT_TYPE_NONE,
		// mouse events
		EVENT_TYPE_MOUSE_BUTTON_PRESS,
		EVENT_TYPE_MOUSE_BUTTON_RELEASE,
		EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN,
		EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP,
		EVENT_TYPE_MOUSE_MOVE,
		// keyboard events
		EVENT_TYPE_KEY_PRESS,
		EVENT_TYPE_KEY_RELEASE,
		// window events
		EVENT_TYPE_WINDOW_RESIZE,
		EVENT_TYPE_WINDOW_REPAINT,
		EVENT_TYPE_WINDOW_CLOSE
	};

	struct Event
	{
		EVENT_TYPE type;
		union
		{
			struct
			{
				MOUSE_BUTTON button;
			} mouse_button_press, mouse_button_release;

			struct
			{
				i32 x, y; // relative to the top right of the window client area
			} mouse_move;

			struct
			{
				KEY key;
			} key_press, key_release;

			struct
			{
				i32 width, height; // client area new size
			} window_resize;
		};
	};

	using resize_callback_t = void (*)(struct Window*, i32, i32);

	struct Window
	{
		// READ ONLY variables. don't change them by yourself, we handle them internally.
		void* native_handle;
		const char* title;
		i32 width, height;
		Event last_event;

		void* user_data;
		resize_callback_t resize_callback;
	};

	REX_CORE_EXPORT Window*
	window_init(const char *title, i32 width, i32 height, void* user_data, resize_callback_t resize_callback);

	REX_CORE_EXPORT void window_deinit(Window* self);
	REX_CORE_EXPORT bool window_poll(Window* self);
	REX_CORE_EXPORT void window_title_set(Window* self, const char* title);
	REX_CORE_EXPORT void window_blit(Window* self, Color_U8 *pixels, i32 width, i32 height);
}
