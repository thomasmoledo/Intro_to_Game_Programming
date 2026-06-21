#include "Entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *protagonist = nullptr;
    Entity *lerper = nullptr;
    Entity *flyer = nullptr;
    Entity *charger = nullptr;
    
    Map *map = nullptr;

    Sound jumpSound;
    Sound hitSound;
    Sound clickSound;
    Sound winSound;
    Sound loseSound;

    SceneID nextSceneID = NO_SCENE;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void update(float deltaTime, int *currHealth) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif
