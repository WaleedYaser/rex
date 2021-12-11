#include "rex-core/exports.h"
#include "rex-core/types.h"

namespace rc
{
	REX_CORE_EXPORT i64 time_milliseconds();
	REX_CORE_EXPORT void sleep(u32 milliseconds);
}
