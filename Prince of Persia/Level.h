#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "Entity.h"
#include "Graphics.h"
#include "Gate.h"


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

	int getBlockXByCoord(int yCoord);
	int getBlockYByCoord(int xCoord);

	void setCodeByCoord(int x, int y, char c);

	void drawLevel();
	void changeScene();
	void loadLevel(int level);
	void loadEntities();

	std::list<Entity>* getTorchEntities();
	std::list<Entity>* getPotionEntities();
	std::list<Entity>* getSpikeEntities();
	std::list<Entity>* getGuilotineEntities();
	std::list<Gate>* getGateEntities();
	
	char getCodeByBlock(int i, int j);
	char getCodeByCoord(int x, int y);

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

	std::list<Entity> *torchList;
	std::list<Entity> *potionList;
	std::list<Entity> *spikeList;
	std::list<Entity> *guilotineList;
	std::list<Gate> *gateList;

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
	static const int LEVEL_HEIGHT_PIX   = 126;
	static const int LEVEL_WIDTH_PIX    = 64;
	static const int LEVEL_HEIGHT_BLOCK	= 3;
	static const int LEVEL_WIDTH_BLOCK	= 10;
	static const int FOOT_FLOAT			= 11;
	static const int TORCH_FLOAT		= 125;
	static const int TORCH_FLOAT_LEFT	= 20;

};