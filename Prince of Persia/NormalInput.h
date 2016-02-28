#pragma once

class Input;

class NormalInput {
private:
	Input* input;

public:
	NormalInput(Input* input);
	bool doLeftStep();
	bool doRightStep();
	bool doRightJump();
	bool doLeftJump();
	bool doClimb();
	bool goDown();
};