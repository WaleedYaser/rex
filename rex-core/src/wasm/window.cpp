#if REX_OS_WASM

#include "rex-core/window.h"
#include "rex-core/assert.h"
#include "rex-core/memory.h"
#include "rex-core/str.h"
#include "rex-core/log.h"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace rc
{
	EM_BOOL
	_canvas_resize_callback(int event_type, const EmscriptenUiEvent* ui_event, void* user_data)
	{
		auto self = (Window*)user_data;

		double canvas_width, canvas_height;
		emscripten_get_element_css_size("#canvas", &canvas_width, &canvas_height);

		i32 width = (i32)canvas_width;
		i32 height = (i32)canvas_height;

		if (self->width != width || self->height != height)
		{
			self->last_event.type = EVENT_TYPE_WINDOW_RESIZE;
			self->last_event.window_resize.width = width;
			self->last_event.window_resize.height = height;

			self->width = width;
			self->height = height;
			emscripten_set_canvas_element_size("#canvas", width, height);

			if (self->resize_callback)
				self->resize_callback(self, self->width, self->height);
		}

		return true;
	}

	Window*
	window_init(const char *title, i32 width, i32 height, void* user_data, resize_callback_t resize_callback)
	{
		auto self = rex_alloc_zeroed_T(Window);

		// override width and height
		double canvas_width, canvas_height;
		emscripten_get_element_css_size("#canvas", &canvas_width, &canvas_height);
		emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, self, false, _canvas_resize_callback);

		width = (i32)canvas_width;
		height = (i32)canvas_height;
		emscripten_set_canvas_element_size("#canvas", width, height);

		self->title = title;
		self->width = width;
		self->height = height;
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
		return false;
	}

	void
	window_title_set(Window* self, const char* title)
	{

	}

	void
	window_blit(Window* self, Color_U8 *pixels, i32 width, i32 height)
	{
		// source: https://stackoverflow.com/a/50832655
		EM_ASM_({
			let data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
			let context = Module['canvas'].getContext('2d');
			let imageData = context.getImageData(0, 0, $1, $2);
			imageData.data.set(data);
			context.putImageData(imageData, 0, 0);
			console.log($1, $2);
		}, (u32*)pixels, width, height);
	}
}

#endif