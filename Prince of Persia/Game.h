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
#include "Collision.h"

#include <windows.h>
#include <string.h>
#include <hash_map>
#include <cmath>

using std::hash_map;
using std::make_pair;
using std::string;


#define DEBUG 0


class Prince;

class Game {
public:
	Game (HWND hwnd, Input* in);
	void GameLoop();
	~Game() {}

	static Sprite* getSprite(string name);
	static void RegisterSprite(string name, string path = "Assets//");

private:
	// Util
	static void LoadAssets();
	void Reset();
	bool isGuardInScene(Character * guard_in);
	void EngageFight(Character * prince, Character * guard, Level * level);
	void SetEngagedGuard(Character* guard);

	// Flow Control
	void HandleInput();
	void ControlAI();
	void CheckCombatCollision();
	void CheckCollision();

	// Draw Functions
	void DrawGraphics();
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

	Timer timer;
	int timeSinceLastFrame;


	Collision* collision;
	Character* engagedGuard;
};
