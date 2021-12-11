#if REX_OS_WINDOWS

#include "rex-core/api.h"
#include "rex-core/path.h"
#include "rex-core/str.h"
#include "rex-core/assert.h"

#include <Windows.h>

#if REX_HOT_RELOAD == 0
extern "C" Rex_Api* rex_api(Rex_Api* api, bool reload);
#endif

struct Rex_Api_RAII
{
	Rex_Api* api = nullptr;
	rex_api_proc proc = nullptr;

	~Rex_Api_RAII()
	{
		if (api && proc)
			api = proc(api, false);
	}
};

Rex_Api*
load_rex_api()
{
	static Rex_Api_RAII rex;

#if REX_HOT_RELOAD == 1
	Str dll_path = str_fmt(frame_allocator(), "%s/rex-raster.dll", app_director());
	Str tmp_path = str_fmt(frame_allocator(), "%s/rex-raster_tmp.dll", app_director());

	WIN32_FILE_ATTRIBUTE_DATA data = {};
	bool res = GetFileAttributesExA(dll_path.ptr, GetFileExInfoStandard, &data);
	rex_assert_msg(res, "failed to get 'rex-raster.dll'attributes");
	if (res == false)
		return;

	static FILETIME last_time;
	if (CompareFileTime(&last_time, &data.ftLastWriteTime) == 0)
		return;

	static HMODULE rex_dll;
	if (rex_dll)
	{
		res = FreeLibrary(rex_dll);
		rex_assert_msg(res, "failed to unload rex-raster.dll");
	}

	bool copy_succeeded = CopyFileA(dll_path.ptr, tmp_path.ptr, false);

	rex_dll = LoadLibraryA(tmp_path.ptr);
	rex_assert_msg(rex_dll, "failed to load rex-raster_tmp.dll");

	rex.proc = ((rex_api_proc)GetProcAddress(rex_dll, "rex_api"));
	rex.api = rex.proc(rex.api, true);

	if (copy_succeeded)
		last_time = data.ftLastWriteTime;
#else
	if (rex.api == nullptr)
	{
		rex.api = rex_api(nullptr, false);
		rex.proc = rex_api;
	}
#endif

	return rex.api;
}

#endif