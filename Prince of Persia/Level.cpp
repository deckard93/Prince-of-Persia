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
	for(int y = 0; y < level_height; y++) {
		for(int x = 0; x < level_width; x++) {
			char c = fgetc(file);
			if(c == '\r') {
				c = fgetc(file);
			}
			if(c == '\n') {
				c = fgetc(file);
			}
			level[y][x] = c;
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
	level[scene_y + getSceneBlockYByCoord(y)][scene_x + getSceneBlockXByCoord(x)] = c;
	loadEntities();
}

char Level::getLevelCodeByBlock(int x, int y) {
	return level[y][x];
}

char Level::getSceneCodeByBlock(int y, int x) {
	return level[scene_y + y][scene_x + x];
}
char Level::getSceneCodeByCoord(int x, int y) {
	int blockX = getSceneBlockXByCoord(x);
	int blockY = getSceneBlockYByCoord(y);

	return getSceneCodeByBlock(blockY, blockX);
}

int Level::getSceneBlockXByCoord(int xCoord) {
	return (xCoord + BLOCK_WIDTH_PX) / BLOCK_WIDTH_PX; //????
}
int Level::getSceneBlockYByCoord(int yCoord) {
	return (yCoord + BLOCK_HEIGHT_PX) / BLOCK_HEIGHT_PX; //????
}

int Level::getLevelBlockX(int x) {
	return scene_x + x;
}
int Level::getLevelBlockY(int y) {
	return scene_y + y;
}

void Level::loadEntities() {

	entities->clear();

	for(int block_y = 0; block_y <= SCENE_HEIGHT_BLK; block_y++)  {
		for(int block_x = 0; block_x <= SCENE_WIDTH_BLK; block_x++) {

			int x = (block_x - 1) * BLOCK_WIDTH_PX;
			int y = BLOCK_HEIGHT_PX * (block_y - 1);

			int absY;
			int absX;

			if (getSceneCodeByBlock(block_y, block_x) == '|') {

				x += SWORD_OFFSET_X;
				y += SWORD_OFFSET_Y;

				int frames = 2;
				float * timing = (float *) malloc(frames * sizeof(float));
				timing[0] = 5000;
				timing[1] = 100;
				Entity* swordEntity = new Entity(new Animation(Game::getSprite("sword"), frames, timing), x, y, swordT);

				swordEntity->getAnim()->setCurrentFrame(rand() % 2);
				swordEntity->getAnim()->setDisplayTime(90);
				swordEntity->getAnim()->setLoop(true);
				swordEntity->getAnim()->Play();

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);

				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, swordEntity);
				entities->insert(*mapElement);
			}

			if(getSceneCodeByBlock(block_y,block_x) == '/') {
	
				/*
				x += SPIKE_OFFSET_X;
				y += SPIKE_OFFSET_Y;	//TODO: Why y - 1 

				Entity* spikeEntity = new Entity (new Animation(Game::getSprite("spikes"), 6), x, y, spikeT);

				spikeEntity->getAnim()->setCurrentFrame(rand() % 5);
				spikeEntity->getAnim()->setDisplayTime(90);
				//spikeEntity->getAnim()->setLoop(true);
				//spikeEntity->getAnim()->Play();

				int absY = getLevelBlockX(y);
				int absX = getLevelBlockY(x);

				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, spikeEntity);
				entities->insert(*mapElement);
				*/

				x += SPIKE_OFFSET_X;
				y += SPIKE_OFFSET_Y;	//TODO: Why y - 1 

				Spikes* spikes = new Spikes(x, y, scene_x + block_y, scene_y + block_x);

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);

				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, spikes);
				entities->insert(*mapElement);

			}
			if(getSceneCodeByBlock(block_y,block_x) == '^') {
				int x = (block_x) * BLOCK_WIDTH_PX;
				int y = BLOCK_HEIGHT_PX * (block_y);

				x += TORCH_OFFSET_X;
				y -= TORCH_OFFSET_Y;

				Entity* torchEntity = new Entity (new Animation(L"Assets//torch.png", 5), x, y, torchT);

				torchEntity->getAnim()->setCurrentFrame(rand() % 5);
				torchEntity->getAnim()->setDisplayTime(80.0);
				torchEntity->getAnim()->setLoop(true);
				torchEntity->getAnim()->Play();

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);

				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, torchEntity);
				entities->insert(*mapElement);
			} 
			if(getSceneCodeByBlock(block_y,block_x) == 'P') {
				x += POTION_OFFSET_X;
				y += POTION_OFFSET_Y;

				Entity* potionEntity = new Entity (new Animation(L"Assets//healthPotion.png", 6), x, y, potionT);

				potionEntity->getAnim()->setCurrentFrame(rand() % 6);
				potionEntity->getAnim()->setDisplayTime(100);
				potionEntity->getAnim()->setLoop(true);
				potionEntity->getAnim()->Play();

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);

				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, potionEntity);
				entities->insert(*mapElement);
			}
			if(getSceneCodeByBlock(block_y,block_x) == '!') {
				Entity* guilotineEntity = new Entity(new Animation(L"Assets//guilotine.png", 5), x, y, guilotineT);

				guilotineEntity->getAnim()->setCurrentFrame(rand() % 5);
				guilotineEntity->getAnim()->setDisplayTime(90);
				guilotineEntity->getAnim()->setLoop(true);
				guilotineEntity->getAnim()->Play();

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);
				
				std::pair<int, int> result(absY, absX);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, guilotineEntity);
				entities->insert(*mapElement);
			}

			if(getSceneCodeByBlock(block_y,block_x) == 'G') {
				Gate* gate = new Gate(x, y, scene_x + block_y, scene_y + block_x);

				absY = getLevelBlockY(block_y);
				absX = getLevelBlockX(block_x);

				std::pair<int, int> result(absY, absX);
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


int Level::getLevelHeight() {
	return level_height;
}
int Level::getLevelWidth() {
	return level_width;
}

std::map<std::pair<int,int>, Entity*>* Level::getEntities() {
	return entities;
}
std::map<std::pair<int, int>, std::pair<int, int> >* Level::getMec() {
	return mechanism;
}


bool Level::isFreeSpace(char c) {
	if (c == ' ' ||
		c == '*' ||
		c == '#' ) {
		return true;
	}
	return false;
}

bool Level::findSpikes(int block_x, int block_y) {
	for (int y = block_y; y < level_height; y++) {
		if (level[y][block_x] == 'I' || 
			level[y][block_x] == '[' ||
			level[y][block_x] == ']') {
			break;
		}
		if (level[y][block_x] == '/') {
			std::pair<int, int> p(y, block_x);
			Entity* e = (*entities)[p];
			Spikes* spikes = dynamic_cast<Spikes*>(e);
			spikes->On();
			return true;
		}
	}
	return false;

}