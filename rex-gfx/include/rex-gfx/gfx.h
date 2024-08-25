#pragma once

#include "rex-gfx/exports.h"

typedef struct Rex_Gfx Rex_Gfx;
typedef struct Rex_Gfx_Command_Queue Rex_Gfx_Command_Queue;
typedef struct Rex_Gfx_Command_List Rex_Gfx_Command_List;
typedef struct Rex_Gfx_Swapchain Rex_Gfx_Swapchain;
typedef struct Rex_Gfx_Texture Rex_Gfx_Texture;

typedef struct Rex_Gfx_Texture_Desc
{
    int width;
    int height;
} Rex_Gfx_Texture_Desc;

typedef struct Rex_Gfx_Viewport
{
    float width;
    float height;
} Rex_Gfx_Viewport;

REX_GFX_EXPORT Rex_Gfx*
rex_gfx_init();

REX_GFX_EXPORT void
rex_gfx_deinit(Rex_Gfx* self);

REX_GFX_EXPORT Rex_Gfx_Command_Queue*
rex_gfx_command_queue_init(Rex_Gfx* self);

REX_GFX_EXPORT void
rex_gfx_command_queue_deinit(Rex_Gfx_Command_Queue* self);

REX_GFX_EXPORT void
rex_gfx_command_queue_execute(Rex_Gfx_Command_Queue* self, Rex_Gfx_Command_List* command_list);

REX_GFX_EXPORT void
rex_gfx_command_queue_flush(Rex_Gfx_Command_Queue* self);

REX_GFX_EXPORT Rex_Gfx_Command_List*
rex_gfx_command_list_init(Rex_Gfx* gfx);

REX_GFX_EXPORT void
rex_gfx_command_list_deinit(Rex_Gfx_Command_List* self);

REX_GFX_EXPORT void
rex_gfx_command_list_begin(Rex_Gfx_Command_List* self, Rex_Gfx_Swapchain* swapchain);

REX_GFX_EXPORT void
rex_gfx_command_list_set_viewport(Rex_Gfx_Command_List* self, const Rex_Gfx_Viewport viewport);

REX_GFX_EXPORT Rex_Gfx_Swapchain*
rex_gfx_swapchain_init(Rex_Gfx* gfx, Rex_Gfx_Command_Queue* command_queue, void* window_native_handle);

REX_GFX_EXPORT void
rex_gfx_swapchain_deinit(Rex_Gfx_Swapchain* self);

REX_GFX_EXPORT void
rex_gfx_swapchain_resize(Rex_Gfx* gfx, Rex_Gfx_Swapchain* self, unsigned width, unsigned height);

REX_GFX_EXPORT void
rex_gfx_swapchain_present(Rex_Gfx_Swapchain* self);

REX_GFX_EXPORT Rex_Gfx_Texture*
rex_gfx_texture_init(Rex_Gfx* gfx, const Rex_Gfx_Texture_Desc texture_desc);

REX_GFX_EXPORT void
rex_gfx_texture_deinit(Rex_Gfx_Texture* self);
