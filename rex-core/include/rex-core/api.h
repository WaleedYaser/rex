#pragma once

#include "rex-core/exports.h"
#include <stdint.h>

typedef union Rex_Pixel
{
	struct { uint8_t b, g, r, a; };
	uint32_t raw;
} Rex_Pixel;

typedef struct Rex_Api
{
	Rex_Pixel* screen;
	int32_t screen_width, screen_height;
	float dt;
	bool quit;

	void (*init)(Rex_Api* self);
	void (*deinit)(Rex_Api* self);
	void (*loop)(Rex_Api* self);
} Rex_Api;

typedef Rex_Api* (*rex_api_proc)(Rex_Api*, bool);

REX_CORE_EXPORT Rex_Api* load_rex_api();
