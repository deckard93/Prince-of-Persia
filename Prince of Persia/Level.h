#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Graphics.h"

#define DEBUG 1

enum direction { U, D, L, R }; 

struct levelBlock {
	//  decor: torch | bricks
	//  trap: guilotine | spikes | gate | potion
	//  on | off //in case of gates / spikes / guilotens
	//  dungeon: I | T | ] | [ | _ |   |
};


class Level {

//functions
public:
	Level() {}

	void changeScene(direction dir);

	void drawLevel();
	void changeScene();
	void loadLevel(int level);
	char getCode(int i, int j);

	~Level();
private:
							
//variales
public:

	static const int SCENE_WIDTH = 10;
	static const int SCENE_HEIGHT = 3;

	static const int MAX_LEVEL_WIDTH = 1024;
	static const int MAX_LEVEL_HEIGHT = 1024;


private:
	int scene_x;
	int scene_y;

	int level_width;
	int level_height;

	//level information
	char scene[32][32];
	char level[1024][1024]; //try to dynamically allocate this

	//Level Sprites
	Sprite block;
	Sprite blockCornerLeft;
	Sprite blockCornerRight;

	Sprite bricks;
	Sprite torch;

	Sprite separator;
	Sprite separatorCorner;

	Sprite floor;

	Sprite columnFront;
	Sprite columnBack;

	Sprite tileCornerLeft;

};