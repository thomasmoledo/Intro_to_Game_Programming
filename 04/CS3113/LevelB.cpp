#include "LevelB.h"

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() { shutdown(); }

bool LevelB::takeDamage( int *currHealth ) {
    --(*currHealth);
    PlaySound(mGameState.hitSound);
    return *currHealth <= 0;
}

void LevelB::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.jumpSound = LoadSound("assets/marioJump.mp3");
    SetSoundVolume(mGameState.jumpSound, 0.15f);
    mGameState.hitSound = LoadSound("assets/hitSound.mp3");
    SetSoundVolume(mGameState.hitSound, 0.35f);

    // Map
    mGameState.map = new Map(
        LEVEL_WIDTH,
        LEVEL_HEIGHT,                 // map grid cols & rows
        (unsigned int *) mLevelData,  // grid data
        "assets/terrain.png",         // texture filepath
        TILE_DIMENSION,               // tile size
        22,                           // texture cols
        11,                           // texture rows
        mOrigin                       // in-game origin
    );

    // Player
     std::map<Direction, std::vector<int>> protagAnimationAtlas = {
        { IDLE_RIGHT, {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 } },
        { IDLE_LEFT,  { 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11 } },
        { RIGHT,      { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 } },
        { LEFT,       { 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33 } },
        { JUMP_RIGHT, { 56 } }, { FALL_RIGHT, { 55 } },
        { JUMP_LEFT,  { 67 } }, { FALL_LEFT, { 68 } }
    };

    float sizeRatio = 5.0f / 6.0f;

    mGameState.protagonist = new Entity(
        { -150.0f, 250.0f },                        // position
        { 125.0f * sizeRatio, 125.0f },             // scale
        "assets/protagSpriteSheet.png",             // texture file address
        ATLAS,                                      // single image or atlas?
        { 6, 11 },                                  // atlas dimensions
        protagAnimationAtlas,                       // actual atlas
        PLAYER                                      // entity type
    );

    mGameState.protagonist->setJumpingPower(550.0f);
    mGameState.protagonist->setColliderDimensions(
        { mGameState.protagonist->getScale().x / 1.5f,
          mGameState.protagonist->getScale().y / 1.0f }
    );
    mGameState.protagonist->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    // Flyer
        std::map<Direction, std::vector<int>> flyerAnimationAtlas = {
        {LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }},
        {RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }},
    };

    mGameState.flyer = new Entity(
        { 500.0f, ORIGIN.y + 100.0f},           // position
        {100.0f, 100.0f},                       // scale
        "assets/Strawberry.png",                // texture file address
        ATLAS,                                  // single image or atlas?
        { 1, 17 },                              // atlas dimensions
        flyerAnimationAtlas,                    // actual atlas
        NPC                                     // entity type
    );

    mGameState.flyer->setAIType(FLYER);
    mGameState.flyer->setAIState(FOLLOWING);

    mGameState.flyer->setColliderDimensions({
        mGameState.flyer->getScale().x / 2.0f,
        mGameState.flyer->getScale().y / 2.0f
    });

    mGameState.flyer->setDirection(LEFT);
    mGameState.flyer->render();
}

void LevelB::update(float deltaTime) {}

void LevelB::update(float deltaTime, int *currHealth)
{
    mGameState.protagonist->update(
            deltaTime,      // delta time / fixed timestep
            nullptr,        // player
            mGameState.map, // map
            nullptr,        // collidable entities
            0               // col. entity count
        );
    
    if (mGameState.protagonist->isColliding(mGameState.flyer))
    {
        if (takeDamage(currHealth))
        {
            *currHealth = 3;
            mGameState.nextSceneID = LOSE_SCREEN;
        } 

        else mGameState.nextSceneID = LEVEL_B;
    }

    mGameState.flyer->update(
            deltaTime,              // delta time / fixed timestep
            nullptr,                // player
            nullptr,                // map
            nullptr,                // collidable entities
            0                       // col. entity count
        );
    
    if (mGameState.protagonist->getPosition().x > 1050.0f)
        mGameState.nextSceneID = LEVEL_C;

    if (mGameState.protagonist->getPosition().y > END_GAME_THRESHOLD)
    {
        if (takeDamage(currHealth))
        {
            *currHealth = 3;
            mGameState.nextSceneID = LOSE_SCREEN;
        } 

        else mGameState.nextSceneID = LEVEL_B; 
    }
        
}

void LevelB::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    // Render the player entity and map
    mGameState.protagonist->render();
    mGameState.flyer->render();
    mGameState.map->render();
}

void LevelB::shutdown()
{
    // delete mGameState.protagonist and mGameState.map, then unload bgm and jumpSound
    delete mGameState.protagonist;
    delete mGameState.flyer;
    delete mGameState.map;

    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.hitSound);
}
