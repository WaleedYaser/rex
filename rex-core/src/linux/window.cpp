#if REX_OS_LINUX

#include "rex-core/window.h"
#include "rex-core/assert.h"
#include "rex-core/memory.h"
#include "rex-core/str.h"

/*
 * usefull resource for xcb
 * https://www.x.org/releases/current/doc/libxcb/tutorial/index.html
 */
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_image.h>
#include <X11/keysym.h>
#include <stdlib.h>

namespace rc
{
	struct IWindow
	{
		Window window;

		xcb_screen_t* screen;
		xcb_connection_t *connection;
		xcb_window_t handle;
		xcb_atom_t wm_delete_win;
		xcb_key_symbols_t *key_symbols;
	};

	// internal function that map between xcb_keycode_t and MP_KEY enum
	static KEY
	_key_from_xcb_keycode(IWindow* self, xcb_keycode_t keycode)
	{
		xcb_keysym_t key = xcb_key_symbols_get_keysym(self->key_symbols, keycode, 0);
		switch (key)
		{
			// numbers
			case '0': return KEY_0;
			case '1': return KEY_1;
			case '2': return KEY_2;
			case '3': return KEY_3;
			case '4': return KEY_4;
			case '5': return KEY_5;
			case '6': return KEY_6;
			case '7': return KEY_7;
			case '8': return KEY_8;
			case '9': return KEY_9;
			// letters
			case 'a': return KEY_A;
			case 'b': return KEY_B;
			case 'c': return KEY_C;
			case 'd': return KEY_D;
			case 'e': return KEY_E;
			case 'f': return KEY_F;
			case 'g': return KEY_G;
			case 'h': return KEY_H;
			case 'i': return KEY_I;
			case 'j': return KEY_J;
			case 'k': return KEY_K;
			case 'l': return KEY_L;
			case 'm': return KEY_M;
			case 'n': return KEY_N;
			case 'o': return KEY_O;
			case 'p': return KEY_P;
			case 'q': return KEY_Q;
			case 'r': return KEY_R;
			case 's': return KEY_S;
			case 't': return KEY_T;
			case 'u': return KEY_U;
			case 'v': return KEY_V;
			case 'w': return KEY_W;
			case 'x': return KEY_X;
			case 'y': return KEY_Y;
			case 'z': return KEY_Z;
			// special keys
			case XK_Return: return KEY_ENTER;
			case XK_Escape: return KEY_ESCAPE;
			case XK_space: return KEY_SPACE;
			default: return KEY_NONE;
		}

		return KEY_NONE;
	}

	Window *
	window_init(const char *title, i32 width, i32 height, void* user_data, resize_callback_t resize_callback)
	{
		auto self = rex_alloc_zeroed_T(IWindow);

		self->window.title = title;
		self->window.width = width;
		self->window.height = height;
		self->window.user_data = user_data;
		self->window.resize_callback = resize_callback;

		// open connection to x server. use DISPLAY environment variable as the default dispaly name
		self->connection = xcb_connect(NULL, NULL);
		if (xcb_connection_has_error(self->connection))
		{
			rex_assert_msg(false, "[rex-core]: failed to connect to X server via XCB");
			rex_dealloc(self);
			return nullptr;
		}

		// get the first screen
		self->screen = xcb_setup_roots_iterator(xcb_get_setup(self->connection)).data;

		self->handle = xcb_generate_id(self->connection);

		// register event types.
		// XCB_CW_BACK_PIXEL: fill window bg with a single color
		// XCB_CW_EVENT_MASK: required if we want to process any event
		u32 mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
		// listen for mouse, keyboard, and window events
		// XCB_EVENT_MASK_STRUCTURE_NOTIFY: we use this to get notified when window is closeed
		u32 values[] = {
			self->screen->black_pixel,
			XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
			XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE
		};

		xcb_create_window(
			self->connection,               // connection
			XCB_COPY_FROM_PARENT,           // depthe (same as root)
			self->handle,                   // window id
			self->screen->root,             // parent window
			0, 0,                           // position
			width, height,                  // size
			10,                             // border width
			XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
			self->screen->root_visual,      // visual
			mask, values                    // masks
		);

		// show window
		xcb_map_window(self->connection, self->handle);

		// set window title
		xcb_change_property(
			self->connection,
			XCB_PROP_MODE_REPLACE,
			self->handle,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			str_len(title),
			title
		);

		// tell server to notify us when window manager attempts to destroy the window
		xcb_intern_atom_cookie_t wm_delete_cookie =
			xcb_intern_atom(self->connection, 0, str_len("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
		xcb_intern_atom_cookie_t wm_protocols_cookie =
			xcb_intern_atom(self->connection, 0, str_len("WM_PROTOCOLS"), "WM_PROTOCOLS");
		xcb_intern_atom_reply_t *wm_delete_reply =
			xcb_intern_atom_reply(self->connection, wm_delete_cookie, NULL);
		xcb_intern_atom_reply_t *wm_protocols_reply =
			xcb_intern_atom_reply(self->connection, wm_protocols_cookie, NULL);
		self->wm_delete_win = wm_delete_reply->atom;

		xcb_change_property(
			self->connection,
			XCB_PROP_MODE_REPLACE,
			self->handle,
			wm_protocols_reply->atom,
			4,
			32,
			1,
			&wm_delete_reply->atom
		);

		// flush the stream
		if (xcb_flush(self->connection) <= 0)
		{
			rex_assert_msg(false, "failed to flush connection");
			rex_dealloc(self);
			return nullptr;
		}

		// we use symbyo table to map between xcb_keycode_t and X11 keysym
		self->key_symbols = xcb_key_symbols_alloc(self->connection);
		if (self->key_symbols == NULL)
		{
			rex_assert_msg(false, "failed to allocate key symbols");
			rex_dealloc(self);
			return nullptr;
		}

		return &self->window;
	}

	void
	window_deinit(Window* window)
	{
		auto self = (IWindow *)window;

		free(self->key_symbols);
		xcb_destroy_window(self->connection, self->handle);
		xcb_disconnect(self->connection);

		rex_dealloc(self);
	}

	bool
	window_poll(Window* window)
	{
		auto self = (IWindow *)window;

		self->window.last_event = {};

		// poll next events and return FALSE if there is no unprocessed events, the event is allocated
		// on the heap that's why we need to free it at the end.
		xcb_generic_event_t *event = xcb_poll_for_event(self->connection);
		if (event == NULL)
			return false;

		// the "& !0x80" is weired I know and I'm not sure why do we need this but that's what the
		// examples do
		switch (event->response_type & ~0x80)
		{
			case XCB_BUTTON_PRESS:
			{
				auto bp = (xcb_button_press_event_t *)event;
				switch (bp->detail)
				{
					case 1:
						self->window.last_event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_LEFT;
						break;
					case 2:
						self->window.last_event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_MIDDLE;
						break;
					case 3:
						self->window.last_event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_RIGHT;
						break;
					case 4:
						self->window.last_event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP;
						break;
					case 5:
						self->window.last_event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN;
						break;
					default:
						// do nothing
						break;
				}
				break;
			}
			case XCB_BUTTON_RELEASE:
			{
				auto br = (xcb_button_release_event_t *)event;
				switch (br->detail)
				{
					case 1:
						self->window.last_event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_LEFT;
						break;
					case 2:
						self->window.last_event.type =EVENT_TYPE_MOUSE_BUTTON_RELEASE;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_MIDDLE;
						break;
					case 3:
						self->window.last_event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
						self->window.last_event.mouse_button_press.button = MOUSE_BUTTON_RIGHT;
						break;
					case 4:
					case 5:
					default:
						// do nothing
						break;
				}
				break;
			}
			case XCB_MOTION_NOTIFY:
			{
				auto mn = (xcb_motion_notify_event_t *)event;
				self->window.last_event.type = EVENT_TYPE_MOUSE_MOVE;
				self->window.last_event.mouse_move.x = mn->event_x;
				self->window.last_event.mouse_move.y = mn->event_y;
				break;
			}
			case XCB_KEY_PRESS:
			{
				auto kp =(xcb_key_press_event_t *)event;
				self->window.last_event.type = EVENT_TYPE_KEY_PRESS;
				self->window.last_event.key_press.key = _key_from_xcb_keycode(self, kp->detail);
				break;
			}
			case XCB_KEY_RELEASE:
			{
				auto kr =(xcb_key_release_event_t *)event;
				self->window.last_event.type = EVENT_TYPE_KEY_RELEASE;
				self->window.last_event.key_release.key = _key_from_xcb_keycode(self, kr->detail);
				break;
			}
			case XCB_CONFIGURE_NOTIFY:
			{
				auto cn = (xcb_configure_notify_event_t *)event;
				if (cn->width != self->window.width || cn->height != self->window.height)
				{
					self->window.last_event.type = EVENT_TYPE_WINDOW_RESIZE;
					self->window.last_event.window_resize.width = cn->width;
					self->window.last_event.window_resize.height = cn->height;

					self->window.width = cn->width;
					self->window.height = cn->height;

					if (self->window.resize_callback)
						self->window.resize_callback(&self->window, self->window.width, self->window.height);
				}
				break;
			}
			case XCB_CLIENT_MESSAGE:
			{
				auto cm = (xcb_client_message_event_t *)event;
				if (cm->data.data32[0] == self->wm_delete_win)
				{
					self->window.last_event.type = EVENT_TYPE_WINDOW_CLOSE;
				}
				break;
			}
			case XCB_MAP_NOTIFY:
			case XCB_MAPPING_NOTIFY:
			case XCB_REPARENT_NOTIFY:
				// do nothing
				break;
			default:
				// rex_assert_msg(false, "[rex-core]: unexpected platform window event");
				break;
		}
		free(event);

		return true;
	}

	void
	window_title_set(Window* window, const char* title)
	{
		auto self = (IWindow *)window;

		self->window.title = title;

		xcb_change_property(
			self->connection,
			XCB_PROP_MODE_REPLACE,
			self->handle,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			str_len(title),
			title
		);
	}

	void
	window_blit(Window* window, Color_U8 *pixels, i32 width, i32 height)
	{
		auto self = (IWindow *)window;

		auto pixmap = xcb_generate_id(self->connection);
		xcb_create_pixmap(
			self->connection,
			self->screen->root_depth,
			pixmap,
			self->screen->root,
			width, height
		);

		auto bgra = rex_alloc_N_from(frame_allocator(), Color_U8, width * height);
		for (i32 i = 0; i < width * height; ++i)
		{
			bgra[i].b = pixels[i].r;
			bgra[i].g = pixels[i].g;
			bgra[i].r = pixels[i].b;
			bgra[i].a = pixels[i].a;
		}

		auto graphics_ctx = xcb_generate_id(self->connection);
		xcb_create_gc(self->connection, graphics_ctx, pixmap, 0, nullptr);
		auto image = xcb_image_create_native(
			self->connection,
			width, height,
			XCB_IMAGE_FORMAT_Z_PIXMAP,
			self->screen->root_depth,
			bgra,
			width * height * sizeof(Color_U8),
			(uint8_t*)bgra
		);
		xcb_image_put(self->connection, pixmap, graphics_ctx, image, 0, 0, 0);

		xcb_copy_area(
			self->connection,
			pixmap,
			self->handle,
			graphics_ctx,
			0, 0,
			0, 0,
			width, height
		);
		xcb_flush(self->connection);

		xcb_free_pixmap(self->connection, pixmap);
		xcb_free_gc(self->connection, graphics_ctx);
	}
}

#endif