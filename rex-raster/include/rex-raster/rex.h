#pragma once

#include <rex-core/api.h>

struct Pixel { float r, g, b, a; };

inline static Pixel operator*(Pixel p, float f) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator*(float f, Pixel p) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator+(Pixel p1, Pixel p2) { return {p1.r + p2.r, p1.g + p2.g, p1.b + p2.b, p1.a + p2.a}; }

typedef struct Image
{
	int width, height;
	Pixel* pixels;
} Image;

struct Rex: Rex_Api
{
	// owned values
	Image canvas;
	float* depth_buffer;
	Image image;

	unsigned int vertices_count;
	struct Vec3* vertices;
	struct Vec3* normals;
	struct Vec2* uvs;

	unsigned int indices_count;
	unsigned int* indices;

	float camera_z;
};
