#include <math.h>
#include <raylib.h>
#include <stdbool.h>

#include "player.h"
#include "scene.h"
#include "window.h"

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

struct ColoredDistance CastRay(struct Player p, float cast_angle)
{
	bool colided;
	float d;
	float rx, ry, yo, xo;
	float px = p.pos.x, py = p.pos.y;
	float ra = p.angle + cast_angle;
	Color ray_color;
	Color yc, xc;

	while (ra < 0)
		ra += PI*2;
	while (ra >= PI*2)
		ra -= PI*2;

	if (ra < PI) {ry = floor(py) - 0.00001f; yo = -1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	if (ra > PI) {ry = floor(py) + 1.00001f; yo =  1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	
	if(ra != 0 && ra != PI)
	{
		colided = StepRay(&rx, &ry, yo, xo);
		if(colided)
		{
			int block = GetSceneValue((int) rx, (int) ry);
			if(block == 1)
				yc = WHITE;
			else if(block == 2)
				yc = RED;
		}
	}
	else {
		rx = px;
		ry = py;
		yc = BLUE;
	}
	
	float yd = distance(px, py, rx, ry);

	if (ra < PI/2 || ra > 3*PI/2) {rx = floor(px) + 1.00001f; xo =  1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	if (ra > PI/2 && ra < 3*PI/2) {rx = floor(px) - 0.00001f; xo = -1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}

	if(ra != PI/2 && ra != 3*PI/2)
	{
		colided = StepRay(&rx, &ry, yo, xo);
		if(colided)
		{
			int block = GetSceneValue((int) rx, (int) ry);
			if(block == 1)
				xc = WHITE;
			else if(block == 2)
				xc = RED;
		}
	}
	else {
		rx = px;
		ry = py;
		xc = BLUE;
	}

	float xd = distance(px, py, rx, ry);

	if (cast_angle < 0.0f)
		cast_angle = -cast_angle;

	if (yd > xd)
	{
		ray_color = xc;
		d = xd * cos(cast_angle);
		ray_color = (Color) {ray_color.r * 0.9, ray_color.g * 0.9, ray_color.b * 0.9, ray_color.a};
	}
	else 
	{
		ray_color = yc;
		d = yd * cos(cast_angle);
		ray_color = (Color) {ray_color.r * 0.7, ray_color.g * 0.7, ray_color.b * 0.7, ray_color.a};
	}

	return (struct ColoredDistance) { d, ray_color };
}
