#include "Input.h"
#include "NormalInput.h"

NormalInput::NormalInput(Input* input) {
    this->input = input;
}

bool NormalInput::doLeftStep() {
    if (input->isShiftPressed() && input->isLeftPressed()) {
        return true;
    }
    return false;
}

bool NormalInput::doRightStep() {
    if (input->isShiftPressed() && input->isRightPressed()) {
        return true;
    }
    return false;
}


bool NormalInput::doRightJump() {
    if (input->isUpPressed() && input->isRightPressed()) {
        return true;
    }
    return false;
}

bool NormalInput::doLeftJump() {
    if (input->isUpPressed() && input->isLeftPressed()) {
        return true;
    }
    return false;
}

bool NormalInput::doClimb() {
    if (input->isUpPressed()     && 
        !input->isRightPressed() &&
        !input->isLeftPressed() ) {
        return true;
    }
    return false;
}

bool NormalInput::goDown() {
    if (input->isDownPressed()) {
        return true;
    }
    return false;
}

bool NormalInput::goLeft() {
    if (input->isLeftPressed() &&
        !input->isUpPressed() ) {
        return true;
    }
    return false;
}

bool NormalInput::goRight() {
    if (input->isRightPressed() &&
        !input->isUpPressed() ) {
        return true;
    }
    return false;
}

bool NormalInput::Strike() {
    if (input->isCtrlPressed()) {
        return true;
    }
    return false;
}

bool NormalInput::Hang()
{
    if (input->isShiftPressed()) {
        return true;
    }
    return false;
}

