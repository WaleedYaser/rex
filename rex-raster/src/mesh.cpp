#include "rex-raster/mesh.h"

#include <rex-core/file.h>
#include <rex-core/log.h>
#include <rex-core/assert.h>
#include <rex-math/vec3.h>

#include <stdint.h>

#if 0
# include "rex-raster/gltf.h"
#endif

namespace rex::raster
{
	inline static void
	_mesh_bounding_box(Mesh& self)
	{
		rex_assert(self.position.count);

		self.bb_min = self.position[0];
		self.bb_max = self.position[0];

		for (auto p: self.position)
		{
			self.bb_min = math::min(self.bb_min, p);
			self.bb_max = math::max(self.bb_max, p);
		}
	}


	Mesh
	mesh_init()
	{
		Mesh self = {};
		self.position = rc::vec_init<math::V3>();
		self.normal = rc::vec_init<math::V3>();
		self.color = rc::vec_init<math::Color_F32>();
		self.uv = rc::vec_init<math::V2>();
		self.indices = rc::vec_init<unsigned>();
		return self;
	}

	void
	mesh_deinit(Mesh& self)
	{
		rc::vec_deinit(self.position);
		rc::vec_deinit(self.normal);
		rc::vec_deinit(self.color);
		rc::vec_deinit(self.uv);
		rc::vec_deinit(self.indices);
		rc::vec_deinit(self.uv_indices);
		self = {};
	}

	Mesh
	mesh_triangle()
	{
		Mesh self = {};

		self.position = rc::vec_from<math::V3>({
			{  0.0f,  1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f },
			{  1.0f, -1.0f, 0.0f },
		});
		self.normal = rc::vec_from<math::V3>({
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
		});
		self.color = rc::vec_from<math::Color_F32>({
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
		});

		self.uv = rc::vec_init<math::V2>();
		self.indices = rc::vec_init<unsigned>();

		_mesh_bounding_box(self);

		return self;
	}

	Mesh
	mesh_quad()
	{
		Mesh self = {};

		self.position = rc::vec_from<math::V3>({
			{ -1.0f,  1.0f, 0.0f },
			{  1.0f, -1.0f, 0.0f },
			{  1.0f,  1.0f, 0.0f },

			{ -1.0f,  1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f },
			{  1.0f, -1.0f, 0.0f },
		});
		self.normal = rc::vec_from<math::V3>({
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },

			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f },
		});
		self.uv = rc::vec_from<math::V2>({
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },

			{ 0.0f, 0.0f },
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
		});

		self.color = rc::vec_init<math::Color_F32>();
		self.indices = rc::vec_init<unsigned>();

		_mesh_bounding_box(self);

		return self;
	}

	Mesh
	mesh_cube()
	{
		Mesh self = {};

		// cube from learnopengl.com
		self.position = rc::vec_from<math::V3>({
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{ -1.0f,  1.0f, -1.0f },
			{ -1.0f, -1.0f, -1.0f },

			{ -1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{ -1.0f, -1.0f,  1.0f },

			{ -1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f, -1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{ -1.0f, -1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },

			{  1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },

			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{ -1.0f, -1.0f,  1.0f },
			{ -1.0f, -1.0f, -1.0f },

			{ -1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{ -1.0f,  1.0f, -1.0f },
		});
		self.normal = rc::vec_from<math::V3>({
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f, -1.0f },
			{  0.0f,  0.0f, -1.0f },

			{  0.0f,  0.0f,  1.0f },
			{  0.0f,  0.0f,  1.0f },
			{  0.0f,  0.0f,  1.0f },
			{  0.0f,  0.0f,  1.0f },
			{  0.0f,  0.0f,  1.0f },
			{  0.0f,  0.0f,  1.0f },

			{ -1.0f,  0.0f,  0.0f },
			{ -1.0f,  0.0f,  0.0f },
			{ -1.0f,  0.0f,  0.0f },
			{ -1.0f,  0.0f,  0.0f },
			{ -1.0f,  0.0f,  0.0f },
			{ -1.0f,  0.0f,  0.0f },

			{  1.0f,  0.0f,  0.0f },
			{  1.0f,  0.0f,  0.0f },
			{  1.0f,  0.0f,  0.0f },
			{  1.0f,  0.0f,  0.0f },
			{  1.0f,  0.0f,  0.0f },
			{  1.0f,  0.0f,  0.0f },

			{  0.0f, -1.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f },

			{  0.0f,  1.0f,  0.0f },
			{  0.0f,  1.0f,  0.0f },
			{  0.0f,  1.0f,  0.0f },
			{  0.0f,  1.0f,  0.0f },
			{  0.0f,  1.0f,  0.0f },
			{  0.0f,  1.0f,  0.0f },
		});
		self.color = rc::vec_from<math::Color_F32>({
			{1.0f, 0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},

			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},

			{0.0f, 0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},

			{1.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},

			{0.0f, 1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 1.0f, 1.0f},

			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
		});

		self.uv = rc::vec_init<math::V2>();
		self.indices = rc::vec_init<unsigned>();

		_mesh_bounding_box(self);

		return self;
	}

	Mesh
	mesh_from_stl(const char* path)
	{
		Mesh self = {};

		auto content = rc::file_read(path, rc::frame_allocator());
		if (content.ptr == nullptr)
		{
			rex_log_error("[rex-raster]: failed to load stl file '%s'", path);
			return self;
		}

		auto ptr = content.ptr;
		// skip header (80 bytes)
		ptr += 80;
		// number of triangles (4 bytes)
		uint32_t triangles_number = *(uint32_t*)ptr;
		ptr += 4;
		// allocate data
		self.position = rc::vec_with_count<math::V3>(triangles_number * 3);
		self.normal = rc::vec_with_count<math::V3>(triangles_number * 3);
		self.color = rc::vec_init<math::Color_F32>();
		self.uv = rc::vec_init<math::V2>();
		self.indices = rc::vec_init<unsigned>();

		// parse triangles
		for (uint32_t i = 0; i < triangles_number; ++i)
		{
			// copy triangle normal (12 bytes)
			self.normal[i*3 + 0] = *(math::V3*)ptr;
			self.normal[i*3 + 1] = *(math::V3*)ptr;
			self.normal[i*3 + 2] = *(math::V3*)ptr;
			ptr += 12;
			// copy 3 triangle vertices (12 bytes each)
			self.position[i*3 + 0] = *(math::V3*)ptr;
			ptr += 12;
			self.position[i*3 + 1] = *(math::V3*)ptr;
			ptr += 12;
			self.position[i*3 + 2] = *(math::V3*)ptr;
			ptr += 12;
			// skip attribute byt count (2 bytes)
			ptr += 2;
		}

		_mesh_bounding_box(self);

		return self;
	}

	Mesh
	mesh_from_obj(const char* path)
	{
		Mesh self = {};
		self.position = rc::vec_init<math::V3>();
		self.uv = rc::vec_init<math::V2>();
		self.normal = rc::vec_init<math::V3>();
		self.indices = rc::vec_init<unsigned>();
		self.uv_indices = rc::vec_init<unsigned>();

		auto content = rc::file_read(path, rc::frame_allocator());
		if (content.ptr == nullptr)
		{
			rex_log_error("[rex-raster]: failed to load obj file '%s'", path);
			return self;
		}

		auto lines = rc::str_lines(content, rc::frame_allocator());
		for (const auto& line: lines)
		{
			// skip empty lines
			if (line.count == 0)
				continue;

			// skip comments
			if (line[0] == '#')
				continue;

			bool result = false;
			auto splits = rc::str_split(line, ' ', rc::frame_allocator());
			if (splits[0] == "v")
			{
				rex_assert(splits.count == 4);
				math::V3 v = {};
				v.x = (float)rc::str_to_double(splits[1], &result); rex_assert(result);
				v.y = (float)rc::str_to_double(splits[2], &result); rex_assert(result);
				v.z = (float)rc::str_to_double(splits[3], &result); rex_assert(result);
				rc::vec_push(self.position, v);
			}
			else if (splits[0] == "vt")
			{
				rex_assert(splits.count == 3 || splits.count == 4);
				math::V2 v = {};
				v.x = (float)rc::str_to_double(splits[1], &result); rex_assert(result);
				v.y = (float)rc::str_to_double(splits[2], &result); rex_assert(result);
				rc::vec_push(self.uv, v);
			}
			else if (splits[0] == "vn")
			{
				rex_assert(splits.count == 4);
				math::V3 v = {};
				v.x = (float)rc::str_to_double(splits[1], &result); rex_assert(result);
				v.y = (float)rc::str_to_double(splits[2], &result); rex_assert(result);
				v.z = (float)rc::str_to_double(splits[3], &result); rex_assert(result);
				rc::vec_push(self.normal, v);
			}
			else if (splits[0] == "f")
			{
				for (rc::sz i = 1; i < splits.count; ++i)
				{
					auto indices = rc::str_split(splits[i], '/', rc::frame_allocator());
					unsigned idx = (unsigned)rc::str_to_int(indices[0], &result) - 1; rex_assert(result);
					rc::vec_push(self.indices, idx);
					if (indices.count > 1)
					{
						unsigned uv_idx = (unsigned)rc::str_to_int(indices[1], &result) - 1; rex_assert(result);
						rc::vec_push(self.uv_indices, uv_idx);
					}
				}
			}
			else
			{
				continue;
				// rex_assert(false);
			}
		}

		_mesh_bounding_box(self);

		return self;
	}

#if 0
	Mesh
	mesh_from_gltf(const char* path)
	{
		auto gltf = gltf_load(self);
		{
			const char* image_path = "";
			for (int i = 0; i < gltf.root.as_object.length; ++i)
			{
				if (str_eq(gltf.root.as_object.members[i].name, "images"))
				{
					image_path = gltf.root.as_object.members[i].value.as_array.values[0].as_object.members[0].value.as_str;
					break;
				}
			}
			image_path = str_concat("data/girl/", image_path);

			// load image file
			int width, height, channels;
			unsigned char* data = stbi_load(image_path, &width, &height, &channels, 4);

			self->image.pixels = rex_alloc_N(Pixel, width * height);
			self->image.width = width;
			self->image.height = height;

			for (int i = 0; i < width * height; ++i)
			{
				self->image.pixels[i].r = data[i*4 + 0] / 255.0f;
				self->image.pixels[i].g = data[i*4 + 1] / 255.0f;
				self->image.pixels[i].b = data[i*4 + 2] / 255.0f;
				self->image.pixels[i].a = data[i*4 + 3] / 255.0f;
			}

			stbi_image_free(data);

			{
				auto buffer = json_find(gltf.root, "buffers").as_array.values[0];
				auto buffer_length = json_find(buffer, "byteLength").as_int;
				auto buffer_uri = json_find(buffer, "uri").as_str;

				auto bin_content = rc::file_read(str_concat("data/girl/", buffer_uri));
				assert(bin_content.count == buffer_length);

				// load only first mesh
				auto meshes = json_find(gltf.root, "meshes");
				auto mesh0 = meshes.as_array.values[0];
				auto primitive = json_find(mesh0, "primitives").as_array.values[0];
				auto attributes = json_find(primitive, "attributes");

				auto position_attr = json_find(attributes, "POSITION").as_int;
				auto normal_attr = json_find(attributes, "NORMAL").as_int;
				auto uv_attr = json_find(attributes, "TEXCOORD_0").as_int;
				auto indices_attr = json_find(primitive, "indices").as_int;

				auto accessors = json_find(gltf.root, "accessors");
				auto position_acc = accessors.as_array.values[position_attr];
				auto normal_acc = accessors.as_array.values[normal_attr];
				auto uv_acc = accessors.as_array.values[uv_attr];
				auto indices_acc = accessors.as_array.values[indices_attr];

				auto position_count = json_find(position_acc, "count").as_int;
				auto normal_count = json_find(normal_acc, "count").as_int;
				auto uv_count = json_find(uv_acc, "count").as_int;
				auto indices_count = json_find(indices_acc, "count").as_int;

				assert(position_count == normal_count && normal_count == uv_count);
				self->vertices_count = (unsigned int)position_count;
				self->indices_count = (unsigned int)indices_count;

				auto position_offset = json_find(position_acc, "byteOffset").as_int;
				auto normal_offset = json_find(normal_acc, "byteOffset").as_int;
				auto uv_offset = json_find(uv_acc, "byteOffset").as_int;
				auto indices_offset = json_find(indices_acc, "byteOffset").as_int;

				auto position_view_index = json_find(position_acc, "bufferView").as_int;
				auto normal_view_index = json_find(normal_acc, "bufferView").as_int;
				auto uv_view_index = json_find(uv_acc, "bufferView").as_int;
				auto indices_view_index = json_find(indices_acc, "bufferView").as_int;

				auto bufferViews = json_find(gltf.root, "bufferViews");
				auto position_view = bufferViews.as_array.values[position_view_index];
				auto normal_view = bufferViews.as_array.values[normal_view_index];
				auto uv_view = bufferViews.as_array.values[uv_view_index];
				auto indices_view = bufferViews.as_array.values[indices_view_index];

				position_offset += json_find(position_view, "byteOffset").as_int;
				normal_offset += json_find(normal_view, "byteOffset").as_int;
				uv_offset += json_find(uv_view, "byteOffset").as_int;
				indices_offset += json_find(indices_view, "byteOffset").as_int;

				self->vertices = (Vec3*)(bin_content.ptr + position_offset);
				self->normals = (Vec3*)(bin_content.ptr + normal_offset);
				self->uvs = (Vec2*)(bin_content.ptr + uv_offset);
				self->indices = (unsigned int*)(bin_content.ptr + indices_offset);
			}
		}
	}
#endif
}
