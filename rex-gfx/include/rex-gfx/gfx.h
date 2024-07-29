#pragma once

#include "rex-gfx/exports.h"

typedef struct Rex_Gfx Rex_Gfx;

REX_GFX_EXPORT Rex_Gfx*
rex_gfx_init();

REX_GFX_EXPORT void
rex_gfx_deinit(Rex_Gfx* self);