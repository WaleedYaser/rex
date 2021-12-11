#ifndef UNICODE
#define UNICODE
#endif

#include <rex-core/api.h>
#include <rex-core/version.h>
#include <rex-core/window.h>
#include <rex-core/memory.h>
#include <rex-core/str.h>

#include <windows.h>
#include <assert.h>

static Rex_Api* g_rex;

inline static void*
_alloc(size_t size)
{
	return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

inline static void
_free(void* ptr)
{
	VirtualFree(ptr, 0, MEM_RELEASE);
}

inline static Content
_file_read(const char* path)
{
	HANDLE hfile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
		return {};

	Content res = {};
	res.size = GetFileSize(hfile, nullptr);
	if (res.size == INVALID_FILE_SIZE)
		return {};

	DWORD bytes_read = 0;
	res.data = (unsigned char*)_alloc(res.size);
	bool read_res = ReadFile(hfile, res.data, res.size, &bytes_read, nullptr);
	CloseHandle(hfile);

	if (read_res == false)
	{
		_free(res.data);
		return {};
	}

	return res;
}


inline static void
_load_rex_module()
{
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

	g_rex = ((rex_api_proc)GetProcAddress(rex_dll, "rex_api"))(g_rex, true);

	if (copy_succeeded)
		last_time = data.ftLastWriteTime;
#else
	if (g_rex) return;
	auto rex_dll = LoadLibrary(L"rex-raster.dll");
	assert(rex_dll && "failed to load rex.dll");
	g_rex = ((rex_api_proc)GetProcAddress(rex_dll, "rex_api"))(g_rex, true);
#endif
}

int main()
{
	// set current directory to process directory
	wchar_t buffer[1024] = {};
	GetModuleFileName(0, buffer, sizeof(buffer));
	wchar_t *last_slash = buffer;
	wchar_t *iter = buffer;
	while (*iter++)
		if (*iter == L'\\')
			last_slash = ++iter;
	*last_slash = L'\0';
	bool res = SetCurrentDirectory(buffer);
	assert(res && "SetCurrentDirectory failed");

	// load rex module for the first time and initialize it
	_load_rex_module();
	// initialize platform functions
	g_rex->alloc = _alloc;
	g_rex->free = _free;
	g_rex->file_read = _file_read;
	g_rex->init(g_rex);

	auto window = rc::window_init("Rex", 1280, 720, g_rex,
		[](rc::Window* window, rc::i32 width, rc::i32 height) {
			auto rex = (Rex_Api*)window->user_data;
			rex->window_width = width;
			rex->window_height = height;
			rex->loop(rex);
			auto pixels = rex_alloc_N_from(rc::frame_allocator(), rc::Color_U8, rex->canvas.width * rex->canvas.height);
			for (rc::i32 i = 0; i < rex->canvas.width * rex->canvas.height; ++i)
			{
				pixels[i].r = (rc::u8)(rex->canvas.pixels[i].r * 255);
				pixels[i].g = (rc::u8)(rex->canvas.pixels[i].g * 255);
				pixels[i].b = (rc::u8)(rex->canvas.pixels[i].b * 255);
				pixels[i].a = (rc::u8)(rex->canvas.pixels[i].a * 255);
			}
			rc::window_blit(window, pixels, rex->canvas.width, rex->canvas.height);
	});

	// timing
	timeBeginPeriod(1);
	LARGE_INTEGER frequency = {};
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER prev_ticks;
	QueryPerformanceCounter(&prev_ticks);

	bool running = true;
	while (running)
	{
		_load_rex_module();
		window->user_data = g_rex;

		while (rc::window_poll(window))
		{
			if (window->last_event.type == rc::EVENT_TYPE_WINDOW_CLOSE)
			{
				running = false;
				break;
			}
		}

		// timing in milliseconds (target 30fps)
		LARGE_INTEGER ticks;
		QueryPerformanceCounter(&ticks);
		LONGLONG busy_ms = (ticks.QuadPart - prev_ticks.QuadPart) * 1000 / frequency.QuadPart;
		if (busy_ms < 33)
			Sleep((DWORD)(33 - busy_ms));

		QueryPerformanceCounter(&ticks);
		LONGLONG total_ms = (ticks.QuadPart - prev_ticks.QuadPart) * 1000 / frequency.QuadPart;
		prev_ticks = ticks;

		auto title = rc::str_fmt(rc::frame_allocator(), "Rex v%d.%d.%d [total: %dms, busy: %dms, free: %dms]",
			REX_VERSION_MAJOR, REX_VERSION_MINOR, REX_VERSION_PATCH, total_ms, busy_ms, total_ms - busy_ms);
		rc::window_title_set(window, title.ptr);

		// rex loop
		g_rex->dt = total_ms * 0.001f;
		window->resize_callback(window, window->width, window->height);
		rc::frame_allocator()->clear();
	}

	// free resources
	rc::window_deinit(window);
	g_rex->deinit(g_rex);
}