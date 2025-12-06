#include <math.h>
#include <raylib.h>
#include <stdbool.h>

#include "player.h"
#include "scene.h"
#include "util.h"

float distance(float x0, float y0, float x1, float y1)
{
	return sqrt((x0-x1)*(x0-x1) + (y0-y1)*(y0-y1));
}

bool StepRay(float* rx, float* ry, float yo, float xo)
{
	int dof = 0;
	while(dof < 8)
	{
		if ((*rx) * (*ry) >= 0.0f && (*ry) * SCENE_WIDTH + (*rx) < SCENE_WIDTH*SCENE_HEIGHT)
		{
			int block = GetSceneValue((int) *rx, (int) *ry);
			if (block != 0) { return true; }
			else 
			{
				*rx += xo;
				*ry += yo;
				dof++;
			}
		}
		else
			return false;
	}
	return false;
}

struct ColoredDistance GetRayYCollision(float ra, float px, float py)
{
	struct ColoredDistance result;
	bool colided;
	float yo, xo, rx, ry;
	if (ra < PI) {ry = floor(py) - 0.00001f; yo = -1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	if (ra > PI) {ry = floor(py) + 1.00001f; yo =  1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	
	if(ra != 0.0f && ra != PI)
	{
		colided = StepRay(&rx, &ry, yo, xo);
		if(colided)
		{
			int block = GetSceneValue((int) rx, (int) ry);
			if(block == 1)
				result.c = WHITE;
			else if(block == 2)
				result.c = RED;
		}
	}
	else {
		rx = px;
		ry = py;
		result.c = BLUE;
	}
	result.d = distance(px, py, rx, ry);
	return result;
}

struct ColoredDistance GetRayXCollision(float ra, float px, float py)
{
	struct ColoredDistance result;
	bool colided;
	float yo, xo, rx, ry;
	if (ra < PI/2 || ra > 3*PI/2) {rx = floor(px) + 1.00001f; xo =  1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	if (ra > PI/2 && ra < 3*PI/2) {rx = floor(px) - 0.00001f; xo = -1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	
	if(ra != PI/2 && ra != 3*PI/2)
	{
		colided = StepRay(&rx, &ry, yo, xo);
		if(colided)
		{
			int block = GetSceneValue((int) rx, (int) ry);
			if(block == 1)
				result.c = WHITE;
			else if(block == 2)
				result.c = RED;
		}
	}
	else {
		rx = px;
		ry = py;
		result.c = BLUE;
	}
	result.d = distance(px, py, rx, ry);
	return result;
}

struct ColoredDistance CastRay(struct Player p, float cast_angle)
{
	float d;
	float px = p.pos.x, py = p.pos.y;
	float ra = p.angle + cast_angle;
	Color ray_color;
	FixAngle(&ra);
	
	struct ColoredDistance ycd = GetRayYCollision(ra, px, py);
	struct ColoredDistance xcd = GetRayXCollision(ra, px, py);

	ModF(&cast_angle);

	if (ycd.d > xcd.d)
	{
		ray_color = xcd.c;
		d = xcd.d * cos(cast_angle);
		ray_color = (Color) {xcd.c.r * 0.9, xcd.c.g * 0.9, xcd.c.b * 0.9, xcd.c.a};
	}
	else 
	{
		ray_color = ycd.c;
		d = ycd.d * cos(cast_angle);
		ray_color = (Color) {ycd.c.r * 0.7, ycd.c.g * 0.7, ycd.c.b * 0.7, ycd.c.a};
	}

	return (struct ColoredDistance) { d, ray_color };
}
