#include "raylib.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 1280
#define HEIGHT 720

#define SCENE_WIDTH 6
#define SCENE_HEIGHT 6
#define RECT_WIDTH 1

#define FOV 75

typedef struct Vector2f
{
	float x;
	float y;
} Vector2f;

int scene[SCENE_WIDTH*SCENE_HEIGHT] = 
{
	1, 1, 1, 1, 1, 1,
	1, 0, 2, 0, 0, 1,
	1, 0, 2, 0, 0, 1,
	1, 0, 0, 0, 2, 1,
	1, 2, 0, 0, 0 ,1,
	1, 1, 1, 1, 1, 1
};

struct Player 
{
	Vector2f pos;
	float angle;
};

typedef struct ColoredDistance
{
	float d;
	Color color;
} ColoredDistance;

ColoredDistance CastRayFromPlayer(struct Player p, float cast_angle)
{
	ColoredDistance result;
	float rx, ry, yo, xo;
	int dof = 0;
	float px = p.pos.x, py = p.pos.y;
	float ra = p.angle + cast_angle;
	while (ra < 0)
		ra += PI*2;
	while (ra >= PI*2)
		ra -= PI*2;

	if (ra < PI) {ry = floor(py) - 0.00001f; yo = -1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	if (ra > PI) {ry = floor(py) + 1.00001f; yo =  1.0f; rx = px - (ry - py)/tan(ra); xo = -yo/tan(ra);}
	if (ra == 0 || ra == PI) {rx = px; ry = py; dof = 8;}
	while(dof < 8)
	{
		if (rx * ry >= 0.0f && ry * SCENE_WIDTH + rx < SCENE_WIDTH*SCENE_HEIGHT)
		{
			int block = scene[(int) ry * SCENE_WIDTH + (int) rx];
			if (block == 1) {result.color = WHITE; dof = 8;}
			else if (block == 2) {result.color = RED; dof = 8;}
			else 
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}
		else
			dof = 8;
	}
	float ycx = rx, ycy = ry;
	float yd = sqrt((py-ry)*(py-ry) + (px-rx)*(px-rx));
	dof = 0;

	if (ra < PI/2 || ra > 3*PI/2) {rx = floor(px) + 1.00001f; xo =  1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	if (ra > PI/2 && ra < 3*PI/2) {rx = floor(px) - 0.00001f; xo = -1.0f; ry = (px - rx)*tan(ra) + py; yo = -xo*tan(ra);}
	if (ra == PI/2 || ra == 3*PI/2) {rx = px; ry = py; dof = 8;}
	while(dof < 8)
	{
		if (rx * ry >= 0.0f && ry * SCENE_WIDTH + rx < SCENE_WIDTH*SCENE_HEIGHT)
		{
			int block = scene[(int) ry * SCENE_WIDTH + (int) rx];
			if (block != 0) dof = 8;
			else 
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}
		else
			dof = 8;
	}
	float xcx = rx, xcy = ry;
	float xd = sqrt((py-ry)*(py-ry) + (px-rx)*(px-rx));

	if (cast_angle < 0.0f)
		cast_angle = -cast_angle;

	if (yd > xd)
	{
		int block = scene[(int) ry * SCENE_WIDTH + (int) rx];
		if (block == 1)
			result.color = WHITE;
		else if (block == 2)
			result.color = RED;
		result.d = xd * cos(cast_angle);
	}
	else 
	{
		result.d = yd * cos(cast_angle);
	}
	
	return result;
}

int main(void)
{
	InitWindow (WIDTH, HEIGHT, "DOOM TJOY LASTFM SPOTIFY");

	struct Player p;
	p.pos = (Vector2f) {3.5f, 1.5f};
	p.angle = PI/4;
	float pvel = 1.0f;
	float pangvel = 3*PI/4;


	while (!WindowShouldClose())
	{
		system("clear");
		float dt = GetFrameTime();
		if (IsKeyDown(KEY_UP))
		{
			p.pos = (Vector2f) {p.pos.x + pvel * cos(p.angle) * dt, p.pos.y - pvel * sin(p.angle) * dt};
		}
		if (IsKeyDown(KEY_DOWN))
		{
			p.pos = (Vector2f) {p.pos.x - pvel * cos(p.angle) * dt, p.pos.y + pvel * sin(p.angle) * dt};
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			p.angle -= pangvel * dt;
			if (p.angle < 0.0f)
				while(p.angle < 0.0f)
					p.angle += PI*2;
		}
		if (IsKeyDown(KEY_LEFT))
		{
			p.angle += pangvel * dt;
			if (p.angle > PI*2)
				while(p.angle > PI*2)
					p.angle -= PI*2;
		}

		BeginDrawing();
			ClearBackground(BLACK);
			for (int i = 0; i < WIDTH/RECT_WIDTH; i++)
			{
				float iangle = (float) i * FOV / (((float)WIDTH/RECT_WIDTH - 1));
				float cast_angle = PI/180 * (iangle - (float) FOV/2);
				
				ColoredDistance cd = CastRayFromPlayer(p, cast_angle);

				int rect_height = HEIGHT/cd.d; if (rect_height > HEIGHT) rect_height = HEIGHT;
				DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), (HEIGHT - rect_height)/2, RECT_WIDTH, rect_height, cd.color);
			}
		EndDrawing();
	}

	

	return 0;
}
