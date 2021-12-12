#if REX_OS_WASM

#include "rex-core/console.h"
#include "rex-core/types.h"

#include <cstdio>

namespace rc
{
	static const char *
	_console_color_to_str(CONSOLE_COLOR color)
	{
		switch (color)
		{
			case CONSOLE_COLOR_FG_GRAY: return "1;30";
			case CONSOLE_COLOR_FG_BLUE: return "1;34";
			case CONSOLE_COLOR_FG_GREEN: return "1;32";
			case CONSOLE_COLOR_FG_YELLOW: return "1;33";
			case CONSOLE_COLOR_FG_RED: return "1;31";
			case CONSOLE_COLOR_BG_RED: return "0;41";
		}
		return "";
	}

	void
	console_write(const char *message, CONSOLE_COLOR color)
	{
		fprintf(stdout, "\033[%sm%s\033[0m", _console_color_to_str(color), message);
	}

	void
	console_write_error(const char *message, CONSOLE_COLOR color)
	{
		fprintf(stderr, "\033[%sm%s\033[0m", _console_color_to_str(color), message);
	}
}

#endif
