#pragma once
#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "Time.h"
#include "settings.h"
#include "segment.h"
#include <vector>
#include "gameObject.h"

using namespace std;

class Snake 
{
public:
	vector<Segment> segments;

	Snake()
	{
		segments.push_back(Segment(5, 5));
	}

	void AddSegment()
	{
		int xTile = segments[segments.size() - 1].xPos;
		int yTile = segments[segments.size() - 1].yPos;
		segments.push_back(Segment(xTile - tileSize * sizeInc, yTile));
	}

	void ResetSnake()
	{
		segments.clear();
		segments.push_back(Segment(5, 5));
	}

	bool CheckCollision(GameObject *object)
	{
		if (object->xPos == segments[0].xPos && object->yPos == segments[0].yPos)
			return true;
		return false;
	}

	void MoveHorizontal(int direction)
	{
		for (int i = segments.size() - 1; i >= 1; i--)
			segments[i].xPos = (int)segments[i - 1].xPos;
		segments[0].MoveHorizontal(direction);

		for (int i = 0; i < segments.size(); i++)
		{
			if (segments[i].xPos < 0)
			{
				segments[i].xPos = SCREEN_WIDTH - tileSize * sizeInc;
				break;
			}
		}
		for (int i = 0; i < segments.size(); i++)
		{
			if (segments[i].xPos > SCREEN_WIDTH)
			{
				segments[i].xPos = 0;
				break;
			}
		}
	}
	void MoveVertical(int direction)
	{
		for (int i = segments.size() - 1; i >= 1; i--)
			segments[i].yPos = (int)segments[i - 1].yPos;
		segments[0].MoveVertical(direction);
		for (int i = 0; i < segments.size(); i++)
		{
			if (segments[i].yPos < 0)
			{
				segments[i].yPos = SCREEN_HEIGHT - tileSize * sizeInc;
				break;
			}
		}
		for (int i = 0; i < segments.size(); i++)
		{
			if (segments[i].yPos > SCREEN_HEIGHT)
			{
				segments[i].yPos = 0;
				break;
			}
		}	
	}

	void Render()
	{
		for (auto segment : segments)
			segment.Render();
	}
};