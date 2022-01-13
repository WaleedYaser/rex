#include "rex-raster/exports.h"
#include "rex-raster/rex.h"
#include "rex-raster/stb_image.h"

#include <rex-core/str.h>
#include <rex-core/defer.h>
#include <rex-core/path.h>

#include <rex-math/math.h>
#include <rex-math/vec2.h>
#include <rex-math/vec3.h>
#include <rex-math/vec4.h>
#include <rex-math/mat4.h>

#include <float.h>

namespace rex::raster
{
	static constexpr math::Color_F32 color_palette[] = {
		{0.19f, 0.30f, 0.39f, 1.0f}, // dark blue
		{0.70f, 0.91f, 0.91f, 1.0f}, // light blue
		{0.56f, 0.73f, 0.67f, 1.0f}, // green
		{0.95f, 0.82f, 0.59f, 1.0f}, // yellow
		{0.93f, 0.54f, 0.46f, 1.0f}, // orange
	};

	template <typename T>
	void
	_swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	// source: https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm#timings-fifth-and-final-attempt
	inline static void
	_raster_line(Rex* self, math::V2i p0, math::V2i p1, math::Color_F32 color)
	{
		bool steep = false;
		if (math::abs(p0.x - p1.x) < math::abs(p0.y - p1.y))
		{
			_swap(p0.x, p0.y);
			_swap(p1.x, p1.y);
			steep = true;
		}

		if (p0.x > p1.x)
			_swap(p0, p1);

		auto d = p1 - p0;
		int derror2 = math::abs(d.y) * 2;
		int error2 = 0;
		int y = p0.y;
		for (int x = p0.x; x <= p1.x; x++)
		{
			if (steep)
				canvas_color(self->canvas, y, x) = color;
			else
				canvas_color(self->canvas, x, y) = color;

			error2 += derror2;
			if (error2 > d.x)
			{
				y += (p1.y > p0.y ? 1 : -1);
				error2 -= d.x * 2;
			}
		}
	}

	inline static math::V3
	_barycentric(math::V2 p0, math::V2 p1, math::V2 p2, math::V2 p)
	{
		auto u = math::cross(
			math::V3{p2.x - p0.x, p1.x - p0.x, p0.x - p.x},
			math::V3{p2.y - p0.y, p1.y - p0.y, p0.y - p.y}
		);

		if (math::abs(u.z) <= 1e-2)
			return {-1.0f, 1.0f, 1.0f};

		return {1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
	}

	inline static void
	_raster_triangle(Rex* self, math::V3 p0, math::V3 p1, math::V3 p2, math::V2 uv0, math::V2 uv1, math::V2 uv2, float intensity)
	{
	#define _LINE_SWEEPING 0
	#if _LINE_SWEEPING
		// skip degenerate triangles
		if (p0.y == p1.y && p0.y == p2.y) return;

		// sort the vertices, p0, p1, p2 lower−to−upper
		if (p0.y > p1.y) _swap(p0, p1);
		if (p0.y > p2.y) _swap(p0, p2);
		if (p1.y > p2.y) _swap(p1, p2);

		for (float y = p0.y; y <= p2.y; ++y)
		{
			// top half or bottom half
			auto bottom_half = y > p1.y || p0.y == p1.y;
			auto _p0 = bottom_half ? p1 : p0;
			auto _p1 = bottom_half ? p2 : p1;

			// short line
			auto t0 = (float)(y - _p0.y) / (_p1.y - _p0.y);
			auto a = _p0 + (_p1 - _p0) * t0;

			// long line
			auto t1 = (float)(y - p0.y) / (p2.y - p0.y);
			auto b = p0 + (p2 - p0) * t1;

			if (a.x > b.x) _swap(a, b);
			for (float x = a.x; x <= b.x; ++x)
				canvas_color(self->canvas, (int)x, (int)y) = color;
		}
	#else
		auto bb_min = math::max(math::min(math::min(p0.xy, p1.xy), p2.xy), math::V2{0, 0});
		auto bb_max = math::min(math::max(math::max(p0.xy, p1.xy), p2.xy), math::V2{(float)self->canvas.width - 1, (float)self->canvas.height - 1});

		// round
		bb_min = {(float)(int)bb_min.x, (float)(int)bb_min.y};
		bb_max = {(float)(int)bb_max.x, (float)(int)bb_max.y};

		math::V2 p = {};
		for (p.y = bb_min.y; p.y <= bb_max.y; ++p.y)
		{
			for (p.x = bb_min.x; p.x <= bb_max.x; ++p.x)
			{
				auto w = _barycentric(p0.xy, p1.xy, p2.xy, p);
				if (w.x < 0 || w.y < 0 || w.z < 0)
					continue;

				auto z = w[0] * p0.z + w[1] * p1.z + w[2] * p2.z;
				if (canvas_depth(self->canvas, (int)p.x, (int)p.y) > z)
				{
					auto uv = w[0] * uv0 + w[1] * uv1 + w[2] * uv2;
					auto color = canvas_color(self->texture, (int)(uv.x * self->texture.width), (int)((1.0f - uv.y) * self->texture.height));
					canvas_color(self->canvas, (int)p.x, (int)p.y) = color * intensity;
					canvas_depth(self->canvas, (int)p.x, (int)p.y) = z;
				}
			}
		}
	#endif
	}

	inline static void
	init(Rex_Api* api)
	{
		auto self = (Rex*)api;

		self->canvas = canvas_init();
		self->cam = camera_init();

		self->mesh = mesh_from_obj(rc::str_fmt(rc::frame_allocator(), "%s/data/african_head/african_head.obj", rc::app_directory()).ptr);
		self->texture = canvas_init();
		{
			int width, height, channels = 0;
			auto data = stbi_load(rc::str_fmt(rc::frame_allocator(), "%s/data/african_head/african_head_diffuse.tga", rc::app_directory()).ptr, &width, &height, &channels, 4);
			canvas_resize(self->texture, width, height);
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					canvas_color(self->texture, x, y).r = data[(x + y * width) * 4 + 0] / 255.0f;
					canvas_color(self->texture, x, y).g = data[(x + y * width) * 4 + 1] / 255.0f;
					canvas_color(self->texture, x, y).b = data[(x + y * width) * 4 + 2] / 255.0f;
					canvas_color(self->texture, x, y).a = data[(x + y * width) * 4 + 3] / 255.0f;
				}
			}
		}
	}

	inline static void
	deinit(Rex_Api* api)
	{
		auto self = (Rex*)api;

		canvas_deinit(self->texture);
		canvas_deinit(self->canvas);
		mesh_deinit(self->mesh);
	}

	inline static void
	loop(Rex_Api* api)
	{
		auto self = (Rex*)api;
		auto& canvas = self->canvas;
		auto& mesh = self->mesh;
		auto dt = self->dt;

		// animation parameters
		static float t = 0;

		canvas_resize(canvas, self->screen_width, self->screen_height);
		canvas_clear(canvas, {0.1f, 0.1f, 0.1f, 1.0f}, 1.0f);

		camera_viewport_update(self->cam, canvas.width, canvas.height);
		camera_input_update(self->cam, self->input, self->dt);
		self->cam.kind = Camera::PROJECTION_PERSPECTIVE;

		float max_length = math::length(mesh.bb_max - mesh.bb_min);
		float distance_h = max_length / math::tan(self->cam.fovy / 2);
		float distance_w = max_length / math::tan(self->cam.fovy * self->cam.aspect / 2);
		float distance   = math::max(distance_w, distance_h);

		self->cam.distance = distance;

		auto M = math::mat4_translation(-mesh.bb_min - (mesh.bb_max - mesh.bb_min) * 0.5f);
		auto V = camera_view_mat(self->cam);
		auto P = camera_proj_mat(self->cam);
		auto viewport = math::mat4_viewport<float>(0, 0, (float)canvas.width, (float)canvas.height);

		auto count = (mesh.indices.count ? mesh.indices.count : mesh.position.count);
		for (rc::sz i = 0; i < count; i += 3)
		{
			rc::sz i0, i1, i2;
			if (mesh.indices.count)
			{
				i0 = mesh.indices[i+0];
				i1 = mesh.indices[i+1];
				i2 = mesh.indices[i+2];
			}
			else
			{
				i0 = i+0;
				i1 = i+1;
				i2 = i+2;
			}

			auto p0 = mesh.position[i0];
			auto p1 = mesh.position[i1];
			auto p2 = mesh.position[i2];

			auto v0 = math::V4{p0.x, p0.y, p0.z, 1.0f} * M * V * P;
			auto v1 = math::V4{p1.x, p1.y, p1.z, 1.0f} * M * V * P;
			auto v2 = math::V4{p2.x, p2.y, p2.z, 1.0f} * M * V * P;

			v0 /= v0.w;
			v1 /= v1.w;
			v2 /= v2.w;

			v0 *= viewport;
			v1 *= viewport;
			v2 *= viewport;

	#define WIREFRAME 0
	#if WIREFRAME
			_raster_line(self, {(int)v0_c.x, (int)v0_c.y}, {(int)v1_c.x, (int)v1_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});
			_raster_line(self, {(int)v1_c.x, (int)v1_c.y}, {(int)v2_c.x, (int)v2_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});
			_raster_line(self, {(int)v2_c.x, (int)v2_c.y}, {(int)v0_c.x, (int)v0_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});

	#else
			math::V3 n0, n1, n2;
			{
				n0 = -math::cross(v2.xyz - v0.xyz, v1.xyz - v0.xyz);
				n1 = n0; n2 = n1;
			}
			auto light_dir = math::V3{0.0f, 0.0f, -1.0f};
			auto intensity = math::dot(math::normalize(n0), light_dir);
			intensity = math::min(math::max(intensity, 0.0f), 1.0f);

			auto uv0 = mesh.uv[mesh.uv_indices[i+0]];
			auto uv1 = mesh.uv[mesh.uv_indices[i+1]];
			auto uv2 = mesh.uv[mesh.uv_indices[i+2]];

			_raster_triangle(self, v0.xyz, v1.xyz, v2.xyz,
				uv0, uv1, uv2,
				intensity
			);
	#endif
		}

		// blit to screen
		for (int i = 0; i < self->screen_width * self->screen_height; ++i)
		{
			self->screen[i].r = (uint8_t)(canvas.color[i].r * 255);
			self->screen[i].g = (uint8_t)(canvas.color[i].g * 255);
			self->screen[i].b = (uint8_t)(canvas.color[i].b * 255);
			self->screen[i].a = 255;
		}

		// update t
		t += dt;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

REX_RASTER_EXPORT Rex_Api*
rex_api(Rex_Api* api, bool reload)
{
	if (api == nullptr)
	{
		auto self = rex_alloc_zeroed_T(rex::raster::Rex);

		self->init = rex::raster::init;
		self->deinit = rex::raster::deinit;
		self->loop = rex::raster::loop;

		return self;
	}
	else if (reload)
	{
		api->init = rex::raster::init;
		api->deinit = rex::raster::deinit;
		api->loop = rex::raster::loop;

		return api;
	}
	else
	{
		rex_dealloc(api);
		return nullptr;
	}
}

#ifdef __cplusplus
}
#endif
