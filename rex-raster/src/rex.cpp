#include "rex-raster/exports.h"
#include "rex-raster/rex.h"

#include <rex-core/str.h>
#include <rex-core/defer.h>
#include <rex-core/path.h>
#include <rex-core/log.h>

#include <rex-math/math.h>
#include <rex-math/vec2.h>
#include <rex-math/vec3.h>
#include <rex-math/vec4.h>
#include <rex-math/mat4.h>

namespace rex::raster
{
	static constexpr math::Color_F32 color_palette[] = {
		{0.19f, 0.30f, 0.39f, 1.0f}, // dark blue
		{0.70f, 0.91f, 0.91f, 1.0f}, // light blue
		{0.56f, 0.73f, 0.67f, 1.0f}, // green
		{0.95f, 0.82f, 0.59f, 1.0f}, // yellow
		{0.93f, 0.54f, 0.46f, 1.0f}, // orange
	};

	inline static void
	init(Rex_Api* api)
	{
		auto self = (Rex*)api;

		self->canvas = canvas_init();
		self->mesh = mesh_cube();
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
		canvas_clear(canvas, {0.1f, 0.1f, 0.1f, 0.1f}, -300);

		auto M =
			math::mat4_rotation_x(-3.14f * 0.5f) *
			math::mat4_rotation_y(t) *
			math::mat4_translation(0.0f, 0.0f, -10.0f);

		auto P = math::mat4_perspective(30.0f, (float)canvas.width / (float)canvas.height, 0.1f, 300.0f);

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
			auto v1_c = v1 * P; v0_c /= v0_c.w;
			auto v2_c = v2 * P; v0_c /= v0_c.w;

			// TODO: use viewport matrix
			// auto v0_s = v0_c * viewport;
			// auto v1_s = v1_c * viewport;
			// auto v2_s = v2_c * viewport;

			v0_c.x = (v0_c.x + 1.0f) * canvas.width * 0.5f; v0_c.y = (v0_c.y + 1.0f) * canvas.height * 0.5f;
			v1_c.x = (v1_c.x + 1.0f) * canvas.width * 0.5f; v1_c.y = (v1_c.y + 1.0f) * canvas.height * 0.5f;
			v2_c.x = (v2_c.x + 1.0f) * canvas.width * 0.5f; v2_c.y = (v2_c.y + 1.0f) * canvas.height * 0.5f;

			math::V3 n0, n1, n2;
			if (mesh.normal.count)
			{
				// TODO: handle this propably
				n0 = (math::V4{ mesh.normal[i0].x, mesh.normal[i0].y, mesh.normal[i0].z, 0.0f } * M).xyz;
				n1 = (math::V4{ mesh.normal[i1].x, mesh.normal[i1].y, mesh.normal[i1].z, 0.0f } * M).xyz;
				n2 = (math::V4{ mesh.normal[i2].x, mesh.normal[i2].y, mesh.normal[i2].z, 0.0f } * M).xyz;
			}

			math::Color_F32 c0, c1, c2;
			if (mesh.color.count)
			{
				c0 = mesh.color[i0];
				c1 = mesh.color[i1];
				c2 = mesh.color[i2];
			}

			auto a = v1_c.xy - v0_c.xy;
			auto b = v2_c.xy - v1_c.xy;
			auto c = v0_c.xy - v2_c.xy;

			float area = -math::cross(a, b);

			// TODO: revisit back-face culling
			// if (area >= 0)
			//     continue;

			auto bb_min = math::min(math::min(v0_c.xy, v1_c.xy), v2_c.xy);
			auto bb_max = math::max(math::max(v0_c.xy, v1_c.xy), v2_c.xy);

			int min_x = (int)math::max(0.0f, bb_min.x);
			int max_x = (int)math::min((float)(canvas.width - 1), bb_max.x);

			int min_y = (int)math::max(0.0f, bb_min.y);
			int max_y = (int)math::min((float)(canvas.height - 1), bb_max.y);

			for (int y = min_y; y <= max_y; ++y)
			{
				for (int x = min_x; x <= max_x; ++x)
				{
					auto p = math::V2{x + 0.5f, y + 0.5f};

					float w0 = math::cross(p - v1_c.xy, b) / area;
					float w1 = math::cross(p - v2_c.xy, c) / area;
					float w2 = math::cross(p - v0_c.xy, a) / area;

					// perspective correction
					float z = 1.0f / (w0 * (1.0f / v0.z) + w1 * (1.0f / v1.z) + w2 * (1.0f / v2.z));
					w0 *= (z / v0.z);
					w1 *= (z / v1.z);
					w2 *= (z / v2.z);

					// rex_log_info("pass");
					if (w0 > 0 && w1 > 0 && w2 > 0)
					{
						float depth = w0 * v0.z + w1 * v1.z + w2 * v2.z;
						if (depth > canvas_depth(canvas, x, y))
						{
							math::Color_F32 color = (mesh.color.count ? w0 * c0 + w1 * c1 + w2 * c2 : color_palette[4]);

							if (mesh.normal.count)
							{
								math::V3 light_pos = {};
								math::Color_F32 light_color = {1.0f, 1.0f, 1.0f, 1.0f};

								// very basic phong lighting
								auto frag_pos = v0.xyz * w0 + v1.xyz * w1 + v2.xyz * w2;
								auto light_dir = math::normalize(light_pos - frag_pos);
								auto diff = math::max(math::dot(n0, light_dir), 0.0f);
								auto diffuse = diff * light_color;
								canvas_color(canvas, x, y) = diffuse * color;
							}
							else
							{
								canvas_color(canvas, x, y) = color;
							}
							canvas_depth(canvas, x, y) = depth;
						}
					}
				}
			}
		}

		// blit to screen
		for (int i = 0; i < self->screen_width * self->screen_height; ++i)
		{
			self->screen[i].r = (uint8_t)(canvas.color[i].r * 255);
			self->screen[i].g = (uint8_t)(canvas.color[i].g * 255);
			self->screen[i].b = (uint8_t)(canvas.color[i].b * 255);
			self->screen[i].a = (uint8_t)(canvas.color[i].a * 255);
		}

		// reverse animation
		if (t <= -3.14f)
			reverse = false;
		else if (t >= 3.14f)
			reverse = true;

		// update t
		t = reverse ? t - dt : t + dt;
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
