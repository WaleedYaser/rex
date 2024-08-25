#include <rex-gfx/gfx.h>
#include <rex-core/window.h>
#include <rex-core/memory.h>
#include <rex-core/time.h>
#include <rex-core/str.h>

struct App
{
	rc::Window* window;
	bool quit;
	Rex_Gfx* gfx;
	Rex_Gfx_Command_Queue* command_queue;
	Rex_Gfx_Command_List* command_list;
	Rex_Gfx_Swapchain* swapchain;
	Rex_Gfx_Texture* depth_stencil_texture;
};

inline static void
app_event(rc::Window* window, rc::Event event);

inline static App&
app_init()
{
	static App self = {};

	self.window = rc::window_init("scratch", 800, 600, &self, app_event);

	self.gfx = rex_gfx_init();
	self.command_queue = rex_gfx_command_queue_init(self.gfx);
	self.swapchain = rex_gfx_swapchain_init(self.gfx, self.command_queue, self.window->native_handle);
	self.command_list = rex_gfx_command_list_init(self.gfx);

	Rex_Gfx_Texture_Desc tex_desc = {};
	tex_desc.width = self.window->width;
	tex_desc.height = self.window->height;
	self.depth_stencil_texture = rex_gfx_texture_init(self.gfx, tex_desc);

	return self;
}

inline static void
app_deinit(App& self)
{
	rex_gfx_command_queue_flush(self.command_queue);

	rex_gfx_texture_deinit(self.depth_stencil_texture);
	rex_gfx_swapchain_deinit(self.swapchain);
	rex_gfx_command_list_deinit(self.command_list);
	rex_gfx_command_queue_deinit(self.command_queue);
	rex_gfx_deinit(self.gfx);
	rc::window_deinit(self.window);
}

inline static bool
app_frame(App& self)
{
	if (self.quit)
		return false;

	rc::window_poll(self.window);

	auto busy_ms = rc::time_milliseconds();
	if (busy_ms < 33)
		rc::sleep((rc::u32)(33 - busy_ms));

	auto free_ms = rc::time_milliseconds();
	auto frame_ms = busy_ms + free_ms;

	auto title = rc::str_fmt(rc::frame_allocator(), "Rex [frame: %lldms, busy: %lldms, free: %lldms]", frame_ms, busy_ms, free_ms);
	rc::window_title_set(self.window, title.ptr);

	rex_gfx_command_list_begin(self.command_list, self.swapchain);
	rex_gfx_command_queue_execute(self.command_queue, self.command_list);
	rex_gfx_swapchain_present(self.swapchain);
	rex_gfx_command_queue_flush(self.command_queue);

	return true;
}

inline static void
app_event(rc::Window* window, rc::Event event)
{
	App* app = (App*)window->user_data;

	switch (event.type)
	{
		case rc::EVENT_TYPE_WINDOW_CLOSE:
		{
			app->quit = true;
			break;
		}
		case rc::EVENT_TYPE_WINDOW_RESIZE:
		{
			rex_gfx_command_queue_flush(app->command_queue);
			rex_gfx_swapchain_resize(app->gfx, app->swapchain, event.window_resize.width, event.window_resize.height);

			rex_gfx_texture_deinit(app->depth_stencil_texture);
			Rex_Gfx_Texture_Desc tex_desc = {};
			tex_desc.width = event.window_resize.width;
			tex_desc.height = event.window_resize.height;
			app->depth_stencil_texture = rex_gfx_texture_init(app->gfx, tex_desc);
			break;
		}
		case rc::EVENT_TYPE_MOUSE_BUTTON_PRESS:
		{
			break;
		}
		case rc::EVENT_TYPE_MOUSE_BUTTON_RELEASE:
		{
			break;
		}
		case rc::EVENT_TYPE_MOUSE_MOVE:
			break;
		case rc::EVENT_TYPE_MOUSE_WHEEL_SCROLL_DOWN:
			break;
		case rc::EVENT_TYPE_MOUSE_WHEEL_SCROLL_UP:
			break;
		case rc::EVENT_TYPE_KEY_PRESS:
		{
			break;
		}
		case rc::EVENT_TYPE_KEY_RELEASE:
		{
			break;
		}
		default:
			// do nothing
			break;
	}
}

int main()
{
	// TODO: make sure memory allocators initialized first
	rc::rex_allocator();

	App& app = app_init();

	// init timing
	rc::time_milliseconds();

	while (app_frame(app));

	app_deinit(app);
}
