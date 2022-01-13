// windows specific code, we include it on windows only
#if REX_OS_WINDOWS

#include "rex-core/window.h"
#include "rex-core/assert.h"
#include "rex-core/memory.h"

#include <Windows.h>
#include <windowsx.h> // For 'GET_X_LPARAM' and 'GET_Y_LPARAM'

namespace rc
{
	inline static KEY
	_key_from_wparam(WPARAM wparam)
	{
		switch (wparam)
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
			case 'A': return KEY_A;
			case 'B': return KEY_B;
			case 'C': return KEY_C;
			case 'D': return KEY_D;
			case 'E': return KEY_E;
			case 'F': return KEY_F;
			case 'G': return KEY_G;
			case 'H': return KEY_H;
			case 'I': return KEY_I;
			case 'J': return KEY_J;
			case 'K': return KEY_K;
			case 'L': return KEY_L;
			case 'M': return KEY_M;
			case 'N': return KEY_N;
			case 'O': return KEY_O;
			case 'P': return KEY_P;
			case 'Q': return KEY_Q;
			case 'R': return KEY_R;
			case 'S': return KEY_S;
			case 'T': return KEY_T;
			case 'U': return KEY_U;
			case 'V': return KEY_V;
			case 'W': return KEY_W;
			case 'X': return KEY_X;
			case 'Y': return KEY_Y;
			case 'Z': return KEY_Z;
			// special keys
			case VK_RETURN: return KEY_ENTER;
			case VK_ESCAPE: return KEY_ESCAPE;
			case VK_SPACE: return KEY_SPACE;
			default: return KEY_NONE;
		}
	}

	static LRESULT CALLBACK
	_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// store and get pointer to _MP_WINDOW_INTERNAL state
		Window *self = NULL;
		if (msg == WM_NCCREATE)
		{
			// we get WM_NCCREATE message before WM_CREATE, in this case we store user data into
			// handle to retrieve it later.
			CREATESTRUCTA *create = (CREATESTRUCTA *)lparam;
			self = (Window*)create->lpCreateParams;
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)self);
		}
		else
		{
			self = (Window *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
		}

		Event event = {};
		switch (msg)
		{
			case WM_LBUTTONDOWN:
			{
				// SetCapture make sure that when we click inside the the window and then
				// move cursor outside it we still get mouse events until we release the
				// mouse.
				SetCapture(hwnd);
				event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
				event.mouse_button_press.button = MOUSE_BUTTON_LEFT;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				SetCapture(hwnd);
				event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
				event.mouse_button_press.button = MOUSE_BUTTON_MIDDLE;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				SetCapture(hwnd);
				event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
				event.mouse_button_press.button = MOUSE_BUTTON_RIGHT;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_LBUTTONUP:
			{
				ReleaseCapture();
				event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
				event.mouse_button_press.button = MOUSE_BUTTON_LEFT;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_MBUTTONUP:
			{
				ReleaseCapture();
				event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
				event.mouse_button_press.button = MOUSE_BUTTON_MIDDLE;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_RBUTTONUP:
			{
				ReleaseCapture();
				event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
				event.mouse_button_press.button = MOUSE_BUTTON_RIGHT;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
					event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP;
				else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
					event.type = EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_MOUSEMOVE:
			{
				event.type = EVENT_TYPE_MOUSE_MOVE;
				event.mouse_move.x = GET_X_LPARAM(lparam);
				event.mouse_move.y = GET_Y_LPARAM(lparam);
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				event.type = EVENT_TYPE_KEY_PRESS;
				event.key_press.key = _key_from_wparam(wparam);
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				event.type = EVENT_TYPE_KEY_RELEASE;
				event.key_release.key = _key_from_wparam(wparam);
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_SIZE:
			{
				event.type = EVENT_TYPE_WINDOW_RESIZE;
				event.window_resize.width = LOWORD(lparam);
				event.window_resize.height = HIWORD(lparam);
				self->width = LOWORD(lparam);
				self->height = HIWORD(lparam);
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_PAINT:
			{
				event.type = EVENT_TYPE_WINDOW_REPAINT;
				if (self->event_callback)
					self->event_callback(self, event);
				break;
			}
			case WM_CLOSE:
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				event.type = EVENT_TYPE_WINDOW_CLOSE;
				if (self->event_callback)
					self->event_callback(self, event);
				return 0;
			}
		}
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	Window*
	window_init(const char *title, i32 width, i32 height, void* user_data, event_callback_t event_callback)
	{
		auto self = rex_alloc_zeroed_T(Window);

		// register window class
		WNDCLASSEXA wc = { };
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = _window_proc;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = "rex_window_class";
		ATOM class_atom = RegisterClassExA(&wc);
		if (class_atom == 0)
		{
			rex_assert_msg(false, "[rex-core]: failed to register window class");
			rex_dealloc(self);
			return nullptr;
		}

		// adjust width and height based on the style, taking border size for example into account
		DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		RECT wr = {0, 0, width, height};
		bool res = AdjustWindowRect(&wr, style, FALSE);
		if (res == false)
		{
			rex_assert_msg(false, "[rex-core]: failed to adjust window rect");
			rex_dealloc(self);
			return nullptr;
		}

		// create native window
		HWND handle = CreateWindowExA(
			0,                                      // optional window style
			MAKEINTATOM(class_atom),                // window class
			title,                                  // window text
			style,                                  // window style
			CW_USEDEFAULT, CW_USEDEFAULT,           // position
			wr.right - wr.left, wr.bottom - wr.top, // size
			NULL,                                   // parent window
			NULL,                                   // menu
			NULL,                                   // instance handle (NULL is default instance)
			self                                    // additional application data
		);
		if (handle == nullptr)
		{
			rex_assert_msg(false, "[rex-core]: failed to create window");
			rex_dealloc(self);
			return nullptr;
		}

		self->title = title;
		self->width = width;
		self->height = height;
		self->native_handle = handle;
		self->user_data = user_data;
		self->event_callback = event_callback;

		return self;
	}

	void
	window_deinit(Window* self)
	{
		if (DestroyWindow((HWND)self->native_handle) == false)
		{
			rex_assert_msg(false, "[rex-core]: failed to destroy window");
		}
		rex_dealloc(self);
	}

	void
	window_poll(Window* self)
	{
		MSG msg = {};
		while (PeekMessageA(&msg, (HWND)self->native_handle, 0, 0, PM_REMOVE))
		{
			// translate virtual-key messages into character messages
			TranslateMessage(&msg);
			// dispatch message to window procedure
			DispatchMessage(&msg);
		}
	}

	void
	window_title_set(Window* self, const char* title)
	{
		self->title = title;
		SetWindowTextA((HWND)self->native_handle, title);
	}

	void
	window_blit(Window* window, uint32_t* pixels, i32 width, i32 height)
	{
		HWND hwnd = (HWND)window->native_handle;

		HDC hdc = ::GetDC(hwnd);
		rex_assert_msg(hdc, "[rex-core]: failed to get device context");

		BITMAPINFO bitmap_info = {};
		bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
		bitmap_info.bmiHeader.biWidth = width;
		bitmap_info.bmiHeader.biHeight = -height; // negative to have origin at top left corner
		bitmap_info.bmiHeader.biPlanes = 1;
		bitmap_info.bmiHeader.biBitCount = 32;
		bitmap_info.bmiHeader.biCompression = BI_RGB;

		auto copied_scan_lines = ::StretchDIBits(
			hdc,
			0, 0, window->width, window->height, // destination
			0, 0, width, height, // source
			pixels,
			&bitmap_info,
			DIB_RGB_COLORS,
			SRCCOPY);
		rex_assert_msg(copied_scan_lines, "[rex-core]: failed to copy pixels to window rect");

		auto res = ::ReleaseDC(hwnd, hdc);
		rex_assert_msg(res, "[rex-core]: failed to release device context");
	}
}

#endif
