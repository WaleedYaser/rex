#include "rex.h"

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
    }

    // clear
    for (int y = 0; y < canvas.height; ++y)
    {
        for (int x = 0; x < canvas.width; ++x)
        {
            canvas.pixels[y * canvas.width + x] = color_palette[0];
        }
    }

    // draw a triangle
    float triangle_cs[] = {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
    };

    Pixel tri_colors[] = {
        color_palette[1],
        color_palette[2],
        color_palette[3],
        // {1.0f, 0.0f, 0.0f, 1.0f},
        // {0.0f, 1.0f, 0.0f, 1.0f},
        // {0.0f, 0.0f, 1.0f, 1.0f},
    };

    float triangle_ss[6] = { };

    // convert triangle to screen space
    for (int i = 0; i < 6; i += 2)
    {
        triangle_ss[i+0] = (triangle_cs[i+0] + 1.0f) * canvas.width * 0.5f;
        triangle_ss[i+1] = (1.0f - triangle_cs[i+1]) * canvas.height * 0.5f;
    }

    // fill triangle
    float triangle_vecs[6] = {
        triangle_ss[2] - triangle_ss[0],
        triangle_ss[3] - triangle_ss[1],

        triangle_ss[4] - triangle_ss[2],
        triangle_ss[5] - triangle_ss[3],

        triangle_ss[0] - triangle_ss[4],
        triangle_ss[1] - triangle_ss[5],
    };

    float area = -(triangle_vecs[0] * triangle_vecs[3] - triangle_vecs[1] * triangle_vecs[2]);
    for (int y = 0; y < canvas.height; ++y)
    {
        for (int x = 0; x < canvas.width; ++x)
        {
            float weights[3] = {};
            for (int i = 0; i < 6; i +=2)
            {
                float vx = x - triangle_ss[i+0];
                float vy = y - triangle_ss[i+1];

                weights[i/2] = (vx * triangle_vecs[i+1] - vy * triangle_vecs[i+0]) / area;
            }

            if (weights[0] > 0 && weights[1] > 0 && weights[2] > 0) {
                canvas.pixels[y * canvas.width + x] = {
                    tri_colors[2].r * weights[0] + tri_colors[0].r * weights[1] + tri_colors[1].r * weights[2],
                    tri_colors[2].g * weights[0] + tri_colors[0].g * weights[1] + tri_colors[1].g * weights[2],
                    tri_colors[2].b * weights[0] + tri_colors[0].b * weights[1] + tri_colors[1].b * weights[2],
                };
            }
        }
    }

    // draw triangle points
    for (int i = 0; i < 6; i += 2)
    {
        int px = (int)triangle_ss[i+0];
        int py = (int)triangle_ss[i+1];
        int r = 20;
        int rr = r * r;
        for (int y = py - r; y < py + r; ++y)
        {
            for (int x = px - r; x < px + r; ++x)
            {
                int d = (x - px) * (x - px) + (y - py) * (y - py);
                if (d <= rr)
                    canvas.pixels[y * canvas.width + x] = tri_colors[i / 2];
            }
        }
    }


#if 1
    // draw animated circle in the middle
    static bool reverse = false;
    static float t = 0;
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

    // reverse animation
    if (t <= 0 || t >= 1)
        reverse = !reverse;

    // update t
    t = reverse ? t - self->dt : t + self->dt;
#endif
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