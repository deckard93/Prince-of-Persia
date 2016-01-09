#pragma once

#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "Benchmark.h"
#include "Audio.h"
#include "Entity.h"
#include "Prince.h"
#include "Level.h"
#include "Gate.h"
#include "Spikes.h"
#include "Guilotine.h"

#include <windows.h>
#include <string.h>
#include <hash_map>


using std::hash_map;
using std::make_pair;
using std::string;


#define DEBUG 1


class Prince;

class Game {
public:
	Game (HWND hwnd, Input* in);
	void GameLoop();
	~Game();

	static Sprite* getSprite(string name);
	static void RegisterSprite(string name, string path = "Assets//");

private:
	//util
	static void LoadAssets();
	void Reset();

	//control
	void HandleInput();
	void ControlAI();
	void CheckCollision();
	void DrawGraphics();

	//Update Frame
	void ComposeFrame();
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
	static hash_map<string, Sprite*>* assets;

	Level *level;
	Prince* prince;

	D3DCOLOR fontSurf[512 * 84];
	Font fixedSys;

public:

	//static const int TOP_MARGIN = (Graphics::SCREENY - Level::BLOCK_HEIGHT_PX * Level::SCENE_HEIGHT_BLK) / 2;
	//static const int LEFT_MARGIN = (Graphics::SCREENX - Level::BLOCK_WIDTH_PX * Level::SCENE_WIDTH_BLK) / 2;

};
