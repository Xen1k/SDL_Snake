#pragma once
#include <SDL.h>
#include "utils.h"

extern SDL_Renderer *renderer;
class GameObject
{
protected:
	SDL_Texture *texture;
public:
	int xPos = 0;
	int yPos = 0;
	GameObject() {}

	GameObject(int xPos, int yPos, std::string texturePath)
	{
		this->xPos = xPos * tileSize * sizeInc;
		this->yPos = yPos * tileSize * sizeInc;
		texture = LoadTexture(texturePath, renderer);
	}
	virtual void Render(int sizeInc_ = sizeInc)
	{
		RenderTexture(texture, renderer, xPos, yPos, sizeInc_);
	}
	virtual void Render(int xSize, int ySize, int sizeInc_ = sizeInc)
	{
		RenderTexture(texture, renderer,
			xPos,
			yPos,
			sizeInc, nullptr, xSize, ySize);
	}
	void SetRandomPostion()
	{
		xPos = rand() % (SCREEN_WIDTH / tileSize / sizeInc - 2) * tileSize * sizeInc + tileSize * sizeInc;
		yPos = rand() % (SCREEN_HEIGHT / tileSize / sizeInc - 2) * tileSize * sizeInc + tileSize * sizeInc;
	}
};