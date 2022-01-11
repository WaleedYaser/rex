#include "rex-raster/exports.h"
#include "rex-raster/rex.h"

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
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
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
	_barycentric(math::V2i p0, math::V2i p1, math::V2i p2, math::V2i p)
	{
		auto u = math::cross(
			math::V3{(float)p1.x - p0.x, (float)p2.x - p0.x, (float)p0.x - p.x},
			math::V3{(float)p1.y - p0.y, (float)p2.y - p0.y, (float)p0.y - p.y}
		);

		if (math::abs(u.z) < 1)
			return {-1.0f, 1.0f, 1.0f};

		return {1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
	}

	inline static void
	_raster_triangle(Rex* self, math::V2i p0, math::V2i p1, math::V2i p2, math::Color_F32 color)
	{
	#define _LINE_SWEEPING 0
	#if _LINE_SWEEPING
		// skip degenerate triangles
		if (p0.y == p1.y && p0.y == p2.y) return;

		// sort the vertices, p0, p1, p2 lower−to−upper
		if (p0.y > p1.y) _swap(p0, p1);
		if (p0.y > p2.y) _swap(p0, p2);
		if (p1.y > p2.y) _swap(p1, p2);

		for (int y = p0.y; y <= p2.y; ++y)
		{
			// top half or bottom half
			auto bottom_half = y > p1.y || p0.y == p1.y;
			auto _p0 = bottom_half ? p1 : p0;
			auto _p1 = bottom_half ? p2 : p1;

			// short line
			auto t0 = (float)(y - _p0.y) / (_p1.y - _p0.y);
			auto a = _p0 + math::V2i{(int)((_p1.x - _p0.x) * t0), (int)((_p1.y - _p0.y) * t0)};

			// long line
			auto t1 = (float)(y - p0.y) / (p2.y - p0.y);
			auto b = p0 + math::V2i{(int)((p2.x - p0.x) * t1), (int)((p2.y - p0.y) * t1)};

			if (a.x > b.x) _swap(a, b);
			for (int x = a.x; x <= b.x; ++x)
				canvas_color(self->canvas, x, y) = color;
		}
	#else
		auto bb_min = math::max(math::min(math::min(p0, p1), p2), math::V2i{0, 0});
		auto bb_max = math::min(math::max(math::max(p0, p1), p2), math::V2i{self->canvas.width - 1, self->canvas.height - 1});

		math::V2i p = {};
		for (p.y = bb_min.y; p.y <= bb_max.y; ++p.y)
		{
			for (p.x = bb_min.x; p.x <= bb_max.x; ++p.x)
			{
				auto w = _barycentric(p0, p1, p2, p);
				if (w.x < 0 || w.y < 0 || w.z < 0)
					continue;

				canvas_color(self->canvas, p.x, p.y) = color;
			}
		}
	#endif
	}

	inline static void
	init(Rex_Api* api)
	{
		auto self = (Rex*)api;

		self->canvas = canvas_init();

		self->mesh = mesh_from_stl(rc::str_fmt(rc::frame_allocator(), "%s/data/dino.stl", rc::app_directory()).ptr);
	}

	inline static void
	deinit(Rex_Api* api)
	{
		auto self = (Rex*)api;

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
		static bool reverse = false;
		static float t = 0;

		canvas_resize(canvas, self->screen_width, self->screen_height);
		canvas_clear(canvas, {0.1f, 0.1f, 0.1f, 1.0f}, -FLT_MAX);

		float fov = (float)(30.0 * math::TO_RADIAN);
		float aspect = (float)canvas.width / (float)canvas.height;

		float max_length = math::length(mesh.bb_max - mesh.bb_min);
		float distance_h = max_length / math::tan(fov / 2);
		float distance_w = max_length / math::tan(fov * aspect / 2);
		float distance   = math::max(distance_w, distance_h);

		auto M =
			math::mat4_translation(-mesh.bb_min - (mesh.bb_max - mesh.bb_min) * 0.5f) *
			math::mat4_euler((float)math::PI_DIV_2, 0.0f, 0.0f) *
			math::mat4_euler(0.0f, t, 0.0f) *
			math::mat4_translation(0.0f, 0.0f, -distance);

		auto P = math::mat4_perspective(fov, (float)canvas.width / (float)canvas.height, 0.1f, distance + max_length);

		auto count = (mesh.indices.count ? mesh.indices.count : mesh.position.count);
		for (int i = 0; i < count; i += 3)
		{
			int i0, i1, i2;
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

			auto v0 = math::V4{p0.x, p0.y, p0.z, 1.0f} * M;
			auto v1 = math::V4{p1.x, p1.y, p1.z, 1.0f} * M;
			auto v2 = math::V4{p2.x, p2.y, p2.z, 1.0f} * M;

			auto v0_c = v0 * P; v0_c /= v0_c.w;
			auto v1_c = v1 * P; v1_c /= v1_c.w;
			auto v2_c = v2 * P; v2_c /= v2_c.w;

			// TODO: use viewport matrix
			// auto v0_s = v0_c * viewport;
			// auto v1_s = v1_c * viewport;
			// auto v2_s = v2_c * viewport;

			v0_c.x = (v0_c.x + 1.0f) * canvas.width * 0.5f; v0_c.y = (v0_c.y + 1.0f) * canvas.height * 0.5f;
			v1_c.x = (v1_c.x + 1.0f) * canvas.width * 0.5f; v1_c.y = (v1_c.y + 1.0f) * canvas.height * 0.5f;
			v2_c.x = (v2_c.x + 1.0f) * canvas.width * 0.5f; v2_c.y = (v2_c.y + 1.0f) * canvas.height * 0.5f;

	#define WIREFRAME 0
	#if WIREFRAME
			_raster_line(self, {(int)v0_c.x, (int)v0_c.y}, {(int)v1_c.x, (int)v1_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});
			_raster_line(self, {(int)v1_c.x, (int)v1_c.y}, {(int)v2_c.x, (int)v2_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});
			_raster_line(self, {(int)v2_c.x, (int)v2_c.y}, {(int)v0_c.x, (int)v0_c.y}, {1.0f, 1.0f, 1.0f, 1.0f});

	#else
			math::V3 n0, n1, n2;
			if (mesh.normal.count)
			{
				// TODO: handle this propably
				n0 = (math::V4{ mesh.normal[i0].x, mesh.normal[i0].y, mesh.normal[i0].z, 0.0f } * M).xyz;
				n1 = (math::V4{ mesh.normal[i1].x, mesh.normal[i1].y, mesh.normal[i1].z, 0.0f } * M).xyz;
				n2 = (math::V4{ mesh.normal[i2].x, mesh.normal[i2].y, mesh.normal[i2].z, 0.0f } * M).xyz;
			}
			else
			{
				n0 = math::cross(v2.xyz - v0.xyz, v1.xyz - v1.xyz);
				n1 = n0; n2 = n1;
			}
			auto light_dir = math::V3{0.0f, 0.0f, -1.0f};
			auto intensity = math::dot(math::normalize(n0), light_dir);

			_raster_triangle(self,
				{(int)v0_c.x, (int)v0_c.y},
				{(int)v1_c.x, (int)v1_c.y},
				{(int)v2_c.x, (int)v2_c.y},
				{intensity, 0.0f, 0.0f, 1.0f}
			);

			// math::Color_F32 c0, c1, c2;
			// if (mesh.color.count)
			// {
			// 	c0 = mesh.color[i0];
			// 	c1 = mesh.color[i1];
			// 	c2 = mesh.color[i2];
			// }

			// auto a = v1_c.xy - v0_c.xy;
			// auto b = v2_c.xy - v1_c.xy;
			// auto c = v0_c.xy - v2_c.xy;

			// float area = -math::cross(a, b);

			// // TODO: back-face culling

			// auto bb_min = math::min(math::min(v0_c.xy, v1_c.xy), v2_c.xy);
			// auto bb_max = math::max(math::max(v0_c.xy, v1_c.xy), v2_c.xy);

			// int min_x = (int)math::max(0.0f, bb_min.x);
			// int max_x = (int)math::min((float)(canvas.width - 1), bb_max.x);

			// int min_y = (int)math::max(0.0f, bb_min.y);
			// int max_y = (int)math::min((float)(canvas.height - 1), bb_max.y);

			// for (int y = min_y; y <= max_y; ++y)
			// {
			// 	for (int x = min_x; x <= max_x; ++x)
			// 	{
			// 		auto p = math::V2{x + 0.5f, y + 0.5f};

			// 		float w0 = math::cross(p - v1_c.xy, b) / area;
			// 		float w1 = math::cross(p - v2_c.xy, c) / area;
			// 		float w2 = math::cross(p - v0_c.xy, a) / area;

			// 		// perspective correction
			// 		float z = 1.0f / (w0 * (1.0f / v0.z) + w1 * (1.0f / v1.z) + w2 * (1.0f / v2.z));
			// 		w0 *= (z / v0.z);
			// 		w1 *= (z / v1.z);
			// 		w2 *= (z / v2.z);

			// 		if (w0 > 0 && w1 > 0 && w2 > 0)
			// 		{
			// 			float depth = w0 * v0.z + w1 * v1.z + w2 * v2.z;
			// 			if (depth > canvas_depth(canvas, x, y))
			// 			{
			// 				math::Color_F32 color = (mesh.color.count ? w0 * c0 + w1 * c1 + w2 * c2 : color_palette[4]);

			// 				if (mesh.normal.count)
			// 				{
			// 					math::V3 light_pos = {};
			// 					math::Color_F32 light_color = {1.0f, 1.0f, 1.0f, 1.0f};

			// 					// very basic phong lighting
			// 					auto frag_pos = v0.xyz * w0 + v1.xyz * w1 + v2.xyz * w2;
			// 					auto light_dir = math::normalize(light_pos - frag_pos);
			// 					auto diff = math::max(math::dot(n0, light_dir), 0.0f);
			// 					auto diffuse = diff * light_color.rgb;

			// 					canvas_color(canvas, x, y).rgb = math::min(diffuse * color.rgb, {1.0f, 1.0f, 1.0f});
			// 					canvas_color(canvas, x, y).a   = color.a;
			// 				}
			// 				else
			// 				{
			// 					canvas_color(canvas, x, y) = color;
			// 				}
			// 				canvas_depth(canvas, x, y) = depth;
			// 			}
			// 		}
			// 	}
			// }
	#endif
		}

		// blit to screen
		for (int i = 0; i < self->screen_width * self->screen_height; ++i)
		{
			self->screen[i].r = (uint8_t)(canvas.color[i].r * 255);
			self->screen[i].g = (uint8_t)(canvas.color[i].g * 255);
			self->screen[i].b = (uint8_t)(canvas.color[i].b * 255);
			self->screen[i].a = (uint8_t)(canvas.color[i].a * 255);
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
