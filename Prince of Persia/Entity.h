#pragma once

#include "Animation.h"


enum entityType : int { noneT, torchT, guilotineT, gateT, potionT, poisonPotT, extendPotT, spikeT, swordT, guardT, doorT, platformT };

class Entity {

// Variables
protected:
    Animation* currentAnim;

    double xPos;
    double yPos;

    int levelX;
    int levelY;

    int sceneX;
    int sceneY;

    double defferX;
    double defferY;

    double accelerationX;
    double accelerationY;

    entityType type;

// Methods
public:
    // Constructors
    Entity(Animation* anim,int x, int y, entityType t = noneT);
    Entity();


    //setters
    void setAccX(double x);
    void setAccY(double y);
    
    void setX(int x);
    void setY(int y);

    void setLevelX(int x);
    void setLevelY(int y);

    void setSceneX(int x);
    void setSceneY(int y);

    void setDefferX(double x);
    void setDefferY(double y);

    void setCurrentAnim(Animation* anim);

    //getters
    Animation* getAnim();

    double getAccX();
    double getAccY();

    int getX();
    int getY();

    int getLevelX();
    int getLevelY();

    int getSceneX();
    int getSceneY();

    int getMidX();
    int getMidY();

    int getDefferX();
    int getDefferY();
    
    // Functions
    virtual void Animate(Graphics* graphics, int& moveX, int& moveY);
    virtual void Animate(Graphics* graphics);

    void setType(entityType t);
    entityType getType();

    virtual void EventHandler(int Event) {}

    void MoveX(double x);
    void MoveY(double y);

    // Destructor
    ~Entity();
};
