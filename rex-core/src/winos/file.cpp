#if REX_OS_WINDOWS

#include "rex-core/file.h"
#include <windows.h>

namespace rc
{
	Str
	file_read(Str path, Allocator allocator)
	{
		HANDLE hfile = CreateFileA(path.ptr, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (hfile == INVALID_HANDLE_VALUE)
			return {};


		auto size = GetFileSize(hfile, nullptr);
		if (size == INVALID_FILE_SIZE)
			return {};

		Str res = vec_with_capacity<char>(size + 1, allocator);

		DWORD bytes_read = 0;
		bool read_res = ReadFile(hfile, res.ptr, size, &bytes_read, nullptr);
		CloseHandle(hfile);

		if (read_res == false)
		{
			str_deinit(res);
			return {};
		}

		res.count = size;
		res[size] = '\0';

		return res;
	}
}

#endif
