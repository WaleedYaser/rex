#pragma once

#include "rex-core/exports.h"

#include <stdbool.h>

// TODO: cleanup

typedef struct Pixel
{
	float r, g, b, a;
} Pixel;

typedef struct Image
{
	int width, height;
	Pixel* pixels;
} Image;

typedef struct Content
{
	unsigned char* data;
	unsigned int size;
} Content;

struct Vec2;
struct Vec3;

typedef struct Rex_Api
{
	// owned values
	Image canvas;
	float* depth_buffer;
	Image image;

	unsigned int vertices_count;
	Vec3* vertices;
	Vec3* normals;
	Vec2* uvs;

	unsigned int indices_count;
	unsigned int* indices;

	float camera_z;

	// values set from client
	bool quit;
	int window_width, window_height;
	float dt;

	// platform functions
	Content (*file_read)(const char* path);

	void (*init)(Rex_Api* self);
	void (*deinit)(Rex_Api* self);
	void (*loop)(Rex_Api* self);
} Rex_Api;

typedef Rex_Api* (*rex_api_proc)(Rex_Api*, bool);

REX_CORE_EXPORT Rex_Api* load_rex_api();
