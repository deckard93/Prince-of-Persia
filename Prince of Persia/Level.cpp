#include "Level.h"
#include "Game.h"

void Level::loadLevel(int l) {

	std::string levelNo = static_cast<std::ostringstream*>( &(std::ostringstream() << l) )->str();

	std::string filePath = "Levels/"; 
	filePath += "level";
	filePath += levelNo;
	filePath += ".txt";

	//open file
	FILE *file = fopen(filePath.c_str(), "rb");
	
	//set initial scene
	fscanf (file, "%d", &scene_x);
	fscanf (file, "%d", &scene_y);
	
	fscanf (file, "%d", &level_height);
	fscanf (file, "%d", &level_width);

	char c;

	//read contents into level array	
	for(int i = 0; i < level_height; i++) {
		for(int j = 0; j < level_width; j++) {
			char c = fgetc(file);
			if(c == '\r') {
				c = fgetc(file);
			}
			if(c == '\n') {
				c = fgetc(file);
			}
			level[i][j] = c;
		}
	}

	torchList = new std::list<Entity>();
	loadEntities();

	fclose (file);
}

char Level::getCodeByBlock(int i, int j) {
	return level[scene_y + i][scene_x + j];
}

char Level::getCodeByCoord(int x, int y) {
	return level[getBlockXByCoord(x)][getBlockYByCoord(y)];
}

int Level::getBlockXByCoord(int xCoord) {
	return (xCoord + LEVEL_WIDTH_PIX) / LEVEL_WIDTH_PIX; //????
}

int Level::getBlockYByCoord(int yCoord) {
	return (yCoord + LEVEL_HEIGHT_PIX) / LEVEL_HEIGHT_PIX; //????
}

void Level::loadEntities() {

	torchList->clear();

	for(int i = 0; i < LEVEL_HEIGHT_BLOCK; i++)  {
		for(int j = 0; j < LEVEL_WIDTH_BLOCK; j++) {
			if(getCodeByBlock(i,j) == '^') {
				int x = Game::LEFT_MARGIN + j * LEVEL_WIDTH_PIX + TORCH_FLOAT_LEFT;
				int y = Game::TOP_MARGIN + LEVEL_HEIGHT_PIX * i - TORCH_FLOAT;

				Entity* torchEntity = new Entity (new Animation(L"Assets//torch.png", 5), x, y );

				torchEntity->getAnim()->setCurrentFrame(rand() % 5);
				torchEntity->getAnim()->setDisplayTime(80.0);
				torchEntity->getAnim()->setLoop(true);
				torchEntity->getAnim()->Play();

				torchList->push_back(*torchEntity);
			}
		}
	}
}

void Level::changeScene(direction dir) {

	switch(dir) {

	case U:
		scene_y -= SCENE_HEIGHT;
		if(scene_y < 0) scene_y = 0;
		break;
	case D:
		scene_y += SCENE_HEIGHT;
		if(scene_y + SCENE_HEIGHT > level_height) scene_y = level_height - 4;
		break;
	case L:
		scene_x -= SCENE_WIDTH;
		if(scene_x < 0) scene_x = 0;
		break;
	case R:
		scene_x += SCENE_WIDTH;
		if(scene_x + SCENE_WIDTH > level_width) scene_x = level_width - 12;
		break;
	}

	loadEntities();
}

std::list<Entity>* Level::getTorchEntities() {

	return torchList;
}