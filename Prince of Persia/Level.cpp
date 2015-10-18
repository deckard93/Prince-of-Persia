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

	int platX;
	int platY;

	int gateX;
	int gateY;

	mechanism = new std::map< std::pair<int, int>, std::pair<int, int> >();

	while (fscanf(file, "%d", &platX) > 0 && fscanf(file, "%d", &platY) > 0 &&
		   fscanf(file, "%d", &gateX) > 0 && fscanf(file, "%d", &gateY) > 0) {

		std::pair<int, int> plat(platY, platX);
		std::pair<int, int> gate(gateY, gateX);

		(*mechanism)[plat] = gate;
		
	}

	entities = new std::map<std::pair<int, int>, Entity*>();

	loadEntities();

	fclose (file);
}

void Level::setCodeByCoord(int x, int y, char c) {
	level[scene_y + getBlockYByCoord(y)][scene_x + getBlockXByCoord(x)] = c;
	loadEntities();
}

char Level::getCodeByBlock(int i, int j) {
	return level[scene_y + i][scene_x + j];
}
char Level::getCodeByCoord(int x, int y) {
	int blockX = getBlockXByCoord(x);
	int blockY = getBlockXByCoord(y);

	return getCodeByBlock(blockY, blockX);
}

int Level::getBlockXByCoord(int xCoord) {
	return (xCoord + BLOCK_WIDTH_PX) / BLOCK_WIDTH_PX; //????
}
int Level::getBlockYByCoord(int yCoord) {
	return (yCoord + BLOCK_HEIGHT_PX) / BLOCK_HEIGHT_PX; //????
}

int Level::getAbsBlockX(int x) {
	return scene_x + x;
}
int Level::getAbsBlockY(int y) {
	return scene_y + y;
}

void Level::loadEntities() {

	entities->clear();

	for(int i = 0; i <= SCENE_HEIGHT_BLK; i++)  {
		for(int j = 0; j <= SCENE_WIDTH_BLK; j++) {

			int x = (j - 1) * BLOCK_WIDTH_PX;
			int y = BLOCK_HEIGHT_PX * (i - 1);

			if(getCodeByBlock(i,j) == '/') {
	
				x += SPIKE_OFFSET_X;
				y += SPIKE_OFFSET_Y;	//TODO: Why i - 1 

				Entity* spikeEntity = new Entity (new Animation(Game::getSprite("spikes"), 5), x, y, spikeT);

				spikeEntity->getAnim()->setCurrentFrame(rand() % 5);
				spikeEntity->getAnim()->setDisplayTime(90);
				spikeEntity->getAnim()->setLoop(true);
				spikeEntity->getAnim()->Play();

				int absI = getAbsBlockX(i);
				int absJ = getAbsBlockY(j);

				std::pair<int, int> result(absI, absJ);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, spikeEntity);
				entities->insert(*mapElement);
			}
			if(getCodeByBlock(i,j) == '^') {
				int x = (j) * BLOCK_WIDTH_PX;
				int y = BLOCK_HEIGHT_PX * (i);

				x += TORCH_OFFSET_X;
				y -= TORCH_OFFSET_Y;

				Entity* torchEntity = new Entity (new Animation(L"Assets//torch.png", 5), x, y, torchT);

				torchEntity->getAnim()->setCurrentFrame(rand() % 5);
				torchEntity->getAnim()->setDisplayTime(80.0);
				torchEntity->getAnim()->setLoop(true);
				torchEntity->getAnim()->Play();

				int absI = getAbsBlockX(i);
				int absJ = getAbsBlockY(j);

				std::pair<int, int> result(absI, absJ);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, torchEntity);
				entities->insert(*mapElement);
			} 
			if(getCodeByBlock(i,j) == 'P') {
				x += POTION_OFFSET_X;
				y += POTION_OFFSET_Y;

				Entity* potionEntity = new Entity (new Animation(L"Assets//healthPotion.png", 6), x, y, potionT);

				potionEntity->getAnim()->setCurrentFrame(rand() % 6);
				potionEntity->getAnim()->setDisplayTime(100);
				potionEntity->getAnim()->setLoop(true);
				potionEntity->getAnim()->Play();

				int absI = getAbsBlockX(i);
				int absJ = getAbsBlockY(j);

				std::pair<int, int> result(absI, absJ);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, potionEntity);
				entities->insert(*mapElement);
			}
			if(getCodeByBlock(i,j) == '!') {
				Entity* guilotineEntity = new Entity(new Animation(L"Assets//guilotine.png", 5), x, y, guilotineT);

				guilotineEntity->getAnim()->setCurrentFrame(rand() % 5);
				guilotineEntity->getAnim()->setDisplayTime(90);
				guilotineEntity->getAnim()->setLoop(true);
				guilotineEntity->getAnim()->Play();

				int absI = getAbsBlockX(i);
				int absJ = getAbsBlockY(j);
				
				std::pair<int, int> result(absI, absJ);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, guilotineEntity);
				entities->insert(*mapElement);
			}

			if(getCodeByBlock(i,j) == 'G') {
				Gate* gate = new Gate(x, y, scene_x + i, scene_y + j);

				int absI = getAbsBlockX(i);
				int absJ = getAbsBlockY(j);

				std::pair<int, int> result(absI, absJ);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, gate);
				entities->insert(*mapElement);		
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

std::map<std::pair<int,int>, Entity*>* Level::getEntities() {
	return entities;
}
std::map<std::pair<int, int>, std::pair<int, int> >* Level::getMec() {
	return mechanism;
}