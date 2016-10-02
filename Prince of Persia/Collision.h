#pragma once

class Prince;

class Collision {

public:
	Collision(Level* level, Graphics* graphics, Input * input);

	void CheckCollision();
	void CheckCharacterCollision(Character* character);
	void CheckPrinceCollision(Prince* prince);
	

private:
	Level* level;
	Graphics* graphics;
	Timer* timer;
	Input* input;

	bool CheckCatchConditions(Prince* prince);
	void CheckLedgeClimb(Prince* prince);
	void CheckSceneChange(Prince* prince);
	void CheckFall(Character* prince);
	void CheckStepDanger(Prince* prince);
	
	void CheckWallCollision(Character* prince);
	void CheckGateCollision(Character* prince);
	void CheckSpikeCollision(Character* prince);

};