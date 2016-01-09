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

	while (fscanf(file, "%d", &platY) > 0 && fscanf(file, "%d", &platX) > 0 &&
		   fscanf(file, "%d", &gateY) > 0 && fscanf(file, "%d", &gateX) > 0) {

		std::pair<int, int> plat(platY, platX);
		std::pair<int, int> gate(gateY, gateX);

		(*mechanism)[plat] = gate;
		
	}

	entities = new std::map<std::pair<int, int>, Entity*>();

	loadEntities();

	fclose (file);
}

bool Level::inScene(int abs_block_x, int abs_block_y) {

	if (abs_block_x >= scene_x &&
		abs_block_x < scene_x + SCENE_WIDTH_BLK &&
		abs_block_y > scene_y &&
		abs_block_y <= scene_y + SCENE_HEIGHT_BLK) {
		return true;
	}
	return false;
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

	for(int abs_block_y = 0; abs_block_y < level_height; abs_block_y++)  {
		for(int abs_block_x = 0; abs_block_x < level_width; abs_block_x++) {

			int x = BLOCK_WIDTH_PX * ((abs_block_x) % SCENE_WIDTH_BLK - 1);
			int y = BLOCK_HEIGHT_PX * ((abs_block_y - 1) % SCENE_HEIGHT_BLK);

			Entity* entity = NULL;

			switch (getLevelCodeByBlock(abs_block_x, abs_block_y)) {

				case'|': {
					x += SWORD_OFFSET_X;
					y += SWORD_OFFSET_Y;

					int frames = 2;
					float * timing = (float *)malloc(frames * sizeof(float));
					timing[0] = 5000;
					timing[1] = 100;
					Entity* sword = new Entity(new Animation(Game::getSprite("sword"), frames, timing), x, y, swordT);

					sword->getAnim()->setCurrentFrame(rand() % 2);
					sword->getAnim()->setDisplayTime(90);
					sword->getAnim()->setLoop(true);
					sword->getAnim()->Play();

					entity = sword;
				} break;

				case '^': {
					x += TORCH_OFFSET_X;
					y += TORCH_OFFSET_Y;

					Entity* torch = new Entity(new Animation(Game::getSprite("torch"), 5), x, y, torchT);

					torch->getAnim()->setCurrentFrame(rand() % 5);
					torch->getAnim()->setDisplayTime(80.0);
					torch->getAnim()->setLoop(true);
					torch->getAnim()->Play();

					entity = torch;
				} break;

				case 'P': {
					x += POTION_OFFSET_X;
					y += POTION_OFFSET_Y;

					Entity* potion = new Entity(new Animation(Game::getSprite("potionHealth"), 6), x, y, potionT);

					potion->getAnim()->setCurrentFrame(rand() % 6);
					potion->getAnim()->setDisplayTime(100);
					potion->getAnim()->setLoop(true);
					potion->getAnim()->Play();

					entity = potion;
				} break;

				case'K': {
					x += POTION_OFFSET_X;
					y += POTION_OFFSET_Y;

					Entity* potion = new Entity(new Animation(Game::getSprite("potionPoison"), 6), x, y, poisonPotT);

					potion->getAnim()->setCurrentFrame(rand() % 6);
					potion->getAnim()->setDisplayTime(100);
					potion->getAnim()->setLoop(true);
					potion->getAnim()->Play();

					entity = potion;
				} break;

				case'E': {
					x += BIG_POTION_OFFSET_X;
					y += BIG_POTION_OFFSET_Y;

					Entity* potion = new Entity(new Animation(Game::getSprite("potionExtend"), 1), x, y, extendPotT);

					potion->getAnim()->setDisplayTime(100);
					potion->getAnim()->setLoop(true);
					potion->getAnim()->Play();

					entity = potion;
				} break;

				case '/': {
					x += SPIKE_OFFSET_X;
					y += SPIKE_OFFSET_Y;	//TODO: Why y - 1 

					Spikes* spikes = new Spikes(x, y, abs_block_x, abs_block_y);
					entity = spikes;
				} break;
				
				case '!': {
					Guilotine* guilotine = new Guilotine(x, y);
					entity = guilotine;
				} break;

				case 'G': {
					Gate* gate = new Gate(x, y, abs_block_x, abs_block_y);
					entity = gate;
				} break;

			}

			if (entity != NULL) {
				std::pair<int, int> result(abs_block_y, abs_block_x);
				std::pair<pair<int, int>, Entity*>* mapElement = new pair<pair<int, int>, Entity*>(result, entity);
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

	//loadEntities();
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
			if (spikes != NULL) { spikes->On(); }
			return true;
		}
	}
	return false;

}