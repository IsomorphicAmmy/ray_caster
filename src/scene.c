#include "scene.h"

int scene[SCENE_WIDTH * SCENE_HEIGHT] = 
{
	1, 1, 1, 1, 1, 1,
	1, 0, 2, 0, 0, 1,
	1, 0, 2, 0, 0, 1,
	1, 0, 0, 0, 2, 1,
	1, 2, 0, 0, 0 ,1,
	1, 1, 1, 1, 1, 1
};

int GetSceneValue(int x, int y)
{
	return scene[y * SCENE_WIDTH + x];
}
