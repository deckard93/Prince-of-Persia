#include "Game.h"
#include "Platform.h"
#include "Graphics.h"
#include "Level.h"
#include "Guard.h"
#include "FinishDoor.h"
#include "GuardAI.h"


#define BORDERS 0


hash_map<string, Sprite*>* Game::assets = new hash_map<string, Sprite*>();


Game::Game(HWND hwnd, Input* in) : 
		graphics(hwnd),
		input(in) {

	LoadFont(&fixedSys, fontSurf, "Assets//fixedsys16x28.bmp", 16, 28, 32);

	//assets
	RegisterSprite("block");
	RegisterSprite("blockCornerLeft");
	RegisterSprite("blockCornerRight");
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

	//entities
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
	
	//prince sprite sheets
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
	RegisterSprite("hang"       , "Assets//prince//");
	RegisterSprite("newHang"    , "Assets//prince//");
	RegisterSprite("runningJump", "Assets//prince//");
	RegisterSprite("runningTurn", "Assets//prince//");
	RegisterSprite("fall"       , "Assets//prince//");
	RegisterSprite("drop"       , "Assets//prince//");
	RegisterSprite("newDrop"    , "Assets//prince//");
	RegisterSprite("drink"      , "Assets//prince//");
	RegisterSprite("pickSword"  , "Assets//prince//");

	//prince sprite sheets
	RegisterSprite("guardIdle", "Assets//guard//");


	//fight sheets
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

	//death/kill sprites
	RegisterSprite("spikeKill"  , "Assets//prince//");
	RegisterSprite("fightKill"  , "Assets//prince//");


	//door sprites
	RegisterSprite("finishDoor"    , "Assets//finishDoor//");
	RegisterSprite("finishDoorBase", "Assets//finishDoor//");
	RegisterSprite("enterDoor"     , "Assets//finishDoor//");

	RegisterSprite("platformMove", "Assets//platform//");


	//objects
	level = new Level();
	prince = new Prince();
	engagedGuard = NULL;

	Reset();

	prince->getAnim()->setLoop(true);
	prince->getAnim()->Play();

}

void Game::GameLoop() {	
	graphics.BeginFrame();
	
	HandleInput();
	if (!prince->isDead()) {
		ControlAI();
		CheckCollision();
	}

	DrawGraphics();

	graphics.EndFrame();
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

//control
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
				CheckCharacterCollision(*character);
			}
		}
	}
}


void Game::CheckCharacterCollision(Character& character) {
	//calculate foot position
	int xFoot = character.getX() + character.getAnim()->getSheet()->getFrameWidth() / 2 - 36;
	int yFoot = character.getY() + character.getAnim()->getSheet()->getFrameHeight() - 9;

	int xFootReal = character.getX() + character.getAnim()->getSheet()->getFrameWidth() / 2;
	int yFootReal = character.getY() + character.getAnim()->getSheet()->getFrameHeight() - 9;

	int mX = character.getDefferX();
	int mY = character.getDefferY();

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);
	//nBlockX = level->getSceneBlockXByCoord(xFootReal - 23);


	//===================== Fall ==========================
	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == '_' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '#' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '/' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '^' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '$' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '|' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '_') {

		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY);
		if (yFoot < bar - 20) {
			//mY += prince->setFall(); //TODO make part of Character
		}
		else {
			//prince->Land(); //TODO make part of character
		}
	}

	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == ' ' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '*') {
		//mY += prince->setFall(); TODO make this part of the character Class
	}

	//=============== Collision With Walls =====================
	if (mX < 0 && level->getSceneCodeByBlock(nBlockY, nBlockX) == ']') {
		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (xFoot + mX < bar) {
			mX = 0;
		}
	}

	if (mX > 0 && level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == '[' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '[') {

		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX)-27);
		if (DEBUG) graphics.DrawLine(bar, yFoot, bar, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

		if (xFoot + mX > bar) {
			mX = 0;
		}
	}

	

	//===================== Move Prince =====================
	character.MoveX(mX);
	character.MoveY(mY);

	char code = level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY());
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(prince->getMidX()));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(prince->getMidY()));
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	////================== Open/Close Gates ==================
	if (code == '-' ||
		code == '=') {


		std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();

		std::pair<int, int> platKey(absY, absX);
		std::pair<int, int> gateKey = (*mechanism)[platKey];

		//gateKey = make_pair(50, 4);

		if (entitites->find(gateKey) == entitites->end()) {
			return;
		}

		Entity* e = (*entitites)[gateKey];
		if (e->getType() == doorT) {
			FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			if (code == '-') {
				d->Open();
			}
		} else if (e->getType() == gateT) {
			Gate* g = dynamic_cast<Gate*>(e);
			if (code == '-') {
				g->Open();
			}
			else {
				g->Close();
			}
		}
	}




}
void Game::CheckPrinceCollision() {
	//calculate foot position
	int xFoot = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2 - 36;
	int yFoot = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;

	int xFootReal = prince->getX() + prince->getAnim()->getSheet()->getFrameWidth() / 2;
	int yFootReal = prince->getY() + prince->getAnim()->getSheet()->getFrameHeight() - 9;


	if (DEBUG) { graphics.DrawCircle(xFootReal, yFootReal, 5, 255, 255, 255); }

	int mX = prince->getDefferX();
	int mY = prince->getDefferY();

	int nBlockX = level->getSceneBlockXByCoord(xFoot);
	int nBlockY = level->getSceneBlockYByCoord(yFoot);
	//nBlockX = level->getSceneBlockXByCoord(xFootReal - 23);


	//===================== Fall ==========================
	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == '_' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '#' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '/' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '^' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '$' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '|' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '_') {

		int bar = (Level::BLOCK_HEIGHT_PX * nBlockY);
		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (yFoot < bar - 20) {
			mY += prince->setFall();
		}
		else {
			//prince->getAnim()->Play(); ???????????????
			prince->Land();
		}
	}

	if (level->getSceneCodeByBlock(nBlockY, nBlockX) == ' ' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '*') {
		mY += prince->setFall();
	}

	//=============== Collision With Walls =====================
	if (mX < 0 && level->getSceneCodeByBlock(nBlockY, nBlockX) == ']') {
		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX));

		if (DEBUG) graphics.DrawLine(0, bar, Graphics::SCREENX, bar, 255, 255, 255);

		if (xFoot + mX < bar) {
			mX = 0;
		}
	}

	if (mX > 0 && level->getSceneCodeByBlock(nBlockY, nBlockX + 1) == '[' ||
		level->getSceneCodeByBlock(nBlockY, nBlockX) == '[') {

		int bar = (Level::BLOCK_WIDTH_PX * (nBlockX)-27);
		if (DEBUG) graphics.DrawLine(bar, yFoot, bar, Level::BLOCK_HEIGHT_PX, 255, 255, 255);

		if (xFoot + mX > bar) {
			mX = 0;
		}
	}

	//===================== Change Scenes =====================
	if (prince->getMidY() > Level::BLOCK_HEIGHT_PX * 3) {
		level->changeScene(D);
		prince->setY(-60);
	}

	if (prince->getMidY() < 0) {
		level->changeScene(U);
		prince->setY(Level::BLOCK_HEIGHT_PX * 3 - 70);
	}

	if (xFoot + 60 > Level::BLOCK_WIDTH_PX * 10 + 5) {
		level->changeScene(R);
		prince->setX(-30);
	}

	if (xFoot < -5) {
		level->changeScene(L);
		prince->setX(Level::BLOCK_WIDTH_PX * 9 - 30);
	}


	//===================== Move Prince =====================
	prince->MoveX(mX);
	prince->MoveY(mY);

	char code = level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY());
	int absX = level->getLevelBlockX(level->getSceneBlockXByCoord(prince->getMidX()));
	int absY = level->getLevelBlockY(level->getSceneBlockYByCoord(prince->getMidY()));
	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	////================== Open/Close Gates ==================
	if (code == '-' ||
		code == '=') {


		std::map<std::pair<int, int>, std::pair<int, int> >* mechanism = level->getMec();

		std::pair<int, int> platKey(absY, absX);
		std::pair<int, int> gateKey = (*mechanism)[platKey];

		//gateKey = make_pair(50, 4);

		if (entitites->find(gateKey) == entitites->end()) {
			return;
		}

		Entity* e = (*entitites)[gateKey];
		if (e->getType() == doorT) {
			FinishDoor* d = dynamic_cast<FinishDoor*>(e);
			if (code == '-') {
				d->Open();
			}
		}
		else if (e->getType() == gateT) {
			Gate* g = dynamic_cast<Gate*>(e);
			if (code == '-') {
				g->Open();
			}
			else {
				g->Close();
			}
		}
	}

	////============== Spikes ==============
	/*
	for (int y = 0; nBlockY + y < 5; y++) {
	char c = level->getSceneCodeByBlock(nBlockY + y, nBlockX);
	if (c == '/') {

	std::pair<int, int> platKey(absY + y, absX);
	Entity* e = (*entitites)[platKey];
	Spikes* spikes = dynamic_cast<Spikes*>(e);
	spikes->On();
	}
	}
	*/



	if (level->findSpikes(absX, absY)) {

	}

	if (code == '/') {
		if (prince->getPrinceState() == sRunning ||
			prince->getPrinceState() == sFalling) {

			nBlockX = level->getSceneBlockXByCoord(prince->getMidX());
			nBlockY = level->getSceneBlockYByCoord(prince->getMidY());
			prince->setX((nBlockX - 1) * Level::BLOCK_WIDTH_PX - 15);
			prince->setY((nBlockY - 1)* Level::BLOCK_HEIGHT_PX);
			prince->spikeKill();
		}
	}


	////============== Guilotine ==============


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
		EngageFight(prince, guard);
	}


	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		guard->checkParryBy(prince);
		if (guard->isHitting(prince)) {
			prince->Hurt();
		}
	}

	for (std::list<Character*>::iterator i = guards->begin(); i != guards->end(); i++) {
		Character* guard = *i;
		if (guard->isDead()) { continue; }
		//if (!prince->checkParryBy(guard));
		if (prince->isHitting(guard)) {
			guard->Hurt();
		}
	}
}
void Game::CheckCollision() {
	CheckPrinceCollision();
	CheckCombatCollision();
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
		prince->defferMoveX(1);
	}

	if (input->getKeyStatus('O')) {
		prince->defferMoveX(-1);
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

		std::string s = "Code: ";
		std::string g(1, level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()));
		s += g + "\n";
		OutputDebugStringA(s.c_str());

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
void Game::DrawGraphics() {

	//graphics.BeginFrame();

	//Sprite* myDoor = getSprite("finishDoor");
	//int yCut = 100;
	//graphics.DrawSprite(100,100, myDoor, 0, 0 + yCut, myDoor->width, myDoor->height - yCut);

	std::map<std::pair<int, int>, Entity*>* entitites = level->getEntities();

	DrawBackground();
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		Entity* entity = i->second;
		Spikes* spikes = dynamic_cast<Spikes*>(entity);
		if (spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateBackground(&graphics);
		}
		else {
			i->second->Animate(NULL);
		}
	}
	if (DEBUG) { graphics.DrawCircle(prince->getMidX(), prince->getMidY(), 5, 255, 255, 255); }
	if (DEBUG) { graphics.DrawCircle(prince->getX(), prince->getY(), 5, 255, 255, 255); }
	
	for (std::map<std::pair<int, int>, Entity*>::iterator i = entitites->begin(); i != entitites->end(); i++) {
		if (i->second == NULL) { continue;  } // TODO: should not happen check load entities
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
		if(spikes != NULL && level->inScene(i->first.second, i->first.first)) {
			spikes->AnimateForeground(&graphics);
		}
		else {
			i->second->Animate(NULL);
		}
	}

	DrawHealth();

	//graphics.EndFrame();

}

void Game::EngageFight(Character* prince, Character* guard) {

	if (prince->getY() != guard->getY()) { return; }
	if (std::abs(prince->getX() - guard->getX()) > 10000) { return; }  //TODO: need to change this
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
	} else {
		int currentGuardDistance = std::sqrt((std::pow(guard->getX() - prince->getX(), 2.0) + std::pow(guard->getY() - prince->getY(), 2.0)));
		int engagedGuardDistance = std::sqrt((std::pow(engagedGuard->getX() - prince->getX(), 2.0) + std::pow(engagedGuard->getY() - prince->getY(), 2.0)));
		
		if (currentGuardDistance < engagedGuardDistance) {
			engagedGuard = guard;
		}
	}
}

//util
void Game::ComposeFrame() {

}
void Game::DrawHealth() {

	for(int i = 0; i < prince->getHealth(); i++ ) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(i * getSprite("healthFull")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}

	for(int i = prince->getHealth(); i < prince->getMaxHealth(); i++ ) {
		//graphics.DrawSprite(y * healthFull.width,  Level::BLOCK_HEIGHT_PX * 3, &healthEmpty );
		graphics.DrawSprite(i * getSprite("healthEmpty")->width, Level::BLOCK_HEIGHT_PX * 3, getSprite("healthEmpty"));
	}




	//std::list<Character*>* guards = level->getGuards();
	//std::list<Character*>::iterator i = guards->begin();
	//engagedGuard = *i;
	if (engagedGuard == NULL) { return;  };
	for (int i = 0; i < engagedGuard->getHealth(); i++) {
		//graphics.DrawSprite(y * healthFull.width, Level::BLOCK_HEIGHT_PX * 3, &healthFull );
		graphics.DrawSprite(Level::BLOCK_WIDTH_PX * 10 - (i + 1) * ( getSprite("healthFull")->width), Level::BLOCK_HEIGHT_PX * 3, getSprite("healthFull"));

	}









}
void Game::DrawBackground() {
	int yOff = 0;
	int xOff = 0;

	for(int y = Level::SCENE_HEIGHT_BLK; y >= 0; y--){
		for(int x = 0; x <= Level::SCENE_WIDTH_BLK; x++){

			yOff = Level::BLOCK_HEIGHT_PX * y;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * x;

			switch(level->getSceneCodeByBlock(y, x)) {
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
			case '~': {
					std::map<std::pair<int, int>, Entity*>* entities = level->getEntities();
					int blockX = level->getLevelBlockX(x);
					int blockY = level->getLevelBlockY(y);
					std::pair<int, int> platformKey = std::make_pair(blockY, blockX);
					Entity* e = entities->at(platformKey);
					Platform* p = dynamic_cast<Platform*>(e);
					if (p != NULL) {
						p->Animate(&graphics);
						if (p->getState() == dislodged) {
							p->setY(p->getY() + 4); //TODO needs to be a variable
							if (level->getSceneCodeByCoord(p->getX(), p->getY()) == '_') {
								level->newSetCodeByCoord(p->getX(), p->getY(), '$');
								level->setSceneCodeByBlock(x, y, ' ');
							}
						}
						
					}
					if (level->getSceneBlockXByCoord(prince->getMidX()) == x && level->getSceneBlockYByCoord(prince->getMidY() == y)) {
						p->Drop();
					}
				} break;

			case 'H':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
				break;
			case '-':
				if(level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == '-') {
					/*TODO: this needs to be fixed */
					if (level->getSceneBlockXByCoord(prince->getMidX()) == x && level->getSceneBlockYByCoord(prince->getMidY() == y)) {
						graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
					}
					else {
						graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
					}
				} else {
					graphics.DrawSprite(xOff, yOff - getSprite("activate")->height, getSprite("activate"));
				}
				break;
			case '=':
				if (level->getSceneCodeByCoord(prince->getMidX(), prince->getMidY()) == '=') {
					if (level->getSceneBlockXByCoord(prince->getMidX()) == x && level->getSceneBlockYByCoord(prince->getMidY() == y)) {
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
			case 'S':
				graphics.DrawSprite(xOff, yOff - getSprite("deadSk")->height, getSprite("deadSk"));
				break;
			case '^':
				graphics.DrawSprite(xOff, yOff - getSprite("tileCornerLeft")->height, getSprite("tileCornerLeft"));
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
			case '*':
				graphics.DrawSprite(xOff, yOff - getSprite("bricks")->height, getSprite("bricks"));
				break;
			case '/':
				graphics.DrawSprite(xOff, yOff - getSprite("holyFloor")->height, getSprite("holyFloor"));
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

	for(int i = 3; i >= 0; i--){
		for(int j = 0; j < 11; j++){

			yOff = Level::BLOCK_HEIGHT_PX * i;
			xOff = - Level::BLOCK_WIDTH_PX + Level::BLOCK_WIDTH_PX * j;

			switch(level->getSceneCodeByBlock(i, j)) {
		

			case '$':
				graphics.DrawSprite(xOff, yOff - getSprite("rubbleFront")->height, getSprite("rubbleFront"));
				break;
			case 'T':
				graphics.DrawSprite(xOff, yOff - getSprite("columnFront")->height, getSprite("columnFront"));
				break;
			case '[':
				graphics.DrawSprite(xOff, yOff - getSprite("blockCornerLeft")->height, getSprite("blockCornerLeft"));
				break;
			case 'G':
				graphics.DrawSprite(xOff, yOff - getSprite("doorFrameFront")->height, getSprite("doorFrameFront"));
				break;
			case 'I':
				graphics.DrawSprite(xOff, yOff - getSprite("block")->height, getSprite("block"));
				break;
			}
		}
	}
}
void Game::Reset() {
	level->loadLevel(1);

	prince = new Prince();
	prince->setX(10 * Level::BLOCK_WIDTH_PX);
	prince->setY(prince->getAnim()->getSheet()->getFrameHeight() - Level::FOOT_FLOAT);
	prince->setState(sIdle);
	
}

Game::~Game() {}

/* Debug information 
if(DEBUG) {
	std::string s;
	s = "P: " + std::to_string((long double)xFoot) + '.' + std::to_string((long double)yFoot);
	s += " | ";
	s += "B:" + std::to_string((long double)nBlockX) + '.' + std::to_string((long double)nBlockY);
	s += " | ";

	if(nBlockY >= 0 && nBlockX >= 0) {
		std::string g(1,scene[nBlockY][nBlockX - 1]);
		s += "M: " + g;
		std::string h(1,scene[nBlockY][nBlockX]);
		s += h;
		std::string y(1,scene[nBlockY][nBlockX + 1]);
		s += y;
	}
	s += "\n";

	OutputDebugStringA(s.c_str());
}

*/
/*
void Game::DrawLevelManualy() {
	int yOff;

	//level 1
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX * 3;

		//blocks
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y  * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height + 2, &blockCornerRight);

		//separatrs
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite((4 + y) * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}

		//floors
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite((4+ y) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//columns
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(5 * BLOCK_WIDTH_PX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);

		//blocks
		graphics.DrawSprite(9 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//level 2
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX * 2;

		//blocks
		for(int y = 0; y < 4; y++) {
			graphics.DrawSprite(0 + (9 - y) * BLOCK_WIDTH_PX, yOff - block.height, &block);
		}

		graphics.DrawSprite(0 + 5 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX - 1, yOff - blockCornerRight.height, &blockCornerRight);

		//separators
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX - 2, yOff - separator.height, &separator);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX - 4, yOff - separatorCorner.height + 2, &separatorCorner);
		
		//floors
		for(int y = 0; y< 3; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		
		//columns
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX + 15, yOff - columnFront.height - separator.height + 1, &columnFront);
		graphics.DrawSprite(3 * BLOCK_WIDTH_PX + 14 + columnFront.width, yOff - columnBack.height - separator.height - 7 + 1, &columnBack);


	//level 3
	yOff = TOP_MARGIN + BLOCK_HEIGHT_PX;

		//separators
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		graphics.DrawSprite(0  - BLOCK_WIDTH_PX - 4, yOff - separatorCorner.height + 2, &separatorCorner);

		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite(0 + (7 - y) * BLOCK_WIDTH_PX, yOff - separator.height, &separator);
			
		}

		//floors
		for(int y = 0; y< 5; y++) {
			graphics.DrawSprite(1 + (y + 3) * BLOCK_WIDTH_PX, yOff + 1 - separator.height - floor.height, &floor);
		}

		//blocks
		graphics.DrawSprite(0 + 9 * BLOCK_WIDTH_PX, yOff - block.height, &block);
		graphics.DrawSprite(0 + 8 * BLOCK_WIDTH_PX, yOff - blockCornerLeft.height, &blockCornerLeft);


	//Level 4

		//separators
		for(int y =0; y < SCENE_WIDTH_BLK; y++) {
			graphics.DrawSprite(y * BLOCK_WIDTH_PX, TOP_MARGIN - separator.height, &separator);
		}

}
*/
/*
LoadSprite(&block,            L"Assets//block.png"             );
LoadSprite(&blockCornerLeft,  L"Assets//blockCornerLeft.png"   );
LoadSprite(&blockCornerRight, L"Assets//blockCornerRight.png"  );
LoadSprite(&bricks,           L"Assets//bricks.png"            );
LoadSprite(&separator,        L"Assets//separator.png"         );
LoadSprite(&separatorCorner,  L"Assets//separator_corner.png"  );
LoadSprite(&floor,            L"Assets//floor.png"             );
LoadSprite(&trap,             L"Assets//trap.png"              );
LoadSprite(&activate,         L"Assets//activate.png"          );
LoadSprite(&healthFull,       L"Assets//healthFull.png"        );
LoadSprite(&healthEmpty,      L"Assets//healthEmpty.png"       );
LoadSprite(&columnFront,      L"Assets//columnFront.png"       );
LoadSprite(&columnBack,       L"Assets//columnBack.png"        );
LoadSprite(&rubbleFront,      L"Assets//rubbleBack.png"        );
LoadSprite(&rubbleBack,       L"Assets//rubbleFront.png"       );
LoadSprite(&gateFrameBack,    L"Assets//doorFrameBack.png"     );
LoadSprite(&gateFrameFront,   L"Assets//doorFrameFront.png"    );
LoadSprite(&tileCornerLeft,   L"Assets//tileCornerLeft.png"    );
LoadSprite(&deadSk,           L"Assets//deadSk.png"            );
LoadSprite(&gate,             L"Assets//gate.png"              );
LoadSprite(&holyFloor,        L"Assets//holyFloor.png"         );
*/