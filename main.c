#include "raylib.h"
#include <math.h>

#define WIDTH 960
#define HEIGHT 480
#define PADDING 1

#define SCENE_WIDTH 6
#define SCENE_HEIGHT 6
#define RECT_WIDTH 10

#define BLOCK_WIDTH WIDTH/SCENE_WIDTH/2
#define BLOCK_HEIGHT HEIGHT/SCENE_HEIGHT
#define PBWIDTH BLOCK_WIDTH - 2 * PADDING
#define PBHEIGHT BLOCK_HEIGHT - 2 * PADDING

#define FOV 90


int scene[SCENE_WIDTH*SCENE_HEIGHT] = 
{
	1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 1,
	1, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 1,
	1, 2, 0, 0, 0 ,1,
	1, 1, 1, 1, 1, 1
};

struct Player 
{
	Vector2 pos;
	float angle;
};

void DrawPlayer(struct Player p)
{
	Vector2 player_real_pos = {p.pos.x * BLOCK_WIDTH, p.pos.y * BLOCK_HEIGHT};
	DrawCircleV(player_real_pos, 10, YELLOW);
	DrawLineV(player_real_pos, (Vector2) {player_real_pos.x + 20 * cos(p.angle), player_real_pos.y + 20 * sin(p.angle)}, YELLOW);
}

int main(void)
{
	InitWindow (WIDTH, HEIGHT, "DOOM TJOY LASTFM SPOTIFY");

	struct Player p;
	p.pos = (Vector2) {3.0f, 3.0f};
	p.angle = 0.0f;
	float pvel = 1.0f;
	float pangvel = 3*PI/4;


	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		if (IsKeyDown(KEY_UP))
		{
			p.pos = (Vector2) {p.pos.x + pvel * cos(p.angle) * dt, p.pos.y + pvel * sin(p.angle) * dt};
		}
		if (IsKeyDown(KEY_DOWN))
		{
			p.pos = (Vector2) {p.pos.x - pvel * cos(p.angle) * dt, p.pos.y - pvel * sin(p.angle) * dt};
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			p.angle += pangvel * dt;
		}
		if (IsKeyDown(KEY_LEFT))
		{
			p.angle -= pangvel * dt;
		}

		BeginDrawing();
			ClearBackground(BLACK);

			for (int y = 0; y < SCENE_HEIGHT; y++)
			{
				for (int x = 0; x < SCENE_WIDTH; x++)
				{
					int scene_value = scene[y * SCENE_WIDTH + x];
					int padded_x = x * BLOCK_WIDTH + PADDING;
					int padded_y = y * BLOCK_HEIGHT + PADDING;

					if (scene_value == 0)
						DrawRectangle(padded_x, padded_y, PBWIDTH, PBHEIGHT, GRAY);
					else if (scene_value == 1)
						DrawRectangle(padded_x, padded_y, PBWIDTH, PBHEIGHT, WHITE);
					else if (scene_value == 2)
						DrawRectangle(padded_x, padded_y, PBWIDTH, PBHEIGHT, RED);
				}
			}
			
			DrawPlayer(p);
		EndDrawing();
	}

	

	return 0;
}
