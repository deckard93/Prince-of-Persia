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
	potionList = new std::list<Entity>();
	spikeList = new std::list<Entity>();
	guilotineList = new std::list<Entity>();
	gateList = new std::list<Entity>();

	loadEntities();

	fclose (file);
}

void Level::setCodeByCoord(int x, int y, char c) {
	level[scene_y + getBlockYByCoord(y - Game::TOP_MARGIN)][scene_x + getBlockXByCoord(x - Game::LEFT_MARGIN)] = c;
	loadEntities();
}

char Level::getCodeByBlock(int i, int j) {
	return level[scene_y + i][scene_x + j];
}
char Level::getCodeByCoord(int x, int y) {
	int blockX = getBlockXByCoord(x - Game::LEFT_MARGIN);
	int blockY = getBlockXByCoord(y - Game::TOP_MARGIN);

	return getCodeByBlock(blockY, blockX);
}

int Level::getBlockXByCoord(int xCoord) {
	return (xCoord + LEVEL_WIDTH_PIX) / LEVEL_WIDTH_PIX; //????
}
int Level::getBlockYByCoord(int yCoord) {
	return (yCoord + LEVEL_HEIGHT_PIX) / LEVEL_HEIGHT_PIX; //????
}

void Level::loadEntities() {

	torchList->clear();
	potionList->clear();
	spikeList->clear();
	guilotineList->clear();

	for(int i = 0; i <= LEVEL_HEIGHT_BLOCK; i++)  {
		for(int j = 0; j <= LEVEL_WIDTH_BLOCK; j++) {
			if(getCodeByBlock(i,j) == '/') {
				OutputDebugString(L"HERE!\n");

				int x = Game::LEFT_MARGIN + (j - 1) * LEVEL_WIDTH_PIX;
				int y = Game::TOP_MARGIN + LEVEL_HEIGHT_PIX * (i - 1) + 26;	//TODO: Why i - 1 //TODO make 27 float

				Entity* spikeEntity = new Entity (new Animation(L"Assets//spikes.png", 5), x, y );

				spikeEntity->getAnim()->setCurrentFrame(rand() % 5);
				spikeEntity->getAnim()->setDisplayTime(90);
				spikeEntity->getAnim()->setLoop(true);
				spikeEntity->getAnim()->Play();

				spikeList->push_back(*spikeEntity);
			}
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
			if(getCodeByBlock(i,j) == 'P') {
				int x = Game::LEFT_MARGIN + j * LEVEL_WIDTH_PIX - 20;
				int y = Game::TOP_MARGIN + LEVEL_HEIGHT_PIX * i - 60;

				Entity* potionEntity = new Entity (new Animation(L"Assets//healthPotion.png", 6), x, y );

				potionEntity->getAnim()->setCurrentFrame(rand() % 6);
				potionEntity->getAnim()->setDisplayTime(100);
				potionEntity->getAnim()->setLoop(true);
				potionEntity->getAnim()->Play();

				potionList->push_back(*potionEntity);
			}
			if(getCodeByBlock(i,j) == '!') {
				int x = Game::LEFT_MARGIN + (j - 1) * LEVEL_WIDTH_PIX;
				int y = Game::TOP_MARGIN + LEVEL_HEIGHT_PIX * (i - 1);	//TODO: Why i - 1 //TODO make 27 float

				Entity* guilotineEntity = new Entity (new Animation(L"Assets//guilotine.png", 5), x, y );

				guilotineEntity->getAnim()->setCurrentFrame(rand() % 5);
				guilotineEntity->getAnim()->setDisplayTime(90);
				guilotineEntity->getAnim()->setLoop(true);
				guilotineEntity->getAnim()->Play();

				guilotineList->push_back(*guilotineEntity);
			}

			if(getCodeByBlock(i,j) == 'G') {
				int x = Game::LEFT_MARGIN + (j - 1) * LEVEL_WIDTH_PIX;
				int y = Game::TOP_MARGIN + LEVEL_HEIGHT_PIX * (i - 1);	//TODO: Why i - 1 //TODO make 27 float

				Entity* gateEntity = new Entity (new Animation(L"Assets//gate.png", 7), x, y );

				gateEntity->getAnim()->setCurrentFrame(rand() % 5);
				gateEntity->getAnim()->setDisplayTime(90);
				gateEntity->getAnim()->setLoop(true);
				gateEntity->getAnim()->Play();

				guilotineList->push_back(*gateEntity);
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
std::list<Entity>* Level::getPotionEntities() {
	return potionList;
}
std::list<Entity>* Level::getSpikeEntities() {
	return spikeList;
}
std::list<Entity>* Level::getGuilotineEntities() {
	return guilotineList;
}
std::list<Entity>* Level::getGateEntities() {
	return gateList;
}