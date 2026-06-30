#ifndef SCENE_H
#define SCENE_H

#include "Boss3.h"

struct GameState
{
    Entity *player = nullptr;
    Entity *boss = nullptr;
    Map    *map = nullptr;
    Entity *starboard = nullptr;
    Entity *buttons = nullptr;

    Sound jumpSound;
    Sound clickSound;

    SceneID nextSceneID;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void renderBackground() = 0;
    virtual void renderUI() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;       }
    Vector2     getOrigin()          const { return mOrigin;          }
    const char *getBGColourHexCode() const { return mBGColourHexCode; }

    // MAIN_MENU
    virtual void showControls() { return; }
    virtual void hideControls() { return; }
};

#endif // SCENE_H
