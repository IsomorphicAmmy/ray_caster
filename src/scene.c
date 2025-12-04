#include "scene.h"

int scene[SCENE_WIDTH * SCENE_HEIGHT];

int GetSceneValue(int x, int y)
{
	return scene[y * SCENE_WIDTH + x];
}
