#include <raylib.h>
#include <math.h>

#include "player.h"
#include "window.h"
#include "scene.h"
#include "util.h"

int main(void)
{
	InitWindow (WIDTH, HEIGHT, "DOOM TJOY LASTFM SPOTIFY");
	SetTargetFPS(60);

	struct Player p;
	p.pos = (Vector2) {3.5f, 1.5f};
	p.angle = PI/4.0f;
	float pvel = 1.0f;

	Vector2 p_last_pos = p.pos;

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		if (IsKeyDown(KEY_W))
			p.pos = (Vector2) {p.pos.x + pvel * cos(p.angle) * dt, p.pos.y - pvel * sin(p.angle) * dt};
		if (IsKeyDown(KEY_S))
			p.pos = (Vector2) {p.pos.x - pvel * cos(p.angle) * dt, p.pos.y + pvel * sin(p.angle) * dt};
		if (IsKeyDown(KEY_A))
			p.pos = (Vector2) {p.pos.x - pvel * sin(p.angle) * dt, p.pos.y - pvel * cos(p.angle) * dt};
		if (IsKeyDown(KEY_D))
			p.pos = (Vector2) {p.pos.x + pvel * sin(p.angle) * dt, p.pos.y + pvel * cos(p.angle) * dt};

		//Collision check
		while (GetSceneValue((int) floor(p.pos.x), (int) floor(p.pos.y)) != 0)
		{
			if (floor(p_last_pos.x) == floor(p.pos.x) - 1)
				p.pos.x = floor(p.pos.x) - 0.001f;
			else if (floor(p_last_pos.x) == floor(p.pos.x) + 1)
				p.pos.x = floor(p.pos.x) + 1.001f;
			else if (floor(p_last_pos.y) == floor(p.pos.y) - 1)
				p.pos.y = floor(p.pos.y) - 0.001f;
			else if (floor(p_last_pos.y) == floor(p.pos.y) + 1)
				p.pos.y = floor(p.pos.y) + 1.001f;
		}

		p_last_pos = p.pos;

		p.angle -= GetMouseDelta().x/WIDTH*PI;
		SetMousePosition(WIDTH/2, HEIGHT/2);
		HideCursor();

		FixAngle(&p.angle);

		BeginDrawing();
			ClearBackground(BLUE);
			for (int i = 0; i < WIDTH/RECT_WIDTH; i++) //Casts rays from right to left
			{
				float iangle = (float) i * FOV / (((float)WIDTH/RECT_WIDTH - 1.0f));
				float cast_angle = PI/180 * (iangle - (float) FOV/2);
				struct ColoredDistance cd = CastRay(p, cast_angle);

				int rect_height = HEIGHT/cd.d; if (rect_height > HEIGHT) rect_height = HEIGHT;
				DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), 0, RECT_WIDTH, (HEIGHT - rect_height)/2 + 1, BLUE);
				DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), (HEIGHT - rect_height)/2 - 1, RECT_WIDTH, rect_height + 1, cd.c);
				DrawRectangle(WIDTH - RECT_WIDTH * (i + 1), HEIGHT - (HEIGHT - rect_height)/2 - 1, RECT_WIDTH, (HEIGHT - rect_height)/2, (Color) {127, 127, 127,255});
			}
			DrawRectangle(0, 0, MeasureText(TextFormat("FPS: %d", GetFPS()), 20) + 20, 40, (Color){0, 0, 0, 120});
			DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, WHITE);
		EndDrawing();

	}
	return 0;
}
