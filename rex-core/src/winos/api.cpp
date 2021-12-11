#if REX_OS_WINDOWS

#include "rex-core/api.h"
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
	WIN32_FILE_ATTRIBUTE_DATA data = {};
	bool res = GetFileAttributesEx(L"rex-raster.dll", GetFileExInfoStandard, &data);
	assert(res && "failed to get 'rex.dll'attributes");
	if (res == false)
		return;

	static FILETIME last_time;
	if (CompareFileTime(&last_time, &data.ftLastWriteTime) == 0)
		return;

	static HMODULE rex_dll;
	if (rex_dll)
	{
		res = FreeLibrary(rex_dll);
		assert(res && "failed to unload rex.dll");
	}

	bool copy_succeeded = CopyFile(L"rex-raster.dll", L"rex-raster_tmp.dll", false);

	rex_dll = LoadLibrary(L"rex-raster_tmp.dll");
	assert(rex_dll && "failed to load rex.dll");

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