#include "rex-core/log.h"
#include "rex-core/console.h"
#include "rex-core/str.h"

#include <stdio.h>
#include <stdarg.h>

namespace rc
{
	void
	log(REX_LOG_LEVEL level, const char *message, ...)
	{
		const char *level_strings[6] = {
			"[REX TRACE]: ",
			"[REX DEBUG]: ",
			"[REX INFO]:  ",
			"[REX WARN]:  ",
			"[REX ERROR]: ",
			"[REX FATAL]: "
		};

		// get variadic arguments after message and format it into formated message, note that we have
		// a 32K limmit for the formated message.
		char formated_message[32 * 1024];
		va_list arg_ptr;
		va_start(arg_ptr, message);
		vsnprintf(formated_message, sizeof(formated_message), message, arg_ptr);
		va_end(arg_ptr);

		// add level string to formated message and store result in message_with_level, we have the
		// same size limit for message_with_level
		auto message_with_level = str_fmt(frame_allocator(), "%s%s\n", level_strings[level], formated_message);

		CONSOLE_COLOR color = {};
		switch (level)
		{
			case REX_LOG_LEVEL_TRACE: color = CONSOLE_COLOR_FG_GRAY; break;
			case REX_LOG_LEVEL_DEBUG: color = CONSOLE_COLOR_FG_BLUE; break;
			case REX_LOG_LEVEL_INFO:  color = CONSOLE_COLOR_FG_GREEN; break;
			case REX_LOG_LEVEL_WARN:  color = CONSOLE_COLOR_FG_YELLOW; break;
			case REX_LOG_LEVEL_ERROR: color = CONSOLE_COLOR_FG_RED; break;
			case REX_LOG_LEVEL_FATAL: color = CONSOLE_COLOR_BG_RED; break;
		}

		// if level is error write it to error console instead
		if (level > REX_LOG_LEVEL_WARN)
			console_write_error(message_with_level.ptr, color);
		else
			console_write(message_with_level.ptr, color);
	}
}
