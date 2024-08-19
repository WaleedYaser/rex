#include <rex-gfx/gfx.h>
#include <rex-core/window.h>

bool quit = false;

inline static void
_rex_event(rc::Window* window, rc::Event event)
{
	switch (event.type)
	{
		case rc::EVENT_TYPE_WINDOW_CLOSE:
		{
			quit = true;
			break;
		}
		case rc::EVENT_TYPE_WINDOW_RESIZE:
		{
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
	rc::Window* window = rc::window_init("scratch", 800, 600, nullptr, _rex_event);

	Rex_Gfx* gfx = rex_gfx_init();
	Rex_Gfx_Command_Queue* command_queue = rex_gfx_command_queue_init(gfx);
	Rex_Gfx_Command_List* command_list = rex_gfx_command_list_init(gfx);
	Rex_Gfx_Swapchain* swapchain = rex_gfx_swapchain_init(gfx, command_queue, window->native_handle);

	while (quit == false)
	{
		rc::window_poll(window);
	}

	rex_gfx_swapchain_deinit(swapchain);
	rex_gfx_command_list_deinit(command_list);
	rex_gfx_command_queue_deinit(command_queue);
	rex_gfx_deinit(gfx);

	rc::window_deinit(window);
	// rex_log_trace("%s", "rex");
	// rex_log_debug("%s", "rex");
	// rex_log_info("%s", "rex");
	// rex_log_warn("%s", "rex");
	// rex_log_error("%s", "rex");
	// rex_log_fatal("%s", "rex");
	// printf("hello, world!\n");
}
