#include "rex-core/assert.h"
#include "rex-core/log.h"

namespace rex::core
{
	void
	report_assertion_failure(const char *expression, const char *message, const char *file, i32 line)
	{
		log(REX_LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n",
			expression, message, file, line);
	}
}
