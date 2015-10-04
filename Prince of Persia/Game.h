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

#define LEVEL_HEIGHT_PIX	126
#define LEVEL_WIDTH_PIX		64
#define LEVEL_HEIGHT_BLOCK	3
#define LEVEL_WIDTH_BLOCK	10
#define FOOT_FLOAT			11
#define TORCH_FLOAT			125
#define TORCH_FLOAT_LEFT	20


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


	//Level Sprites
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
	static const int TOP_MARGIN = (Graphics::SCREENY - LEVEL_HEIGHT_PIX * LEVEL_HEIGHT_BLOCK) / 2;
	static const int LEFT_MARGIN = (Graphics::SCREENX - LEVEL_WIDTH_PIX * LEVEL_WIDTH_BLOCK) / 2;

};