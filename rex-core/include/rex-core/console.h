#pragma once

#include "rex-core/exports.h"

namespace rex::core
{
	typedef enum CONSOLE_COLOR {
		CONSOLE_COLOR_FG_GRAY,
		CONSOLE_COLOR_FG_BLUE,
		CONSOLE_COLOR_FG_GREEN,
		CONSOLE_COLOR_FG_YELLOW,
		CONSOLE_COLOR_FG_RED,
		CONSOLE_COLOR_BG_RED
	} CONSOLE_COLOR;

	// write a message to the console with the specified color.
	//  message: string for the message.
	//  color: CONSOLE_COLOR enum for the color.
	REX_CORE_EXPORT void
	console_write(const char *message, CONSOLE_COLOR color);

	// write a message to the error console (if available) with the specified color.
	//  message: string for the message.
	//  color: CONSOLE_COLOR enum for the color.
	REX_CORE_EXPORT void
	console_write_error(const char *message, CONSOLE_COLOR color);
}
