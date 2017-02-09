#pragma once;


#include "Entity.h"
#include "Audio.h"

#include <map>


class Level;


enum Action { aLeftJump, aRightJump, aJumpGrab, aCrouch, aGoRight, aGoLeft, aClimbUp, aClimbDown, aLeftStep, aRightStep, aHang, aNone, aStrike, aParry, aEngage, aDisengage };
enum characterState { sRunning, sFalling, sIdle, sDead, sFinish, sJumping, sJumpGrab, sClimbUp, sStep, sRunningTurn };

class Character : public Entity {
//Functions
public:
    Character();
    Character(Audio* audio);
    void Hurt();
    void Heal();

    int getHealth();
    int getMaxHealth();

    void SwitchFacing();
    bool isImmune();
    bool isParying();
    bool isHitting(Character * enemy, Level & level);
    bool checkParryBy(Character* enemy);
    bool isFighting();
    bool isIdle();
    bool isDead();
    
    bool isFacingRight();
    void FaceCharacter(Character& character, Level& level);

    void EngageEnemy(Character & enemy);

    bool isInScene();
    bool isMovingUp();
    void setInScene(bool val);

    characterState getState();
    void setState(characterState state);
    void spikeKill();
    void Land(int currentBlockY);
    void setFall(int currentBlockY);

    Animation* turn;
    Animation* running;
    Animation* climbUp;
    Animation* crouch;
    Animation* jumpGrab;
    Animation* staticJump;
    Animation* step;
    Animation* hang;
    Animation* runningJump;
    Animation* runningTurn;
    Animation* fall;
    Animation* drop;
    Animation* drink;
    Animation* pickSword;
    

protected:
    void defaultToIdle();

protected:
    Animation* idle;

    Animation* fightIdle;
    Animation* fightStep;
    Animation* fightParry;
    Animation* fightParried;
    Animation* fightStrike;

    Animation* fightInjure;
    Animation* fightDying;
    Animation* fightStart;
    Animation* fightFinish;

    Animation* swordDeath;
    Animation* guilotineDeath;
    Animation* spikeDeath;

    std::map<Character*, bool> seenEnemies;

    characterState state;
    bool hasSword;

//variables
protected:
    bool isAnimating;
    bool facingRight;
    bool inFight;
    bool dead;

    bool inScene;

    int maxHealth;
    int currentHealth;
    int lastBlockY;

    Audio* audio;
    
public:

};
