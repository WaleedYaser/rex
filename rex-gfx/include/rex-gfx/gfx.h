#pragma once

#include "rex-gfx/exports.h"

typedef struct Rex_Gfx Rex_Gfx;
typedef struct Rex_Gfx_Command_Queue Rex_Gfx_Command_Queue;
typedef struct Rex_Gfx_Command_List Rex_Gfx_Command_List;
typedef struct Rex_Gfx_Swapchain Rex_Gfx_Swapchain;

REX_GFX_EXPORT Rex_Gfx*
rex_gfx_init();

REX_GFX_EXPORT void
rex_gfx_deinit(Rex_Gfx* self);

REX_GFX_EXPORT Rex_Gfx_Command_Queue*
rex_gfx_command_queue_init(Rex_Gfx* self);

REX_GFX_EXPORT void
rex_gfx_command_queue_deinit(Rex_Gfx_Command_Queue* gfx);

REX_GFX_EXPORT Rex_Gfx_Command_List*
rex_gfx_command_list_init(Rex_Gfx* gfx);

REX_GFX_EXPORT void
rex_gfx_command_list_deinit(Rex_Gfx_Command_List* self);

REX_GFX_EXPORT Rex_Gfx_Swapchain*
rex_gfx_swapchain_init(Rex_Gfx* gfx, Rex_Gfx_Command_Queue* command_queue, void* window_native_handle);

REX_GFX_EXPORT void
rex_gfx_swapchain_deinit(Rex_Gfx_Swapchain* self);
