#pragma once

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

typedef struct Rex
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
	int quit;
	int window_width, window_height;
	float dt;

	// platform functions
	void* (*alloc)(size_t size);
	void  (*free)(void *ptr);
	Content (*file_read)(const char* path);
} Rex;

typedef struct Rex_Api
{
	void (*init)(Rex* self);
	void (*destory)(Rex* self);

	void (*reload)(Rex* self);
	void (*loop)(Rex* self);
} Rex_Api;

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) Rex_Api*
rex_api();

#ifdef __cplusplus
}
#endif

typedef Rex_Api* (*rex_api_proc)(void);
