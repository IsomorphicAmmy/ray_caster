#pragma once

#include <raylib.h>

#define FOV 75

struct Player 
{
	Vector2 pos;
	float angle;
};

void CastRay(const struct Player p, float cast_angle);
