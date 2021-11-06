#include "rex.h"

#include "geometry.h"

inline static Pixel operator*(Pixel p, float f) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator*(float f, Pixel p) { return {p.r * f, p.g * f, p.b * f, p.a * f}; }
inline static Pixel operator+(Pixel p1, Pixel p2) { return {p1.r + p2.r, p1.g + p2.g, p1.b + p2.b, p1.a + p2.a}; }

static constexpr Pixel color_palette[] = {
    {0.19f, 0.30f, 0.39f, 1.0f}, // dark blue
    {0.70f, 0.91f, 0.91f, 1.0f}, // light blue
    {0.56f, 0.73f, 0.67f, 1.0f}, // green
    {0.95f, 0.82f, 0.59f, 1.0f}, // yellow
    {0.93f, 0.54f, 0.46f, 1.0f}, // orange
};

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
        self->bunny_vertices_count = triangles_number * 3;
        self->bunny_vertices = (Vec3*)self->alloc(self->bunny_vertices_count * sizeof(Vec3));
        self->bunny_normals = (Vec3*)self->alloc(self->bunny_vertices_count * sizeof(Vec3));
        // parse triangles
        for (unsigned int i = 0; i < triangles_number; ++i)
        {
            // copy triangle normal (12 bytes)
            self->bunny_normals[i*3 + 0] = *(Vec3*)ptr;
            self->bunny_normals[i*3 + 1] = *(Vec3*)ptr;
            self->bunny_normals[i*3 + 2] = *(Vec3*)ptr;
            ptr += 12;
            // copy 3 triangle vertices (12 bytes each)
            self->bunny_vertices[i*3 + 0] = *(Vec3*)ptr;
            ptr += 12;
            self->bunny_vertices[i*3 + 1] = *(Vec3*)ptr;
            ptr += 12;
            self->bunny_vertices[i*3 + 2] = *(Vec3*)ptr;
            ptr += 12;
            // skip attribute byt count (2 bytes)
            ptr += 2;
        }
    }
    self->free(stl_data.data);

    // center stl
    Vec3 bb_min = self->bunny_vertices[0];
    Vec3 bb_max = self->bunny_vertices[0];
    for (unsigned int i = 1; i < self->bunny_vertices_count; ++i)
    {
        bb_min = min(bb_min, self->bunny_vertices[i]);
        bb_max = max(bb_max, self->bunny_vertices[i]);
    }
    for (unsigned int i = 0; i < self->bunny_vertices_count; ++i)
        self->bunny_vertices[i] -= ((bb_max + bb_min) * 0.5f);

    self->camera_z = (bb_max - bb_min).z * 2.5f;
}

inline static void
destroy(Rex* self)
{
    self->free(self->depth_buffer);
    self->free(self->canvas.pixels);
    self->free(self->bunny_vertices);
    self->free(self->bunny_normals);
}

inline static void
reload(Rex* self)
{
    self->free(self->bunny_vertices);
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
        self->depth_buffer[i] = 1.0f;
    }

    // animation parameters
    static bool reverse = false;
    static float t = 0;

    // model matrix
    // Mat4 model = mat4_euler(0, t, 0) * mat4_translation(0, 0, -50);
    Mat4 model =  mat4_rotation_x(-3.14f * 0.5f) * mat4_rotation_y(t) * mat4_translation(0, 0, -self->camera_z);
    // projection matrix
    Mat4 proj = mat4_perspective(30, (float)canvas.width / (float)canvas.height, 0.1f, 300.0f);

    // triangle in camera space
#if 0
    Vec3 vertices[] = {
        // triangle
#if 0
        {  0.0f,  1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
#else
        // box from learnopengl.com
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
#endif
    };
    int count = sizeof(vertices) / sizeof(Vec3);

    Pixel colors[] = {
        // color_palette[1],
        // color_palette[2],
        // color_palette[3],
#if 0
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
#else
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
#endif
    };

#endif

    Vec3* vertices = self->bunny_vertices;
    Vec3* normals = self->bunny_normals;
    int count = self->bunny_vertices_count;

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
        Vec4 _n0 = Vec4{ normals[i+0].x, normals[i+0].y, normals[i+0].z, 0.0f } * model;
        Vec4 _n1 = Vec4{ normals[i+1].x, normals[i+1].y, normals[i+1].z, 0.0f } * model;
        Vec4 _n2 = Vec4{ normals[i+2].x, normals[i+2].y, normals[i+2].z, 0.0f } * model;

        Vec3 n0 = Vec3{ _n0.x, _n0.y, _n0.z };
        Vec3 n1 = Vec3{ _n1.x, _n1.y, _n1.z };
        Vec3 n2 = Vec3{ _n2.x, _n2.y, _n2.z };

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
                    float depth = w0 * v0_c.z + w1 * v1_c.z + w2 * v1_c.z;
                    if (depth < self->depth_buffer[y * canvas.width + x])
                    {
                        Pixel light_color = color_palette[3];
                        Vec3 light_pos = {};
                        // phong lighting
                        // ambient
                        Pixel ambient = 0.2f * light_color;
                        // diffuse
                        Vec3 frag_pos = v0_w * w0 + v1_w * w1 + v2_w * w2;
                        Vec3 light_dir = normalize(light_pos - frag_pos);
                        float diff = max(dot(n0, light_dir), 0.0);
                        Pixel diffuse = diff * light_color;
                        canvas.pixels[y * canvas.width + x] = diffuse;
                        // canvas.pixels[y * canvas.width + x] = Pixel{n0.x, n0.y, n0.z};

#if 0
                        canvas.pixels[y * canvas.width + x] = w0 * colors[i] + w1 * colors[i+1] + w2 * colors[i+2];
#endif
                        // canvas.pixels[y * canvas.width + x] = color_palette[1];
                        self->depth_buffer[y * canvas.width + x] = depth;
                    }
                }
            }
        }
    }

#if 0
    // draw animated circle in the middle
    int mid_x = canvas.width / 2;
    int mid_y = canvas.height / 2;
    float radius_squared = 100 * 100 + t*t * 100 * 100;
    for (int y = 0; y < canvas.height; ++y)
    {
        for (int x = 0; x < canvas.width; ++x)
        {
            int d = (x - mid_x) * (x - mid_x) + (y - mid_y) * (y - mid_y);
            if (d <= radius_squared)
                canvas.pixels[y * canvas.width + x] = color_palette[4];
        }
    }

#endif

    // reverse animation
    if (t <= -3.14f)
        reverse = false;
    else if (t >= 3.14f)
        reverse = true;

    // update t
    float dt = self->dt > 0.033f ? 0.033f : self->dt;
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