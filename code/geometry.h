#pragma once

extern "C" {
    // forward declaration (cmath)
    double sqrt(double);
    double sin(double);
    double cos(double);
    double tan(double);
    double asin(double);
    double atan2(double, double);
}

inline static float sin(float f) { return (float)::sin((double)f); }
inline static float cos(float f) { return (float)::cos((double)f); }
inline static float tan(float f) { return (float)::tan((double)f); }

inline static float min(float a, float b) { return a < b ? a : b; }
inline static float max(float a, float b) { return a > b ? a : b; }

struct Vec2
{
    float x, y;
};

inline static Vec2 operator-(const Vec2& a, const Vec2& b) { return {a.x - b.x, a.y - b.y}; }

inline static float cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }

inline static Vec2 min(const Vec2& a, const Vec2& b) { return { a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y }; }
inline static Vec2 max(const Vec2& a, const Vec2& b) { return { a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y }; }

struct Vec3
{
    float x, y, z;
};

inline static Vec3 operator+(const Vec3& a, const Vec3& b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline static Vec3 operator-(const Vec3& a, const Vec3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline static Vec3& operator+=(Vec3& a, const Vec3& b) { a = {a.x + b.x, a.y + b.y, a.z + b.z}; return a; }
inline static Vec3& operator-=(Vec3& a, const Vec3& b) { a = {a.x - b.x, a.y - b.y, a.z - b.z}; return a; }
inline static Vec3 operator*(const Vec3& a, float f) { return {a.x * f, a.y * f, a.z *f}; }

inline static Vec3 min(const Vec3& a, const Vec3& b) { return { a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z }; }
inline static Vec3 max(const Vec3& a, const Vec3& b) { return { a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z }; }


union Vec4
{
    struct { float x, y, z, w; };
    float values[4];

    inline const float& operator[](int index) const { return values[index]; }
    inline float& operator[](int index) { return values[index]; }
};

inline static Vec4& operator /= (Vec4& v, float f) { v = {v.x/f, v.y/f, v.z/f, v.w/f }; return v; }

struct Mat4
{
    Vec4 rows[4];

    inline const Vec4& operator[](int index) const { return rows[index]; }
    inline Vec4& operator[](int index) { return rows[index]; }
};

inline static Vec4
operator*(const Vec4 &v, const Mat4 &M)
{
    return Vec4{
        v.x * M[0][0] + v.y * M[1][0] + v.z * M[2][0] + v.w * M[3][0],
        v.x * M[0][1] + v.y * M[1][1] + v.z * M[2][1] + v.w * M[3][1],
        v.x * M[0][2] + v.y * M[1][2] + v.z * M[2][2] + v.w * M[3][2],
        v.x * M[0][3] + v.y * M[1][3] + v.z * M[2][3] + v.w * M[3][3]
    };
}

inline static Mat4
operator*(const Mat4 &A, const Mat4 &B)
{
    Mat4 C;

    C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];
    C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];
    C[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];
    C[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];

    C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];
    C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];
    C[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];
    C[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];

    C[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];
    C[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];
    C[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];
    C[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];

    C[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];
    C[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];
    C[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];
    C[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];

    return C;
}

inline static Mat4
mat4_translation(float tx, float ty, float tz)
{
    return Mat4{
        1,  0,  0, 0,
        0,  1,  0, 0,
        0,  0,  1, 0,
        tx, ty, tz, 1
    };
}

inline static Mat4
mat4_rotation_x(float pitch)
{
    float c = cos(pitch);
    float s = sin(pitch);

    return Mat4{
        1,  0, 0, 0,
        0,  c, s, 0,
        0, -s, c, 0,
        0,  0, 0, 1
    };
}

inline static Mat4
mat4_rotation_y(float head)
{
    float c = cos(head);
    float s = sin(head);

    return Mat4{
        c, 0, -s, 0,
        0, 1,  0, 0,
        s, 0,  c, 0,
        0, 0,  0, 1
    };
}

inline static Mat4
mat4_rotation_z(float roll)
{
    float c = cos(roll);
    float s = sin(roll);

    return Mat4{
            c, s, 0, 0,
           -s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
}

inline static Mat4
mat4_euler(float pitch, float head, float roll)
{
    float sh = sin(head);
    float ch = cos(head);
    float sp = sin(pitch);
    float cp = cos(pitch);
    float sr = sin(roll);
    float cr = cos(roll);

    // order yxz
    return Mat4{
            cr*ch - sr*sp*sh, sr*ch + cr*sp*sh, -cp*sh, 0.0f,
        -sr*cp           , cr*cp           ,  sp   , 0.0f,
            cr*sh + sr*sp*ch, sr*sh - cr*sp*ch,  cp*ch, 0.0f,
            0.0f            , 0.0f            ,  0.0f  ,1.0f
    };
}

inline static Mat4
mat4_perspective(float fovy, float aspect, float znear, float zfar)
{
    Mat4 M{};

    float h = tan(fovy / 2.0f);
    float w = aspect * h;

    M[0][0] = 1.0f / w;
    M[1][1] = 1.0f / h;

    // to map z to [-1, 1] use
    // M[2][2] = -(zfar + znear) / (zfar - znear)
    // M[3][2] = -(2.0f * zfar * znear) / (zfar - znear)
    M[2][2] = -zfar / (zfar - znear);
    M[2][3] = -1.0f;
    M[3][2] = -(zfar * znear) / (zfar - znear);

    return M;
}