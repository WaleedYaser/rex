#include "rex-core/path.h"

#include <windows.h>

namespace rc
{
	const char*
	app_directory()
	{
		static char buffer[1024] = {};
		static bool init = true;
		if (init)
		{
			init = false;

			GetModuleFileNameA(0, buffer, sizeof(buffer));
			char* last_slash = buffer;
			char* iter = buffer;
			while (*iter++)
				if (*iter == '\\')
					last_slash = ++iter;
			*last_slash = '\0';
		}

		return buffer;
	}
}