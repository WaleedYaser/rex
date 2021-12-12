#if REX_OS_LINUX

#include "rex-core/path.h"
#include "rex-core/log.h"

#include <limits.h> /* PATH_MAX */
#include <unistd.h>
#include <stdlib.h>

namespace rc
{
	const char*
	app_directory()
	{
		static char buffer[PATH_MAX] = {};
		static bool init = true;
		if (init)
		{
			init = false;

			char relative[PATH_MAX] = {};
			readlink("/proc/self/exe", relative, sizeof(relative));
			realpath(relative, buffer);

			char* last_slash = buffer;
			char* iter = buffer;
			while (*iter++)
				if (*iter == '/')
					last_slash = ++iter;
			*last_slash = '\0';

			rex_log_info(buffer);
		}
		return buffer;
	}
}

#endif
