#if REX_OS_LINUX

#include "rex-core/file.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace rc
{
	Str
	file_read(Str path, Allocator allocator)
	{
		auto handle = ::open(path.ptr, O_RDONLY);
		if (handle == -1)
			return {};

		struct stat s = {};
		if (::fstat(handle, &s) != 0)
			return {};

		Str res = vec_with_capacity<char>(s.st_size + 1, allocator);
		auto bytes_read = ::read(handle, res.ptr, s.st_size);

		if (bytes_read != s.st_size)
		{
			str_deinit(res);
			return {};
		}

		res.count = s.st_size;
		res[s.st_size] = '\0';

		return res;
	}
}

#endif
