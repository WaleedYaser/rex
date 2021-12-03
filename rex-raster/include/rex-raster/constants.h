#pragma once

#include <rex-core/api.h>
#include "rex-raster/geometry.h"

// colors
static constexpr Pixel color_palette[] = {
    {0.19f, 0.30f, 0.39f, 1.0f}, // dark blue
    {0.70f, 0.91f, 0.91f, 1.0f}, // light blue
    {0.56f, 0.73f, 0.67f, 1.0f}, // green
    {0.95f, 0.82f, 0.59f, 1.0f}, // yellow
    {0.93f, 0.54f, 0.46f, 1.0f}, // orange
};

// triangle
static constexpr Vec3 triangle_vertices[] = {
    {  0.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
};
static constexpr Vec3 triangle_normals[] = {
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
};
static constexpr Pixel triangle_colors[] = {
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
};
static constexpr unsigned int triangle_vertices_count = sizeof(triangle_vertices) / sizeof(triangle_vertices[0]);

// quad
static constexpr Vec3 quad_vertices[] = {
    { -1.0f,  1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
    {  1.0f,  1.0f, 0.0f },

    { -1.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
};
static constexpr Vec2 quad_uvs[] = {
    { 0.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },

    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
};
static constexpr unsigned int quad_vertices_count = sizeof(quad_vertices) / sizeof(quad_vertices[0]);

// cube from learnopengl.com
static constexpr Vec3 cube_vertices[] = {
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
};
static constexpr Vec3 cube_normals[] = {
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
};
static constexpr Pixel cube_colors[] = {
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
};
static constexpr unsigned int cube_vertices_count = sizeof(cube_vertices) / sizeof(cube_vertices[0]);