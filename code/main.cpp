#ifndef UNICODE
#define UNICODE
#endif

#include "rex.h"

#include <windows.h>
#include <assert.h>

static Rex g_rex;
static Rex_Api* g_api;

static DWORD* g_blt_bits;
static int g_blt_size;

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

inline static void
_paint(HWND hwnd)
{
    if (g_blt_size < g_rex.window_width * g_rex.window_height)
    {
        _free(g_blt_bits);

        g_blt_bits = (DWORD*)_alloc(g_rex.window_width * g_rex.window_height * sizeof(DWORD));
    }

    // construct color buffer
    for (int i = 0; i < g_rex.canvas.width * g_rex.canvas.height; ++i)
    {
        g_blt_bits[i] =
            ((unsigned char)(g_rex.canvas.pixels[i].r * 255) << 16) |
            ((unsigned char)(g_rex.canvas.pixels[i].g * 255) << 8) |
            ((unsigned char)(g_rex.canvas.pixels[i].b * 255) << 0);
    }

    // blit color buffer to window
    HDC hdc = GetDC(hwnd);
    assert(hdc && "failed to get device context");

    BITMAPINFO bitmap_info = {};
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = g_rex.canvas.width;
    bitmap_info.bmiHeader.biHeight = -g_rex.canvas.height; // negative to have origin at top left corner
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    int copied_scan_lines = ::StretchDIBits(
        hdc,
        0, 0, g_rex.window_width, g_rex.window_height, // destination
        0, 0, g_rex.canvas.width, g_rex.canvas.height, // source
        g_blt_bits,
        &bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY);
    assert(copied_scan_lines && "[bolt/window]: failed to copy pixels to window rect");

    [[maybe_unused]] bool res = ReleaseDC(hwnd, hdc);
    assert(res && "failed to release device context");
}

LRESULT CALLBACK
_wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        g_rex.quit = true;
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        g_rex.window_width = LOWORD(lParam);
        g_rex.window_height = HIWORD(lParam);
        g_api->loop(&g_rex);
        _paint(hwnd);
        break;
    };

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

inline static void
_hot_reload_rex_module()
{
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    bool res = GetFileAttributesEx(L"rex.dll", GetFileExInfoStandard, &data);
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

    bool copy_succeeded = CopyFile(L"rex.dll", L"rex_tmp.dll", false);

    rex_dll = LoadLibrary(L"rex_tmp.dll");
    assert(rex_dll && "failed to load rex.dll");

    Rex_Api* old_api = g_api;
    g_api = ((rex_api_proc)GetProcAddress(rex_dll, "rex_api"))();
    if (old_api == nullptr)
        g_api->init(&g_rex);
    g_api->reload(&g_rex);

    if (copy_succeeded)
        last_time = data.ftLastWriteTime;
}

int
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
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
    _hot_reload_rex_module();
    // initialize platform functions
    g_rex.alloc = _alloc;
    g_rex.free = _free;

    // window creation
    WNDCLASSEX wcx = {};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_OWNDC;
    wcx.lpfnWndProc = _wnd_proc;
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcx.hInstance = hInstance;
    wcx.lpszClassName = L"rex_window_class";
    ATOM class_atom = RegisterClassEx(&wcx);
    assert(class_atom != 0 && "RegisterClassExA failed");

    HWND hwnd = CreateWindowEx(
        0,
        MAKEINTATOM(class_atom),
        L"rex",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    // timing
    timeBeginPeriod(1);
    LARGE_INTEGER frequency = {};
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER prev_ticks;
    QueryPerformanceCounter(&prev_ticks);

    // game loop
    while (true)
    {
        _hot_reload_rex_module();

        // consume window events
        MSG msg = {};
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // check for quit
        if (g_rex.quit)
            break;

        // timing in milliseconds (target 30fps)
        LARGE_INTEGER ticks;
        QueryPerformanceCounter(&ticks);
        LONGLONG busy_ms = (ticks.QuadPart - prev_ticks.QuadPart) * 1000 / frequency.QuadPart;
        if (busy_ms < 33)
            Sleep((DWORD)(33 - busy_ms));

        QueryPerformanceCounter(&ticks);
        LONGLONG total_ms = (ticks.QuadPart - prev_ticks.QuadPart) * 1000 / frequency.QuadPart;
        prev_ticks = ticks;

        wsprintf(buffer, L"rex [total: %dms, busy: %dms, free: %dms]", total_ms, busy_ms, total_ms - busy_ms);
        SetWindowText(hwnd, buffer);

        // rex loop
        g_rex.dt = 1.0f / total_ms;
        g_api->loop(&g_rex);
        _paint(hwnd);
    }

    // free resources
    _free(g_blt_bits);
    DestroyWindow(hwnd);
    g_api->destory(&g_rex);
}