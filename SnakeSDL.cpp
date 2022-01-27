#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "Time.h"
#include "snake.h"
#include "food.h"
#include <ctime>
#include "gameObject.h"
#include "segment.h"
#include <string>
#include "fileManager.h"

using namespace std;

FileWithRecords *records = nullptr;

SDL_Event e;

SDL_Window *window;
SDL_Renderer *renderer;

Snake *snake;

vector<Segment*> borders;

GameObject *food;

const int speed = 50;

int horizontalMoveDirection = 1, vercticalMoveDirection = 0;

double skippingFrames = 0;

bool bordersEnabled = false;

enum Scene { Menu, Game, InputName };

Scene currentScene = InputName;

GameObject *menuImage, *tutorialImage, *tutorialButton;
vector<GameObject*> bestPlayers;
SDL_Color textColor = { 255, 255, 255 };
TTF_Font *font;

int points = 0;

int mx, my;
string playerName = "_";

bool SkipFrames(int framesToSkip)
{
	if (framesToSkip < skippingFrames)
	{
		skippingFrames = 0;
		return true;
	}
	else
	{
		skippingFrames += Time::GetDeltaTime() * 1000;
		return false;
	}
}

void Render()
{
	food->Render();
	snake->Render();
	if (bordersEnabled)
		for (int i = 0; i < borders.size(); i++)
		{
			borders[i]->Render();
		}
}

int GameScene();
int MenuScene();
bool InputNameScene();

bool Update()
{
	Time::SetNow(SDL_GetPerformanceCounter());
	SDL_RenderClear(renderer);

	if (currentScene == InputName && !InputNameScene())
		return false;
	if (currentScene == Menu && !MenuScene())
		return false;
	else if (currentScene == Game && !GameScene())
		return false;
	

	SDL_RenderPresent(renderer);
	Time::Calculate();
	return true;
}

bool InputNameScene()
{
	RenderText("Input your name: (length < 5)", font, textColor, 28 * sizeInc, renderer, 17, 2,
		SCREEN_WIDTH / 2 - 17 * 9,
		SCREEN_HEIGHT / 4);
	RenderText(playerName, font, textColor, 28 * sizeInc, renderer, playerName.length(), 2,
		SCREEN_WIDTH / 2 - playerName.length() * 9,
		SCREEN_HEIGHT / 2);

	string navText = "SPACE - continue";

	RenderText(navText, font, textColor, 28 * sizeInc, renderer, navText.size(), 2,
		SCREEN_WIDTH / 2 - navText.size() * 9,
		SCREEN_HEIGHT - SCREEN_HEIGHT / 4);
	
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
			{
				playerName = "_";
				break;
			}
			case SDLK_SPACE:
			{
				if (playerName.size() < 5)
				{
					if (playerName == "_")
						playerName = "Unkwn";
					currentScene = Menu;
				}
				else
					playerName = "_";
				break;
			}
			}
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			if (playerName == "_")
				playerName = "";
			playerName += e.text.text;
		}

		if (e.type == SDL_QUIT)
			return false;
	}

	return true;
}

void ResetGame()
{
	records->SetNewRecord(points, playerName);
	points = 0;
	snake->ResetSnake();
	horizontalMoveDirection = 1;
	vercticalMoveDirection = 0;
	food->SetRandomPostion();
}



int value = 0;

bool tutorialIsActive = false;
int MenuScene()
{
	SDL_GetMouseState(&mx, &my);


	menuImage->Render(27);
	if(tutorialIsActive)
		tutorialImage->Render(27);

	string recordString;

	for (int i = 0; i < bestPlayers.size(); i++)
	{
		recordString = records->GetRecordString(i);

		bestPlayers[i]->Render(5, 2, 4);

		RenderText(recordString, font, textColor, 28 * sizeInc, renderer, recordString.length() / 1.5, 2,
			20,
			(i * 2 + 5) * tileSize * sizeInc + tileSize * sizeInc / 2.2);
	}

	tutorialButton->Render(4);
	RenderText("?", font, textColor, 28 * sizeInc, renderer, 1, 2, tileSize * 4 / 2.5, (6 * 2 + 5) * tileSize * sizeInc + tileSize * sizeInc / 2.5);
	

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{

			switch (e.key.keysym.sym)
			{
			case SDLK_q:
			{
				
				currentScene = Game;
				bordersEnabled = true;
				break;
			}
			case SDLK_e:
			{
				currentScene = Game;
				bordersEnabled = false;
				break;
			}
			case SDLK_ESCAPE:
			{
				return false;
				break;
			}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if(mx > tutorialButton->xPos && mx < tutorialButton->xPos + tileSize * sizeInc * 2 
				&& my > tutorialButton->yPos && my < tutorialButton->yPos + tileSize * sizeInc * 2 && !tutorialIsActive)
				tutorialIsActive = true;
			else
				tutorialIsActive = false;

		}

		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}

	return true;
}



int GameScene()
{

	Render();

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{

			switch (e.key.keysym.sym)
			{
			case SDLK_w:
			{

				if (vercticalMoveDirection != -1)
				{
					horizontalMoveDirection = 0;
					vercticalMoveDirection = 1;
				}
				break;
			}
			case SDLK_a:
			{
				if (horizontalMoveDirection != 1)
				{
					horizontalMoveDirection = -1;
					vercticalMoveDirection = 0;
				}
				break;
			}

			case SDLK_s:
			{
				if (vercticalMoveDirection != 1)
				{
					horizontalMoveDirection = 0;
					vercticalMoveDirection = -1;
				}
				break;
			}
			case SDLK_d:
			{
				if (horizontalMoveDirection != -1)
				{
					horizontalMoveDirection = 1;
					vercticalMoveDirection = 0;
				}
				break;
			}
			case SDLK_SPACE:
			{
				snake->AddSegment();

				break;
			}
			case SDLK_ESCAPE:
			{
				ResetGame();
				currentScene = Menu;
				break;
			}
			}
		}
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}
	if (snake->CheckCollision(food))
	{
		snake->AddSegment();
		food->SetRandomPostion();
		points++;
	}
	for (int i = 1; i < snake->segments.size(); i++)
	{
		if (snake->segments[i].xPos == snake->segments[0].xPos && snake->segments[i].yPos == snake->segments[0].yPos) {
			ResetGame();
			currentScene = Menu;
		}

	}

	if (bordersEnabled)
		for (int i = 0; i < borders.size(); i++)
		{
			if (borders[i]->xPos == snake->segments[0].xPos && borders[i]->yPos == snake->segments[0].yPos) {
				ResetGame();
				currentScene = Menu;
			}

		}

	if (SkipFrames(speed)) {
		snake->MoveHorizontal(horizontalMoveDirection);
		snake->MoveVertical(vercticalMoveDirection);
	}

	return true;
}

int main(int argc, char** argv)
{
	srand(time(0));
	TTF_Init();
	window = CreateWindow();
	renderer = CreateRenderer(window);
	font = TTF_OpenFont("./progressPixel.ttf", 40);

	snake = new Snake();

	records = new FileWithRecords;

	food = new Food();

	for(int i = 0; i < 3; i++)
		bestPlayers.push_back(new GameObject(0, i * 2 + 5, "./best.png"));

	tutorialButton = new GameObject(0, 6 * 2 + 5, "./best.png");
	//Fill borders
	for (int i = 1; i < SCREEN_WIDTH / tileSize / sizeInc - 1; i++)
	{
		borders.push_back(new Segment(i, 0, "./border.png"));
		borders.push_back(new Segment(i, SCREEN_HEIGHT / tileSize / sizeInc - 1, "./border.png"));
	}
	for (int i = 0; i < SCREEN_WIDTH / tileSize / sizeInc; i++)
	{
		borders.push_back(new Segment(0, i, "./border.png"));
		borders.push_back(new Segment(SCREEN_WIDTH / tileSize / sizeInc - 1, i, "./border.png"));
	}

	menuImage = new GameObject(5, 5, "./menu.png");
	tutorialImage = new GameObject(5, 5, "./tutorial.png");

	while (Update());

	return 0;
}
