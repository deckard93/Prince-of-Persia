#include "Input.h"
#include "Audio.h"
#include "Benchmark.h"
#include "Audio.h"
#include "Entity.h"
#include "Prince.h"
#include "Gate.h"
#include "Spikes.h"
#include "Guilotine.h"

#include "Platform.h"
#include "Graphics.h"
#include "Level.h"
#include "Guard.h"
#include "FinishDoor.h"
#include "GuardAI.h"
#include "Collision.h"

#include "Game.h"


hash_map<string, Sprite*>* Game::assets = new hash_map<string, Sprite*>();


Game::Game(HWND hwnd, Input* in) : 
		graphics(hwnd),
		input(in) {

	LoadFont(&fixedSys, fontSurf, "Assets//fixedsys16x28.bmp", 16, 28, 32);

	// Assets
	RegisterSprite("block");
	RegisterSprite("blockCornerLeft");
	RegisterSprite("blockCornerRight");
	RegisterSprite("blockRound");
	RegisterSprite("bricks");
	RegisterSprite("separator");
	RegisterSprite("separator_corner");
	RegisterSprite("floor");
	RegisterSprite("trap");
	RegisterSprite("activate");
	RegisterSprite("healthFull");
	RegisterSprite("healthEmpty");
	RegisterSprite("columnFront");
	RegisterSprite("columnBack");
	RegisterSprite("rubbleBack");
	RegisterSprite("rubbleFront");
	RegisterSprite("doorFrameBack");
	RegisterSprite("doorFrameFront");
	RegisterSprite("tileCornerLeft");
	RegisterSprite("deadSk");
	RegisterSprite("gate");
	RegisterSprite("holyFloor");

	// Door sprites
	RegisterSprite("finishDoor", "Assets//finishDoor//");
	RegisterSprite("finishDoorBase", "Assets//finishDoor//");
	RegisterSprite("enterDoor", "Assets//finishDoor//");

	RegisterSprite("platformMove", "Assets//platform//");


	// Entities
	RegisterSprite("spikesBackground");
	RegisterSprite("spikesForeground");
	RegisterSprite("spikes");
	RegisterSprite("torch");
	RegisterSprite("guilotine");
	RegisterSprite("gate");
	RegisterSprite("sword");
	RegisterSprite("potionHealth");
	RegisterSprite("potionPoison");
	RegisterSprite("potionExtend");
	
	// Prince sprite sheets
	RegisterSprite("idle"       , "Assets//prince//");
	RegisterSprite("turn"       , "Assets//prince//");
	RegisterSprite("running"    , "Assets//prince//");
	RegisterSprite("climbUp"    , "Assets//prince//");
	RegisterSprite("newClimbUp" , "Assets//prince//");
	RegisterSprite("jumpGrab"   , "Assets//prince//");
	RegisterSprite("newJumpGrab", "Assets//prince//");
	RegisterSprite("crouch"     , "Assets//prince//");
	RegisterSprite("staticJump" , "Assets//prince//");
	RegisterSprite("step"       , "Assets//prince//");
	RegisterSprite("missStep"   , "Assets//prince//");
	RegisterSprite("hang"       , "Assets//prince//");
	RegisterSprite("newHang"    , "Assets//prince//");
	RegisterSprite("runningJump", "Assets//prince//");
	RegisterSprite("runningTurn", "Assets//prince//");
	RegisterSprite("fall"       , "Assets//prince//");
	RegisterSprite("drop"       , "Assets//prince//");
	RegisterSprite("newDrop"    , "Assets//prince//");
	RegisterSprite("drink"      , "Assets//prince//");
	RegisterSprite("pickSword"  , "Assets//prince//");

	// Guard sprite sheets
	RegisterSprite("guardIdle", "Assets//guard//");


	// Fight sprite sheets
	RegisterSprite("fightIdle"  , "Assets//prince//");
	RegisterSprite("fightStep"  , "Assets//prince//");
	RegisterSprite("fightParry" , "Assets//prince//");
	RegisterSprite("fightStart" , "Assets//prince//");
	RegisterSprite("fightStrike", "Assets//prince//");
	RegisterSprite("fightFinish", "Assets//prince//");
	RegisterSprite("fightInjure", "Assets//prince//");
	RegisterSprite("fightDying" , "Assets//prince//");
	RegisterSprite("swordDeath", "Assets//prince//");

	RegisterSprite("guardFightIdle"   , "Assets//guard//");
	RegisterSprite("guardFightStep"   , "Assets//guard//");
	RegisterSprite("guardFightParry"  , "Assets//guard//");
	RegisterSprite("guardFightParried", "Assets//guard//");
	RegisterSprite("guardFightStrike" , "Assets//guard//");
	RegisterSprite("guardFightInjure" , "Assets//guard//");
	RegisterSprite("guardFightDying"  , "Assets//guard//");
	RegisterSprite("guardSwordDeath"  , "Assets//guard//");
	//RegisterSprite("fightFinish", "Assets//guard//");
	//RegisterSprite("fightStart", "Assets//guard//");

	// Death/Kill sprites
	RegisterSprite("spikeKill"  , "Assets//prince//");
	RegisterSprite("fightKill"  , "Assets//prince//");


	// Objects
	level = new Level();
	prince = new Prince();
	engagedGuard = NULL;
	collision = new Collision(level, &graphics, input);

	Reset();

	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();


	timer.StartWatch();
	timeSinceLastFrame = 0;
}

void Game::RegisterSprite(string name, string path) {
	Sprite * sprite = new Sprite();
	std::wstring item = std::wstring(path.begin(), path.end()) + std::wstring(name.begin(), name.end()) + L".png";
	LoadSprite(sprite, item.c_str());
	assets->insert(make_pair(name, sprite));
}
Sprite* Game::getSprite(string name) {
	return (*assets)[name];
}

void Game::ControlAI() {
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		if (i->second == NULL) { continue; } // TODO: should not happen check load entities
		if (i->second->getType() == guardT) {
			if (level->inScene(i->first.second, i->first.first)) {
				Entity* guard = i->second;
				GuardAI* guardAI = dynamic_cast<GuardAI*>(guard);
				assert(guardAI != 0);
				guardAI->Control(*prince, *level);
				Character* character = dynamic_cast<Character*>(guard);
				assert(character != 0);
				collision->CheckCharacterCollision(character);
			}
		}
	}
}
void Game::HandleInput()  {
	
	if(input->hasBeenPressed('A')) {
		level->changeScene(L);
	} 

	if(input->hasBeenPressed('D')) {
		level->changeScene(R);
	}

	if(input->hasBeenPressed('S')) {
		level->changeScene(D);
	} 

	if(input->hasBeenPressed('W')) {
		level->changeScene(U);
	}

	if (input->hasBeenPressed('R')) {
		Reset();
	}

	if (input->getKeyStatus('P')) {
		prince->setDefferX(1);
	}

	if (input->getKeyStatus('O')) {
		prince->setDefferX(-1);
	}


	if (input->getKeyStatus('G')) {
		prince->Heal();
		std::list<Character*>* guards = level->getGuards();
		std::list<Character*>::iterator i = guards->begin();
		Character* guard = *i;
		guard->Heal();
	}

	if (input->getKeyStatus('F')) {
		prince->Hurt();
	}

	if (input->hasBeenPressed('H')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == spikeT) {
				Spikes* spikes = dynamic_cast<Spikes*>(entity);
				if (spikes != NULL) { spikes->On(); }
			}
		}
	}

	if (input->hasBeenPressed('J')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == spikeT) {
				Spikes* spikes = dynamic_cast<Spikes*>(entity);
				if (spikes != NULL) { spikes->Off(); }
			}
		}
	}

	if (input->hasBeenPressed('V')) {
		std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
		for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
			Entity* entity = i->second;
			if (entity->getType() == doorT) {
				FinishDoor* door = dynamic_cast<FinishDoor*>(entity);
				if (door != NULL) { door->Finish(); }
			}
		}
	}

	if(input->isShiftPressed()) {

		std::string s = "Code: ";
		std::string g(1, level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()));
		s += g + "\n";
		OutputDebugStringA(s.c_str());

		if(level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'P') {
			if(prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->Heal();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'K') {
			if (prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->Hurt();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'E') {
			if (prince->Drink()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
				prince->increaseMaxHealth();
			}
		}

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == '|') {
			if (prince->PickUpSword()) {
				level->setCodeByCoord(prince->getMidX(), prince->getMidY(), '_');
			}
		}


		
	}

	if (input->isUpPressed()) {

		//std::string s = "Code: ";
		//std::string g(1, level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()));
		//s += g + "\n";
		//OutputDebugStringA(s.c_str());

		if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == 'H') {
			std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();
			std::pair<int, int> gateKey = make_pair(level->getLevelBlockYByCoord(prince->getMidY()), level->getLevelBlockXByCoord(prince->getMidX()));
		
			std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

			if (entitites->find(gateKey) == entitites->end()) {
				return;
			}

			Entity* e = (*entitites)[gateKey];
			if (e->getType() == doorT) {
				FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			
				if (d->Finish()) {
					prince->setState(sFinish);
				}
			}			
		}
	}


	prince->HandlePrince(input);
}

void Game::Reset() {
	level->loadLevel(1);

	prince = new Prince();
	//prince->setX(10 * Level::BLOCK_WIDTH_PX - 50);
	prince->setX(7 * Level::BLOCK_WIDTH_PX - 70);
	prince->setY(prince->getAnim()->getSheet()->getFrameHeight() - Level::FOOT_FLOAT - Level::BLOCK_HEIGHT_PX + 12); // Magic Number should be size of ledge?
	prince->setState(sIdle);
}


void Game::GameLoop() {

	graphics.BeginFrame();

	DrawGraphics();

	HandleInput();

	if (!prince->isDead()) {
		ControlAI();
		CheckCollision();
	}

	graphics.EndFrame();
}


bool Game::isGuardInScene(Character* guard_in) {

	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Character* guard = dynamic_cast<Character*>(entity);
		if (guard != NULL && level->inScene(i->first.second, i->first.first) && guard_in == guard) {
			return true;
		}
	}
	return false;
}
void Game::EngageFight(Character* prince, Character* guard, Level* level) {

	if (level->getCharLevelBlockY(prince) != level->getCharLevelBlockY(guard)) { return; }
	if (std::abs(level->getCharLevelBlockX(prince) - level->getCharLevelBlockX(guard)) > 9) { return; }  //TODO: This doesnt work guard is calculated relative to scene
	if (!isGuardInScene(guard)) { return; }

	SetEngagedGuard(guard);
	if (!prince->isIdle()) { return; }

	if (prince->isFighting()) {
		prince->FaceCharacter(*guard, *level);
		return;
	}

	if (prince->getX() < guard->getX() && !prince->isFacingRight()) { return; }
	if (prince->getX() >= guard->getX() && prince->isFacingRight()) { return; }

	prince->EngageEnemy(*guard);
}
void Game::SetEngagedGuard(Character* guard) {
	if (engagedGuard == NULL) {
		engagedGuard = guard;
	}
	else {
		int currentGuardDistance = std::sqrt((std::pow(guard->getX() - prince->getX(), 2.0) + std::pow(guard->getY() - prince->getY(), 2.0)));
		int engagedGuardDistance = std::sqrt((std::pow(engagedGuard->getX() - prince->getX(), 2.0) + std::pow(engagedGuard->getY() - prince->getY(), 2.0)));

		if (currentGuardDistance < engagedGuardDistance) {
			engagedGuard = guard;
		}
	}
}


// TODO
void Game::DrawGraphics() {

	//graphics.BeginFrame();

	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	DrawBackground();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Spikes* spikes = dynamic_cast<Spikes*>(entity);
		if (spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateBackground(&graphics);
		}
		else {
			//i->second->Animate(NULL);
		}
	}
	if (DEBUG) { graphics.DrawCircle(prince->getMidX(), prince->getMidY(), 5, 255, 255, 255); }
	if (DEBUG) { graphics.DrawCircle(prince->getX(), prince->getY(), 5, 255, 255, 255); }

	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		if (i->second == NULL) { continue; } // TODO: should not happen check load entities
		if (i->second->getType() == spikeT || i->second->getType() == platformT) { continue; }

		if (level->inScene(i->first.second, i->first.first)) {
			i->second->Animate(&graphics);
		}
		else {
			i->second->Animate(NULL);
		}
	}

	prince->Animate(&graphics);

	DrawForeground();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Spikes* spikes = dynamic_cast<Spikes*>(entity);
		if (spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateForeground(&graphics);
		}
		else {
			//i->second->Animate(NULL);
		}
	}

	DrawHealth();

	//graphics.EndFrame();

}
void Game::DrawHealth() {

	for (int i = 0; i < prince->getHealth(); i++) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(i * getSprite("healthFull")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}

	for (int i = prince->getHealth(); i < prince->getMaxHealth(); i++) {
		//graphics.DrawSprite(y * healthFull.width,  Level::BLOCK_HEIGHT_PX * 3, &healthEmpty );
		graphics.DrawSprite(i * getSprite("healthEmpty")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthEmpty"));
	}

	//std::list<Character*>* guards = level->getGuards();
	//std::list<Character*>::iterator i = guards->begin();
	//engagedGuard = *i;
	if (engagedGuard == NULL) { return; };
	for (int i = 0; i < engagedGuard->getHealth(); i++) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(Level::BLOCK_WIDTH_PX * 10 - (i + 1) * (getSprite("healthFull")->width), Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}
}
void Game::DrawBackground() {
	int yOff = 0;
	int xOff = 0;

	//TODO this value is being reused in check collision move it in a common location or eliminate collision detection when drawing completely
	int princeXOffset = 22;

	for (int y = Level::SCENE_HEIGHT_BLK; y >= 0; y--) {
		for (int x = 0; x <= Level::SCENE_WIDTH_BLK; x++) {

			yOff = Level::BLOCK_HEIGHT_PX * y;
			xOff = -Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * x;

			switch (level->getSceneCodeByBlock(y, x)) {
			case '|':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff - 1, yOff - getSprite("columnBack")->height - 15, getSprite("columnBack"));
				break;
			case ']':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerRight")->height, getSprite("blockCornerRight"));
				break;
			case '_':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '1':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '~': {
				std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
				int blockX = level->getLevelBlockX(x);
				int blockY = level->getLevelBlockY(y);
				std::pair<int, int> platformKey = std::make_pair(blockY, blockX);
				Entity* e = entities->at(platformKey);
				Platform* p = dynamic_cast<Platform*>(e);
				if (p != NULL) {
					int currentLevelBlockX = blockX;
					int currentLevelBlockY = blockY + (p->getYOffset() + 120) / Level::BLOCK_HEIGHT_PX;

					if (level->inScene(currentLevelBlockX, currentLevelBlockY)) {
						p->Animate(&graphics);
					}
				}
				if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x &&
					level->getSceneBlockYByCoord(prince->getMidY()) == y && prince->getState() != sJumping) {
					p->Drop();
				}
			} break;

			case 'H':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '-':
				if (level->getSceneCodeByCoord(prince->getMidX() - princeXOffset, prince->getMidY()) == '-') {
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x &&
						level->getSceneBlockYByCoord(prince->getMidY() == y) &&
						prince->getState() != sJumping) {
						graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
					}
				}
				else {
					graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
				}
				break;
			case '=':
				if (level->getSceneCodeByCoord(prince->getMidX() - princeXOffset, prince->getMidY()) == '=') {
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == x &&
						level->getSceneBlockYByCoord(prince->getMidY() == y) &&
						prince->getState() != sJumping
						) {
						graphics.DrawSprite(xOff, yOff + 2 - getSprite("trap")->height, getSprite("trap"));
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("trap")->height, getSprite("trap"));
					}
				}
				else {
					graphics.DrawSprite(xOff, yOff - getSprite("trap")->height, getSprite("trap"));
				}
				break;
			case '*':
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case '/':
				graphics.DrawSprite(xOff, yOff - getSprite("holyFloor")->height, getSprite("holyFloor"));
				break;
			case 'S':
				graphics.DrawSprite(xOff, yOff - getSprite("deadSk")->height, getSprite("deadSk"));
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - getSprite("doorFrameBack")->height, getSprite("doorFrameBack"));
				break;
			case '#':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case 'P':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'K':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case 'E':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleBack")->height, getSprite("rubbleBack"));
				break;
			case '^':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '!':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case ' ':
				break;

			}
		}
	}
}
void Game::DrawForeground() {
	int yOff;
	int xOff;


	//TODO this value is being reused in check collision move it in a common location or eliminate collision detection when drawing completely
	int princeXOffset = 22;

	int mask = 20;
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 11; j++) {

			yOff = Level::BLOCK_HEIGHT_PX * i;
			xOff = -Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * j;

			switch (level->getSceneCodeByBlock(i, j)) {
			case '_': //or
			case '#': //or
			case 'P': //or
			case 'K': //or				
			case 'E': //or
			case '^': //or
			case '!':
				if (level->isEmptySpace(level->getSceneCodeByBlock(i, j - 1))) {
					graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height + mask, getSprite("tileCornerLeft"), 0, mask, getSprite("tileCornerLeft")->width - 60, getSprite("tileCornerLeft")->height - mask);
				}
				break;
			case 'T':
				if (level->isEmptySpace(level->getSceneCodeByBlock(i, j - 1))) {
					graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height + mask, getSprite("tileCornerLeft"), 0, mask, getSprite("tileCornerLeft")->width - 60, getSprite("tileCornerLeft")->height - mask);
				}
				graphics.DrawSprite(xOff, yOff - getSprite("columnFront")->height, getSprite("columnFront"));
				break;
			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleFront")->height, getSprite("rubbleFront"));
				break;
			case '[':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerLeft")->height, getSprite("blockCornerLeft"));
				break;
			case 'O':
				graphics.DrawSprite(xOff, yOff - getSprite("blockRound")->height, getSprite("blockRound"));
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - getSprite("doorFrameFront")->height, getSprite("doorFrameFront"));
				break;
			case 'I':
				graphics.DrawSprite(xOff, yOff - getSprite("block")->height, getSprite("block"));
				break;
			case '-':
				if (level->getSceneCodeByCoord(prince->getMidX() - princeXOffset, prince->getMidY()) == '-') {
					if (level->getSceneBlockXByCoord(prince->getMidX() - princeXOffset) == j &&
						level->getSceneBlockYByCoord(prince->getMidY() == i) &&
						prince->getState() != sJumping) {
						graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height + mask, getSprite("tileCornerLeft"), 0, mask, getSprite("tileCornerLeft")->width - 60, getSprite("tileCornerLeft")->height - mask);
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("activate")->height + mask, getSprite("activate"), 0, mask, getSprite("activate")->width - 60, getSprite("activate")->height - mask);
					}
				}
				else {
					graphics.DrawSprite(xOff, yOff - getSprite("activate")->height + mask, getSprite("activate"), 0, mask, getSprite("activate")->width - 60, getSprite("activate")->height - mask);
				}
				break;
				break;
			}
		}
	}
}

void Game::CheckCombatCollision() {

	if (engagedGuard != NULL && engagedGuard->isDead()) {
		prince->Disengage();
	}
	engagedGuard = NULL;
	std::list<Character*>* guards = level->getGuards();

	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		EngageFight(prince, guard, level);
	}


	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		guard->checkParryBy(prince);
		if (guard->isHitting(prince, *level)) {
			prince->Hurt();
		}
	}

	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		//if (!prince->checkParryBy(guard));
		if (prince->isHitting(guard, *level)) {
			guard->Hurt();
		}
	}
	if (engagedGuard == NULL) { return; }
	double distance = abs(prince->getX() - engagedGuard->getX());
	if (distance > 50 && prince->isFighting()) {
		prince->FaceCharacter(*engagedGuard, *level);
	}
}
void Game::CheckCollision() {

	// TODO all X and Y coords to absolute (level) values as opposed to screen values refactor this
	std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entities->begin(); i != entities->end(); i++) {
		Entity* entity = i->second;
		Platform* p = dynamic_cast<Platform*>(entity);

		int levelBlockX = i->first.second;
		int levelBlockY = i->first.first;
		
		if (p != NULL && p->getState() == dislodged) {
			p->setYOffset(p->getYOffset() + 5); //TODO needs to be a variable

			int currentLevelBlockX = levelBlockX;
			int currentLevelBlockY = levelBlockY + (p->getYOffset() + 50) / Level::BLOCK_HEIGHT_PX;

			if (level->getLevelCodeByBlock(currentLevelBlockX, currentLevelBlockY) == '_') {
				p->setState(broken);
				level->setLevelCodeByBlock(currentLevelBlockX,currentLevelBlockY, '$');
				level->setLevelCodeByBlock(levelBlockX, levelBlockY, ' ');
			}
			else {
				p->setY(int(p->getY() + 5) % (Level::BLOCK_HEIGHT_PX * Level::SCENE_HEIGHT_BLK));
				//p->setY((levelBlockY - level->getSceneY()) * Level::BLOCK_HEIGHT_PX + p->getYOffset() - p->getAnim()->getSheet()->getFrameHeight());

				if (!level->inScene(levelBlockX, levelBlockY) && level->inScene(currentLevelBlockX, currentLevelBlockY)) {
					p->Animate(&graphics);
				}
			}
		}	
	}


	collision->CheckPrinceCollision(prince);
	CheckCombatCollision();
}