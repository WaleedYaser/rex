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
    *self = Rex{};
}

inline static void
destroy(Rex* self)
{
    self->free(self->depth_buffer);
    self->free(self->canvas.pixels);
}

inline static void
reload(Rex* self)
{

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
    Mat4 model = mat4_euler(0, t, 0) * mat4_translation(0, 0, -5);
    // projection matrix
    Mat4 proj = mat4_perspective(30, (float)canvas.width / (float)canvas.height, 0.1f, 100.0f);

    // triangle in camera space
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

    // transform vertices to clip space
    for (int i = 0; i < count; ++i)
    {
        Vec4 v = Vec4{ vertices[i].x, vertices[i].y, vertices[i].z, 1.0f } * model * proj;
        v /= v.w;
        vertices[i] = {v.x, v.y, v.z};
    }

    // transform vertices to screen space
    for (int i = 0; i < count; ++i)
    {
        vertices[i].x = (vertices[i].x + 1.0f) * canvas.width * 0.5f;
        vertices[i].y = (vertices[i].y + 1.0f) * canvas.height * 0.5f;
    }

    for (int i = 0; i < count; i += 3)
    {
        Vec2 v0 = { vertices[i+0].x, vertices[i+0].y };
        Vec2 v1 = { vertices[i+1].x, vertices[i+1].y };
        Vec2 v2 = { vertices[i+2].x, vertices[i+2].y };

        Vec2 a = v1 - v0;
        Vec2 b = v2 - v1;
        Vec2 c = v0 - v2;

        float area = -cross(a, b);

        Vec2 bb_min = min(min(v0, v1), v2);
        Vec2 bb_max = max(max(v0, v1), v2);

        int min_x = (int)max(0, bb_min.x);
        int max_x = (int)min((float)canvas.width, bb_max.x);

        int min_y = (int)max(0, bb_min.y);
        int max_y = (int)min((float)canvas.height, bb_max.y);

        for (int y = min_y; y < max_y; ++y)
        {
            for (int x = min_x; x < max_x; ++x)
            {
                Vec2 p = Vec2{(float)x, (float)y};

                float w0 = cross(p - v1, b) / area;
                float w1 = cross(p - v2, c) / area;
                float w2 = cross(p - v0, a) / area;

                if (w0 > 0 && w1 > 0 && w2 > 0)
                {
                    float depth = w0 * vertices[i+0].z + w1 * vertices[i+1].z + w2 * vertices[i+2].z;
                    if (depth < self->depth_buffer[y * canvas.width + x])
                    {
                        canvas.pixels[y * canvas.width + x] = w0 * colors[i] + w1 * colors[i+1] + w2 * colors[i+2];
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