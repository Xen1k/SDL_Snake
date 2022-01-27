#pragma once

#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "Time.h"
#include "snake.h"
#include "gameObject.h"

using namespace std;


extern Snake *snake;
extern SDL_Renderer *renderer;

class Food : public GameObject
{
public:

	Food()
	{
		xPos = (rand() % (SCREEN_WIDTH / tileSize / sizeInc) - 2) * tileSize * sizeInc + tileSize * sizeInc;
		yPos = (rand() % (SCREEN_HEIGHT / tileSize / sizeInc) - 2) * tileSize * sizeInc + tileSize * sizeInc;
		texture = LoadTexture("./food.png", renderer);
	}

};