// #include "rex-raster/gltf.h"
#include "rex-raster/exports.h"
#include "rex-raster/geometry.h"
#include "rex-raster/constants.h"

#include <rex-core/api.h>
#include <rex-core/memory.h>
#include <rex-core/str.h>
#include <rex-core/defer.h>
#include <rex-core/path.h>
#include <rex-core/file.h>
#include <rex-core/log.h>

#include "assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "rex-raster/stb_image.h"

inline static Pixel operator*(Pixel p, float f) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator*(float f, Pixel p) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator+(Pixel p1, Pixel p2) { return {p1.r + p2.r, p1.g + p2.g, p1.b + p2.b, p1.a + p2.a}; }

inline static const char*
str_concat(const char *a, const char *b)
{
    int a_len = 0;
    for (const char* ptr = a; *ptr != '\0'; ptr++, a_len++);
    int b_len = 0;
    for (const char* ptr = b; *ptr != '\0'; ptr++, b_len++);

    char* res = (char*)malloc(a_len + b_len + 1);
    for (int i = 0; i < a_len; ++i)
        res[i] = a[i];
    for (int i = 0; i < b_len; ++i)
        res[a_len+i] = b[i];
    res[a_len + b_len] = '\0';

    return res;
}

inline static void
init(Rex_Api* self)
{
#if 1
#if 0
    // parse stl file
    auto stl_data = rc::file_read(rc::str_fmt(rc::frame_allocator(), "%s/data/dino.stl", rc::app_directory()));
    rex_defer(rc::str_deinit(stl_data));
    {
        auto ptr = stl_data.ptr;
        // skip header (80 bytes)
        ptr += 80;
        // number of triangles (4 bytes)
        unsigned int triangles_number = *(unsigned int*)ptr;
        ptr += 4;
        // allocate data
        self->vertices_count = triangles_number * 3;
        self->vertices = rex_alloc_N(Vec3, self->vertices_count);
        self->normals = rex_alloc_N(Vec3, self->vertices_count);
        // parse triangles
        for (unsigned int i = 0; i < triangles_number; ++i)
        {
            // copy triangle normal (12 bytes)
            self->normals[i*3 + 0] = *(Vec3*)ptr;
            self->normals[i*3 + 1] = *(Vec3*)ptr;
            self->normals[i*3 + 2] = *(Vec3*)ptr;
            ptr += 12;
            // copy 3 triangle vertices (12 bytes each)
            self->vertices[i*3 + 0] = *(Vec3*)ptr;
            ptr += 12;
            self->vertices[i*3 + 1] = *(Vec3*)ptr;
            ptr += 12;
            self->vertices[i*3 + 2] = *(Vec3*)ptr;
            ptr += 12;
            // skip attribute byt count (2 bytes)
            ptr += 2;
        }
    }
#endif

    self->vertices = (Vec3*)cube_vertices;
    self->normals = (Vec3*)cube_normals;
    self->vertices_count = cube_vertices_count;

#else
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
#endif

    // center model
    {
        Vec3 bb_min = self->vertices[0];
        Vec3 bb_max = self->vertices[0];
        for (unsigned int i = 1; i < self->vertices_count; ++i)
        {
            bb_min = min(bb_min, self->vertices[i]);
            bb_max = max(bb_max, self->vertices[i]);
        }
        for (unsigned int i = 0; i < self->vertices_count; ++i)
            self->vertices[i] -= ((bb_max + bb_min) * 0.5f);

        self->camera_z = (bb_max - bb_min).z * 2.0f;
    }
}

inline static void
deinit(Rex_Api* self)
{
    rex_dealloc(self->image.pixels);
    rex_dealloc(self->depth_buffer);
    rex_dealloc(self->canvas.pixels);
    // self->free(self->vertices);
    // self->free(self->normals);
}


inline static void
loop(Rex_Api* self)
{
    auto& canvas = self->canvas;

    if (canvas.width != self->window_width || canvas.height != self->window_height)
    {
        rex_dealloc(canvas.pixels);
        canvas.pixels = rex_alloc_N(Pixel, self->window_width * self->window_height);
        canvas.width = self->window_width;
        canvas.height = self->window_height;

        rex_dealloc(self->depth_buffer);
        self->depth_buffer = rex_alloc_N(float, self->window_width * self->window_height);
    }

    // clear color and depth
    for (int i = 0; i < canvas.height * canvas.width; ++i)
    {
        canvas.pixels[i] = { 0.1f, 0.1f, 0.1f, 1.0f };
        self->depth_buffer[i] = -300.0f;
    }

    // animation parameters
    static bool reverse = false;
    static float t = 0;

#define STL 1
// #define TEXTURE 0
// #define TRI 1
// #define CUBE 1
// #define QUAD 1

#if STL
    Vec3* vertices = self->vertices;
    Vec3* normals = self->normals;
    int count = self->indices_count > 0 ? self->indices_count : self->vertices_count;
    // model matrix
    Mat4 model =  mat4_rotation_x(-3.14f * 0.5f) * mat4_rotation_y(t) * mat4_translation(0, 0, -self->camera_z);
#elif TRI
    const Vec3* vertices = triangle_vertices;
    const Vec3* normals = triangle_normals;
    [[maybe_unused]] const Pixel* colors = triangle_colors;
    int count = triangle_vertices_count;
    // model matrix
    Mat4 model = mat4_rotation_y(t) * mat4_translation(0, 0, -5);
#elif CUBE
    const Vec3* vertices = cube_vertices;
    const Vec3* normals = cube_normals;
    [[maybe_unused]] const Pixel* colors = cube_colors;
    int count = cube_vertices_count;
    // model matrix
    Mat4 model = mat4_rotation_y(t) * mat4_translation(0, 0, -5);
#elif QUAD
    const Vec3* vertices = quad_vertices;
    int count = quad_vertices_count;
    // model matrix
    Mat4 model = mat4_rotation_y(t) * mat4_translation(0, 0, -2.5f);
#endif

    // projection matrix
    Mat4 proj = mat4_perspective(30, (float)canvas.width / (float)canvas.height, 0.1f, 300.0f);

    Mat4 mp = model * proj;
    for (int i = 0; i < count; i += 3)
    {
        int i0 = i, i1 = i+1, i2 = i+2;
        if (self->indices_count > 0)
        {
            i0 = self->indices[i+0];
            i1 = self->indices[i+1];
            i2 = self->indices[i+2];
        }

        // transform vertices to world space
        Vec4 _v0_w = Vec4{ vertices[i0].x, vertices[i0].y, vertices[i0].z, 1.0f } * model;
        Vec4 _v1_w = Vec4{ vertices[i1].x, vertices[i1].y, vertices[i1].z, 1.0f } * model;
        Vec4 _v2_w = Vec4{ vertices[i2].x, vertices[i2].y, vertices[i2].z, 1.0f } * model;

        Vec3 v0_w = Vec3{ _v0_w.x, _v0_w.y, _v0_w.z };
        Vec3 v1_w = Vec3{ _v1_w.x, _v1_w.y, _v1_w.z };
        Vec3 v2_w = Vec3{ _v2_w.x, _v2_w.y, _v2_w.z };

        // transform normals to world space
#if 1
        Vec4 _n0 = Vec4{ normals[i0].x, normals[i0].y, normals[i0].z, 0.0f } * model;
        Vec4 _n1 = Vec4{ normals[i1].x, normals[i1].y, normals[i1].z, 0.0f } * model;
        Vec4 _n2 = Vec4{ normals[i2].x, normals[i2].y, normals[i2].z, 0.0f } * model;

        Vec3 n0 = Vec3{ _n0.x, _n0.y, _n0.z };
        Vec3 n1 = Vec3{ _n1.x, _n1.y, _n1.z };
        Vec3 n2 = Vec3{ _n2.x, _n2.y, _n2.z };
#elif TEXTURE
        Vec2 uv0 = self->uvs[i0];
        Vec2 uv1 = self->uvs[i1];
        Vec2 uv2 = self->uvs[i2];
#endif

        // transform vertices to clip space
        Vec4 v0_c = Vec4{ v0_w.x, v0_w.y, v0_w.z, 1.0f } * proj;
        v0_c /= v0_c.w;
        Vec4 v1_c = Vec4{ v1_w.x, v1_w.y, v1_w.z, 1.0f } * proj;
        v1_c /= v1_c.w;
        Vec4 v2_c = Vec4{ v2_w.x, v2_w.y, v2_w.z, 1.0f } * proj;
        v2_c /= v2_c.w;

        // transform vertices to screen space
        v0_c.x = (v0_c.x + 1.0f) * canvas.width * 0.5f;
        v0_c.y = (v0_c.y + 1.0f) * canvas.height * 0.5f;
        v1_c.x = (v1_c.x + 1.0f) * canvas.width * 0.5f;
        v1_c.y = (v1_c.y + 1.0f) * canvas.height * 0.5f;
        v2_c.x = (v2_c.x + 1.0f) * canvas.width * 0.5f;
        v2_c.y = (v2_c.y + 1.0f) * canvas.height * 0.5f;

        Vec2 v0 = { v0_c.x, v0_c.y };
        Vec2 v1 = { v1_c.x, v1_c.y };
        Vec2 v2 = { v2_c.x, v2_c.y };

        Vec2 a = v1 - v0;
        Vec2 b = v2 - v1;
        Vec2 c = v0 - v2;

        float area = -cross(a, b);

        // TODO: revisit back-face culling
        // if (area >= 0)
        //     continue;

        Vec2 bb_min = min(min(v0, v1), v2);
        Vec2 bb_max = max(max(v0, v1), v2);

        int min_x = (int)max(0, bb_min.x);
        int max_x = (int)min((float)canvas.width - 1, bb_max.x);

        int min_y = (int)max(0, bb_min.y);
        int max_y = (int)min((float)canvas.height - 1, bb_max.y);

        for (int y = min_y; y <= max_y; ++y)
        {
            for (int x = min_x; x <= max_x; ++x)
            {
                Vec2 p = Vec2{x + 0.5f, y + 0.5f};

                float w0 = cross(p - v1, b) / area;
                float w1 = cross(p - v2, c) / area;
                float w2 = cross(p - v0, a) / area;

                // perspective correction
                float z = 1.0f / (
                    w0 * (1.0f / v0_w.z) +
                    w1 * (1.0f / v1_w.z) +
                    w2 * (1.0f / v2_w.z));
                w0 *= (z / v0_w.z);
                w1 *= (z / v1_w.z);
                w2 *= (z / v2_w.z);

                // rex_log_info("pass");
                if (w0 > 0 && w1 > 0 && w2 > 0)
                {
                    float depth = w0 * v0_w.z + w1 * v1_w.z + w2 * v2_w.z;
                    if (depth > self->depth_buffer[y * canvas.width + x])
                    {
#if STL
                        Pixel color = color_palette[3];
#else
                        Pixel color = w0 * colors[i0] + w1 * colors[i1] + w2 * colors[i2];
#endif

#if TEXTURE
                        Vec2 uv = w0 * uv0 + w1 * uv1 + w2 * uv2;
                        int idx = (int)(uv.x * (self->image.width - 1)) + (int)(uv.y * (self->image.height - 1)) * self->image.width;
                        Pixel color = self->image.pixels[idx];
#endif

#define LIGHT 1
#if LIGHT
                        Pixel light_color = Pixel{1.0f, 1.0f, 1.0f, 1.0f} * 1.0f;
                        Vec3 light_pos = {};
                        // very basic phong lighting
                        Vec3 frag_pos = v0_w * w0 + v1_w * w1 + v2_w * w2;
                        Vec3 light_dir = normalize(light_pos - frag_pos);
                        float diff = max(dot(n0, light_dir), 0.0);
                        Pixel diffuse = diff * light_color;

                        canvas.pixels[y * canvas.width + x] = {
                            diffuse.r * color.r,
                            diffuse.g * color.g,
                            diffuse.b * color.b,
                            diffuse.a * color.a,
                        };
#else
                        canvas.pixels[y * canvas.width + x] = color;
#endif
                        self->depth_buffer[y * canvas.width + x] = depth;
                    }
                }
            }
        }
    }

    // reverse animation
    if (t <= -3.14f)
        reverse = false;
    else if (t >= 3.14f)
        reverse = true;

    // update t
    float dt = self->dt; // > 0.033f ? 0.033f : self->dt;
    t = reverse ? t - dt : t + dt;
}

#ifdef __cplusplus
extern "C" {
#endif

REX_RASTER_EXPORT Rex_Api*
rex_api(Rex_Api* api, bool reload)
{
    if (api == nullptr)
    {
        auto self = (Rex_Api*)rex_alloc(sizeof(Rex_Api));
        *self = {};
        self->init = init;
        self->deinit = deinit;
        self->loop = loop;
        return self;
    }
    else if (reload)
    {
        api->init = init;
        api->deinit = deinit;
        api->loop = loop;

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
