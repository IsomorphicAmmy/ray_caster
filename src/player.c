#include <math.h>
#include <raylib.h>

#include "player.h"
#include "scene.h"
#include "window.h"


void StepRay(float* rx, float* ry, float yo, float xo)
{
	int dof;
	while(dof < 8)
	{
		if (((int) *rx) * ((int) *ry) >= 0.0f && *ry * SCENE_WIDTH + *rx < SCENE_WIDTH*SCENE_HEIGHT)
		{
			int block = GetSceneValue(*rx, *ry);
			if (block != 0) { break; }
			else 
			{
				*rx += xo;
				*ry += yo;
				dof++;
			}
		}
		else
			break;
	}
}

void CastRay(struct Player p, float cast_angle)
{
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
		StepRay(&rx, &ry, yo, xo);
		int block = GetSceneValue((int) rx, (int) ry);
		if(block == 1)
			yc = WHITE;
		else if(block == 2)
			yc = RED;
	}
	else {
		rx = px;
		ry = py;
		yc = BLUE;
	}
	
	float yd = sqrt((py-ry)*(py-ry) + (px-rx)*(px-rx));

	if (ra < PI/2 || ra > 3*PI/2) {rx = floor(px) + 1.00001f; xo =  1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	if (ra > PI/2 && ra < 3*PI/2) {rx = floor(px) - 0.00001f; xo = -1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}

	if(ra != PI/2 && ra != 3*PI/2)
	{
		StepRay(&rx, &ry, yo, xo);
		int block = GetSceneValue((int) rx, (int) ry);
		if(block == 1)
			xc = WHITE;
		else if(block == 2)
			xc = RED;
	}
	else {
		rx = px;
		ry = py;
		xc = BLUE;
	}

	float xd = sqrt((py-ry)*(py-ry) + (px-rx)*(px-rx));

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

	int i = ((cast_angle*180/PI + (float)FOV/2.0f) * ((float) WIDTH/RECT_WIDTH - 1.0f))/FOV;

	int rect_height = HEIGHT/d; if (rect_height > HEIGHT) rect_height = HEIGHT;
	DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), 0, RECT_WIDTH, (HEIGHT - rect_height)/2 + 1, BLUE);
	DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), (HEIGHT - rect_height)/2 - 1, RECT_WIDTH, rect_height + 1, ray_color);
	DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), HEIGHT - (HEIGHT - rect_height)/2 - 1, RECT_WIDTH, (HEIGHT - rect_height)/2, (Color) {127, 127, 127, 255});
}
