#pragma once

struct Vec2
{
	float x, y;
};

inline static Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }

inline static float cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
