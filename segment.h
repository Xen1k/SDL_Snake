#pragma once
#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "Time.h"
#include "settings.h"
#include "gameObject.h"

using namespace std;

extern SDL_Renderer *renderer;
class Segment : public GameObject
{
private:
	double speed = 4;

public:
	Segment(int xPos, int yPos, string texturePath = "./block.png")
	{
		this->xPos = xPos * tileSize * sizeInc;
		this->yPos = yPos * tileSize * sizeInc;
		texture = LoadTexture(texturePath, renderer);
	}


	void MoveHorizontal(int direction)
	{
		xPos += direction * tileSize * sizeInc;
	}
	void MoveVertical(int direction)
	{
		yPos -= direction * tileSize * sizeInc;
	}
};