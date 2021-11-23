#include "rex-core/console.h"

#if REX_OS_WINDOWS

#include "rex-core/types.h"
#include <Windows.h>

namespace rex::core
{
	// internal function to map CONSOLE_COLOR to u8
	inline static u8
	_console_color_to_u8(CONSOLE_COLOR color)
	{
		switch (color)
		{
			case CONSOLE_COLOR_FG_GRAY: return 8;
			case CONSOLE_COLOR_FG_BLUE: return 1;
			case CONSOLE_COLOR_FG_GREEN: return 2;
			case CONSOLE_COLOR_FG_YELLOW: return 6;
			case CONSOLE_COLOR_FG_RED: return 4;
			case CONSOLE_COLOR_BG_RED: return 64;
		}
	}

	// API

	void
	console_write(const char *message, CONSOLE_COLOR color)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO info = {};
		GetConsoleScreenBufferInfo(console_handle, &info);

		SetConsoleTextAttribute(console_handle, _console_color_to_u8(color));
		OutputDebugStringA(message);
		u64 length = strnlen(message, 2 * 1024);
		WriteConsoleA(console_handle, message, (DWORD)length, NULL, NULL);

		SetConsoleTextAttribute(console_handle, info.wAttributes);
	}

	void
	console_write_error(const char *message, CONSOLE_COLOR color)
	{
		HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO info = {};
		GetConsoleScreenBufferInfo(console_handle, &info);

		SetConsoleTextAttribute(console_handle, _console_color_to_u8(color));
		OutputDebugStringA(message);
		u64 length = strnlen(message, 2 * 1024);
		WriteConsoleA(console_handle, message, (DWORD)length, NULL, NULL);

		SetConsoleTextAttribute(console_handle, info.wAttributes);
	}
}

#endif
