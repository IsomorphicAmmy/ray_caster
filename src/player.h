#pragma once

#include <raylib.h>

#define FOV 75

struct Player 
{
	Vector2 pos;
	float angle;
};

struct ColoredDistance
{
	float d;
	Color c;
};

struct ColoredDistance CastRay(const struct Player p, float cast_angle);
