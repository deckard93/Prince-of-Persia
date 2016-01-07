#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <map>

#include "Graphics.h"
#include "Gate.h"
#include "Spikes.h"

using std::pair;

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

	int getSceneBlockXByCoord(int yCoord);
	int getSceneBlockYByCoord(int xCoord);

	int getLevelBlockX(int x);
	int getLevelBlockY(int y);

	void setCodeByCoord(int x, int y, char c);

	void drawLevel();
	void changeScene();
	void loadLevel(int level);
	void loadEntities();

	std::map<pair<int, int>, Entity*>* getEntities();
	std::map<pair<int, int>, pair<int, int> >* getMec();
	
	char getLevelCodeByBlock(int x, int y);

	char getSceneCodeByBlock(int y, int x);
	char getSceneCodeByCoord(int x, int y);

	int getLevelHeight();
	int getLevelWidth();

	bool isFreeSpace(char c);


	bool findSpikes(int block_x, int block_y);


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

	std::map<pair<int, int>, Entity*>* entities;
	std::map<pair<int, int>, pair<int, int> >* mechanism;

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

public:
	static const int BLOCK_HEIGHT_PX   = 126;
	static const int BLOCK_WIDTH_PX    = 64;
	static const int SCENE_HEIGHT_BLK	= 3;
	static const int SCENE_WIDTH_BLK	= 10;

	static const int FOOT_FLOAT			= 11;

	static const int TORCH_OFFSET_Y = 125;
	static const int TORCH_OFFSET_X = 20;

	static const int POTION_OFFSET_X = 40;
	static const int POTION_OFFSET_Y = 60;

	static const int SPIKE_OFFSET_X = 0;
	static const int SPIKE_OFFSET_Y = 26;

	static const int SWORD_OFFSET_X = 0;
	static const int SWORD_OFFSET_Y = 95;


};
