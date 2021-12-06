#pragma once

#include "rex-core/exports.h"

namespace rc
{
	enum CONSOLE_COLOR
	{
		CONSOLE_COLOR_FG_GRAY,
		CONSOLE_COLOR_FG_BLUE,
		CONSOLE_COLOR_FG_GREEN,
		CONSOLE_COLOR_FG_YELLOW,
		CONSOLE_COLOR_FG_RED,
		CONSOLE_COLOR_BG_RED
	};

	REX_CORE_EXPORT void console_write(const char *message, CONSOLE_COLOR color);
	REX_CORE_EXPORT void console_write_error(const char *message, CONSOLE_COLOR color);
}
