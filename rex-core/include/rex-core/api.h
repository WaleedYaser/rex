#pragma once

#include "rex-core/exports.h"
#include <stdint.h>

typedef union Rex_Pixel
{
// TODO: handle pixel format in a better in better way
#if REX_OS_WASM
	struct { uint8_t r, g, b, a; };
#else
	struct { uint8_t b, g, r, a; };
#endif
	uint32_t raw;
} Rex_Pixel;

typedef enum REX_KEY
{
	REX_KEY_NONE,
	// numbers
	REX_KEY_0,
	REX_KEY_1,
	REX_KEY_2,
	REX_KEY_3,
	REX_KEY_4,
	REX_KEY_5,
	REX_KEY_6,
	REX_KEY_7,
	REX_KEY_8,
	REX_KEY_9,
	// letters
	REX_KEY_A,
	REX_KEY_B,
	REX_KEY_C,
	REX_KEY_D,
	REX_KEY_E,
	REX_KEY_F,
	REX_KEY_G,
	REX_KEY_H,
	REX_KEY_I,
	REX_KEY_J,
	REX_KEY_K,
	REX_KEY_L,
	REX_KEY_M,
	REX_KEY_N,
	REX_KEY_O,
	REX_KEY_P,
	REX_KEY_Q,
	REX_KEY_R,
	REX_KEY_S,
	REX_KEY_T,
	REX_KEY_U,
	REX_KEY_V,
	REX_KEY_W,
	REX_KEY_X,
	REX_KEY_Y,
	REX_KEY_Z,
	// special keys
	REX_KEY_SPACE,
	REX_KEY_ENTER,
	REX_KEY_ESCAPE,
	REX_KEY_ARROW_UP,
	REX_KEY_ARROW_DOWN,
	REX_KEY_ARROW_LEFT,
	REX_KEY_ARROW_RIGHT,
	// mouse
	REX_KEY_MOUSE_LEFT,
	REX_KEY_MOUSE_MIDDLE,
	REX_KEY_MOUSE_RIGHT,

	REX_KEY_COUNT,
} REX_KEY;

typedef struct Rex_Key_State
{
	bool pressed;  // Once we press.
	bool released; // Once we release.
	bool down;

	int press_count;
	int release_count;
} Rex_Key_State;

typedef struct Rex_Input
{
	int mouse_x, mouse_y;
	int mouse_dx, mouse_dy;
	float mouse_wheel;
	Rex_Key_State keys[REX_KEY_COUNT];
} Rex_Input;

typedef struct Rex_Api
{
	Rex_Pixel* screen;
	int32_t screen_width, screen_height;
	float dt;
	bool quit;
	Rex_Input input;

	void (*init)(Rex_Api* self);
	void (*deinit)(Rex_Api* self);
	void (*loop)(Rex_Api* self);
} Rex_Api;

typedef Rex_Api* (*rex_api_proc)(Rex_Api*, bool);

REX_CORE_EXPORT Rex_Api* load_rex_api();
