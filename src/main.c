#include <raylib.h>
#include <math.h>

#include "player.h"
#include "window.h"

int main(void)
{
	InitWindow (WIDTH, HEIGHT, "DOOM TJOY LASTFM SPOTIFY");

	struct Player p;
	p.pos = (Vector2) {3.5f, 1.5f};
	p.angle = PI/4;
	float pvel = 1.0f;


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

		p.angle -= GetMouseDelta().x/WIDTH*PI;
		SetMousePosition(WIDTH/2, HEIGHT/2);
		HideCursor();

		if (p.angle < 0.0f)
			while(p.angle < 0.0f)
				p.angle += PI*2;
		if (p.angle > PI*2)
			while(p.angle > PI*2)
				p.angle -= PI*2;

		BeginDrawing();
			ClearBackground(BLUE);
			for (int i = 0; i < WIDTH/RECT_WIDTH; i++)
			{
				float iangle = (float) i * FOV / (((float)WIDTH/RECT_WIDTH - 1));
				float cast_angle = PI/180 * (iangle - (float) FOV/2);
				
				CastRay(p, cast_angle);
			}
		EndDrawing();

	}
	return 0;
}
