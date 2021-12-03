#pragma once

#include "rex-core/exports.h"

// switches to enable or disable specific logging level.
#define REX_LOG_WARN_ENABLED  (1)
#define REX_LOG_INFO_ENABLED  (1)
#define REX_LOG_DEBUG_ENABLED (1)
#define REX_LOG_TRACE_ENABLED (1)
#define REX_LOG_ERROR_ENABLED (1)
#define REX_LOG_FATAL_ENABLED (1)

#if REX_RELEASE
// disable debug and trace logging in release mode.
# define REX_LOG_DEBUG_ENABLED (0)
# define REX_LOG_TRACE_ENABLED (0)
#endif

namespace rex::core
{
	typedef enum REX_LOG_LEVEL {
		REX_LOG_LEVEL_TRACE = 0,
		REX_LOG_LEVEL_DEBUG = 1,
		REX_LOG_LEVEL_INFO  = 2,
		REX_LOG_LEVEL_WARN  = 3,
		REX_LOG_LEVEL_ERROR = 4,
		REX_LOG_LEVEL_FATAL = 5
	} REX_LOG_LEVEL;

	// log formated message based on it's log level, most propably you will not need to call this
	// function directly and will use bellow macros.
	//  level: REX_LOG_LEVEL enum to specify log level.
	//  message: string to log.
	//  ...: args to pass for formatting the message.
	REX_CORE_EXPORT void
	log(REX_LOG_LEVEL level, const char *message, ...);
}

#if REX_LOG_TRACE_ENABLED == 1
// __VA_ARGS__: accept variable number of arguments in macro.
// we add '##' before __VA_ARGS__ to remove extra ',' before it if variable arguments are omitted or empty.
# define rex_log_trace(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
# define rex_log_trace(message, ...) // do nothing
#endif

#if REX_LOG_DEBUG_ENABLED == 1
# define rex_log_debug(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
# define rex_log_debug(message, ...) // do nothing
#endif

#if REX_LOG_INFO_ENABLED == 1
# define rex_log_info(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
# define rex_log_info(message, ...) // do nothing
#endif

#if REX_LOG_WARN_ENABLED == 1
# define rex_log_warn(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
# define rex_log_warn(message, ...) // do nothing
#endif

#if REX_LOG_ERROR_ENABLED == 1
# define rex_log_error(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
# define rex_log_error(message, ...) // do nothing
#endif

#if REX_LOG_FATAL_ENABLED == 1
# define rex_log_fatal(message, ...) rex::core::log(rex::core::REX_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#else
# define rex_log_fatal(message, ...) // do nothing
#endif
