#pragma once

#include <rex-core/api.h>
#include <rex-math/vec3.h>
#include <rex-math/mat3.h>
#include <rex-math/mat4.h>
#include <rex-core/window.h>

namespace rex
{
	struct Camera
	{
		enum PROJECTION
		{
			PROJECTION_ORTHOGRAPHIC,
			PROJECTION_PERSPECTIVE
		} kind;

		math::V3 position;
		math::V3 rotation;
		float scale;

		float distance;
		float near, far;
		float aspect;

		float fovy;
		float ortho_size;

		int width, height;
	};

	inline static Camera
	camera_init()
	{
		Camera self = {};

		self.scale      = 1.0f;
		self.distance   = 1.0f;
		self.near       = 0.1f;
		self.far        = 100.0f;
		self.aspect     = 1.0f;
		self.fovy       = (float)(30.0 * math::TO_RADIAN);
		self.ortho_size = 5.0f;

		return self;
	}

	inline static void
	camera_viewport_update(Camera& self, int width, int height)
	{
		self.width = width;
		self.height = height;
		self.aspect = (float)width / height;
	}

	inline static void
	camera_input_update(Camera& self, const Rex_Input& input, float dt)
	{
		auto rotation = math::mat3_euler(self.rotation);

		auto up = math::V3{0.0f, 1.0f, 0.0f} * rotation;
		auto right = math::V3{1.0f, 0.0f, 0.0f} * rotation;

		// ban
		static constexpr float SENSITIVITY = 100.0f;
		if (input.keys[REX_KEY_MOUSE_RIGHT].down || input.keys[REX_KEY_MOUSE_MIDDLE].down)
		{
			auto dx = input.mouse_dx * dt * SENSITIVITY / self.width;
			auto dy = -input.mouse_dy * dt * SENSITIVITY / self.width;

			self.position -= (right * dx + up * dy);
		}

		if (input.keys[REX_KEY_MOUSE_LEFT].down)
		{
			auto dx = input.mouse_dx * dt * SENSITIVITY / self.width;
			auto dy = -input.mouse_dy * dt * SENSITIVITY / self.width;

			self.rotation.x += dy;
			self.rotation.y -= dx;

			if (self.rotation.x >= 89.0f * (float)math::TO_RADIAN)
				self.rotation.x = 89.0f * (float)math::TO_RADIAN;
			else if (self.rotation.x <= -89.0f * (float)math::TO_RADIAN)
				self.rotation.x = -89.0f * (float)math::TO_RADIAN;
		}

		if (input.mouse_wheel > 0)
			self.scale *= 1.1f;
		else if (input.mouse_wheel < 0)
			self.scale *= 0.9f;

		if (input.keys[REX_KEY_SPACE].pressed)
		{
			self.position = {};
			self.rotation = {};
			self.scale = 1;
		}

	}

	inline static math::M4
	camera_view_mat(const Camera& self)
	{
		auto rotation = math::mat3_rotation_x(self.rotation.x) * math::mat3_rotation_y(self.rotation.y);

		auto forward = math::V3{0.0f, 0.0f, 1.0f} * rotation;
		auto up = math::V3{0.0f, 1.0f, 0.0f} * rotation;

		return math::mat4_look_at(forward * self.distance * self.scale + self.position, self.position, up);
	}

	inline static math::M4
	camera_proj_mat(const Camera& self)
	{
		if (self.kind == Camera::PROJECTION_ORTHOGRAPHIC)
		{
			return math::mat4_ortho(
				-self.ortho_size * self.aspect * self.scale,
				 self.ortho_size * self.aspect * self.scale,
				-self.ortho_size * self.scale,
				 self.ortho_size * self.scale,
				 self.near,
				 self.far
			);
		}
		else if (self.kind == Camera::PROJECTION_PERSPECTIVE)
		{
			return math::mat4_perspective(self.fovy, self.aspect, self.near, self.far);
		}
		return math::mat4_identity<float>();
	}
}
