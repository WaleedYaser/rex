#pragma once

#include "rex-core/exports.h"

#define REX_LOG_WARN_ENABLED  (1)
#define REX_LOG_INFO_ENABLED  (1)
#define REX_LOG_DEBUG_ENABLED (1)
#define REX_LOG_TRACE_ENABLED (1)
#define REX_LOG_ERROR_ENABLED (1)
#define REX_LOG_FATAL_ENABLED (1)

#if REX_RELEASE
# define REX_LOG_DEBUG_ENABLED (0)
# define REX_LOG_TRACE_ENABLED (0)
#endif

namespace rc
{
	enum REX_LOG_LEVEL
	{
		REX_LOG_LEVEL_TRACE = 0,
		REX_LOG_LEVEL_DEBUG = 1,
		REX_LOG_LEVEL_INFO  = 2,
		REX_LOG_LEVEL_WARN  = 3,
		REX_LOG_LEVEL_ERROR = 4,
		REX_LOG_LEVEL_FATAL = 5
	};

	REX_CORE_EXPORT void log(REX_LOG_LEVEL level, const char *message, ...);
}

#if REX_LOG_TRACE_ENABLED == 1
# define rex_log_trace(message, ...) rc::log(rc::REX_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
# define rex_log_trace(message, ...) // do nothing
#endif

#if REX_LOG_DEBUG_ENABLED == 1
# define rex_log_debug(message, ...) rc::log(rc::REX_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
# define rex_log_debug(message, ...) // do nothing
#endif

#if REX_LOG_INFO_ENABLED == 1
# define rex_log_info(message, ...) rc::log(rc::REX_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
# define rex_log_info(message, ...) // do nothing
#endif

#if REX_LOG_WARN_ENABLED == 1
# define rex_log_warn(message, ...) rc::log(rc::REX_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
# define rex_log_warn(message, ...) // do nothing
#endif

#if REX_LOG_ERROR_ENABLED == 1
# define rex_log_error(message, ...) rc::log(rc::REX_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
# define rex_log_error(message, ...) // do nothing
#endif

#if REX_LOG_FATAL_ENABLED == 1
# define rex_log_fatal(message, ...) rc::log(rc::REX_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#else
# define rex_log_fatal(message, ...) // do nothing
#endif
