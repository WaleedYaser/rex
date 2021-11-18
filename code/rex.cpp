#include "rex.h"

#include "geometry.h"
#include "constants.h"

#include "assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

inline static Pixel operator*(Pixel p, float f) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator*(float f, Pixel p) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator+(Pixel p1, Pixel p2) { return {p1.r + p2.r, p1.g + p2.g, p1.b + p2.b, p1.a + p2.a}; }

inline static void
init(Rex* self)
{
    // parse stl file
    Content stl_data = self->file_read(L"../data/dino.stl");
    {
        unsigned char* ptr = stl_data.data;
        // skip header (80 bytes)
        ptr += 80;
        // number of triangles (4 bytes)
        unsigned int triangles_number = *(unsigned int*)ptr;
        ptr += 4;
        // allocate data
        self->vertices_count = triangles_number * 3;
        self->vertices = (Vec3*)self->alloc(self->vertices_count * sizeof(Vec3));
        self->normals = (Vec3*)self->alloc(self->vertices_count * sizeof(Vec3));
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
    self->free(stl_data.data);

    // center stl
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

    // load image file
    {
        int width, height, channels;
        unsigned char* data = stbi_load("../data/girl/textures/color_baseColor.jpeg", &width, &height, &channels, 4);

        self->image.pixels = (Pixel*)self->alloc(width * height * sizeof(Pixel));
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
    }
}

inline static void
destroy(Rex* self)
{
    self->free(self->image.pixels);
    self->free(self->depth_buffer);
    self->free(self->canvas.pixels);
    self->free(self->vertices);
    self->free(self->normals);
}

inline static void
reload(Rex* self)
{
    self->free(self->vertices);
    init(self);
}

inline static void
loop(Rex* self)
{
    auto& canvas = self->canvas;

    if (canvas.width != self->window_width || canvas.height != self->window_height)
    {
        self->free(canvas.pixels);
        canvas.pixels = (Pixel*)self->alloc(self->window_width * self->window_height * sizeof(Pixel));
        canvas.width = self->window_width;
        canvas.height = self->window_height;

        self->free(self->depth_buffer);
        self->depth_buffer = (float*)self->alloc(self->window_width * self->window_height * sizeof(float));
    }

    // clear color and depth
    for (int i = 0; i < canvas.height * canvas.width; ++i)
    {
        canvas.pixels[i] = color_palette[0];
        self->depth_buffer[i] = -300.0f;
    }

    // animation parameters
    static bool reverse = false;
    static float t = 0;

// #define STL 1
// #define TRI 1
// #define CUBE 1
#define QUAD 1

#if STL
    Vec3* vertices = self->vertices;
    Vec3* normals = self->normals;
    int count = self->vertices_count;
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
        // transform vertices to world space
        Vec4 _v0_w = Vec4{ vertices[i+0].x, vertices[i+0].y, vertices[i+0].z, 1.0f } * model;
        Vec4 _v1_w = Vec4{ vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, 1.0f } * model;
        Vec4 _v2_w = Vec4{ vertices[i+2].x, vertices[i+2].y, vertices[i+2].z, 1.0f } * model;

        Vec3 v0_w = Vec3{ _v0_w.x, _v0_w.y, _v0_w.z };
        Vec3 v1_w = Vec3{ _v1_w.x, _v1_w.y, _v1_w.z };
        Vec3 v2_w = Vec3{ _v2_w.x, _v2_w.y, _v2_w.z };

        // transform normals to world space
#if QUAD == 0
        Vec4 _n0 = Vec4{ normals[i+0].x, normals[i+0].y, normals[i+0].z, 0.0f } * model;
        Vec4 _n1 = Vec4{ normals[i+1].x, normals[i+1].y, normals[i+1].z, 0.0f } * model;
        Vec4 _n2 = Vec4{ normals[i+2].x, normals[i+2].y, normals[i+2].z, 0.0f } * model;

        Vec3 n0 = Vec3{ _n0.x, _n0.y, _n0.z };
        Vec3 n1 = Vec3{ _n1.x, _n1.y, _n1.z };
        Vec3 n2 = Vec3{ _n2.x, _n2.y, _n2.z };
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

                if (w0 > 0 && w1 > 0 && w2 > 0)
                {
                    float depth = w0 * v0_w.z + w1 * v1_w.z + w2 * v2_w.z;
                    if (depth > self->depth_buffer[y * canvas.width + x])
                    {
#if STL
                        Pixel color = color_palette[1];
#elif QUAD == 0
                        Pixel color = w0 * colors[i] + w1 * colors[i+1] + w2 * colors[i+2];
#else
                        Vec2 uv = w0 * quad_uvs[i+0] + w1 * quad_uvs[i+1] + w2 * quad_uvs[i+2];
                        int idx = (int)(uv.x * (self->image.width - 1)) + (int)(uv.y * (self->image.height - 1)) * self->image.width;
                        Pixel color = self->image.pixels[idx];
#endif

#define LIGHT 0
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

Rex_Api*
rex_api()
{
    static Rex_Api api;
    api.init = init;
    api.destory = destroy;
    api.reload = reload;
    api.loop = loop;
    return &api;
}