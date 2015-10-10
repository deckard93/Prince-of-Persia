#pragma once

#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "Benchmark.h"
#include "Audio.h"
#include "Entity.h"
#include "Prince.h"
#include "Level.h"

#include <windows.h>
#include <string.h>

#define DEBUG 0

class Prince;

class Game {
public:
	Game (HWND hwnd, Input* in);
	void GameLoop();
	~Game();

private:
	//control
	void HandleInput();
	void ControlAI();
	void CheckCollision();
	void DrawGraphics();

	//Update Frame
	void ComposeFrame();
	void DrawLevel();
	void DrawBackground();
	void DrawForeground();
	void DrawHealth();

private:
	HWND		winHandle;
	Graphics	graphics;
	Input*		input;
	//Audio		audio;
	//Benchmark	benchmark;

private:
	Level *level;

	//Level Data
	int TOP;
	int BOTTOM;

	int LEFT;
	int RIGHT;	

	Sprite block;
	Sprite blockCornerLeft;
	Sprite blockCornerRight;

	Sprite bricks;
	Sprite torch;

	Sprite separator;
	Sprite separatorCorner;

	Sprite floor;

	Sprite healthFull;
	Sprite healthEmpty;

	Sprite columnFront;
	Sprite columnBack;

	Sprite gateFrameFront;
	Sprite gateFrameBack;

	Sprite rubble_front;
	Sprite rubble_back;

	Sprite deadSk;
	Sprite holyFloor;
	Sprite gate;
	Sprite tileCornerLeft;

	int dummy;

	//entities
	Entity* torchEntity;
	SpriteSheet* torchSheet;
	Prince* prince;

	Sprite test;

	D3DCOLOR fontSurf[512 * 84];
	Font fixedSys;
	Sprite sp;

public:

	static const int TOP_MARGIN = (Graphics::SCREENY - Level::LEVEL_HEIGHT_PIX * Level::LEVEL_HEIGHT_BLOCK) / 2;
	static const int LEFT_MARGIN = (Graphics::SCREENX - Level::LEVEL_WIDTH_PIX * Level::LEVEL_WIDTH_BLOCK) / 2;

};
