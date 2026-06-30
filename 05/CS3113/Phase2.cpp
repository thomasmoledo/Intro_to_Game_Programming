#include "Phase2.h"

Phase2::Phase2()                                      : Scene{ {},     nullptr   } {}
Phase2::Phase2(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}

Phase2::~Phase2() { shutdown(); }

void Phase2::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    background = LoadTexture("assets/phase2Background.png");
    bossHealthBar = LoadTexture("assets/HealthBar2.png");
    playerHealthBar = LoadTexture("assets/HealthBarP.png");

/* ----------- PROTAGONIST ----------- */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { LEFT,  { 4, 5, 6 }},
        { UP,    { 0, 1, 2, 3 }},
        { RIGHT, { 7, 8, 9 }},
    };

    mGameState.player = new Player(
        { mOrigin.x, mOrigin.y },                    // position
        { 250.0f, 250.0f },                          // scale
        "assets/playerModel.png",                    // texture file address
        ATLAS,                                       // single image or atlas?
        { 3, 4 },                                    // atlas dimensions
        playerAnimationAtlas,                        // actual atlas
        PLAYER                                       // entity type
    );

    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x / 3.5f,
        mGameState.player->getScale().y / 3.0f
    });

    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.player->setSpeed(300.0f);
    mGameState.player->setAttackDamage(5.0f);

    /* ----------- STARBOARD ----------- */
    float sizeRatioStarboard = 1600.0f / 220.0f;

    mGameState.starboard = new Entity(
        { mOrigin.x, mOrigin.y + 200.0f },           // position
        { 100.0f * sizeRatioStarboard, 100.0f },     // scale
        "assets/starboard.png",                      // texture file address
        PLATFORM                                     // entity type
    );

    mGameState.starboard->setColliderDimensions({
        mGameState.starboard->getScale().x,
        mGameState.starboard->getScale().y / 3.0f
    });

    /* ----------- BOSS ----------- */
    float sizeRatioBoss = 1.0f;

    mGameState.boss = new Boss2(
        { mOrigin.x + 200.0f, mOrigin.y - 200.0f }, // position
        { 300.0f * sizeRatioBoss, 300.0f },         // scale
        "assets/cat2.png",                          // texture file address
        BOSS                                        // entity type
    );

    bossHealthRatio = mGameState.boss->getCurrHealth() / mGameState.boss->getMaxHealth();
}

void Phase2::update(float deltaTime)
{
    mGameState.player->update(
        deltaTime,            // delta time / fixed timestep
        mGameState.boss,      // boss
        nullptr,              // map
        mGameState.starboard, // collidable entities
        1                     // col. entity count
    );

    mGameState.starboard->update(
        deltaTime,         // delta time / fixed timestep
        mGameState.player, // player
        nullptr,           // map
        nullptr,           // collidable entities
        0                  // col. entity count
    );

    mGameState.starboard->setPosition({
        mGameState.player->getPosition().x,
        mGameState.starboard->getPosition().y
    });

    mGameState.boss->update(
        deltaTime,            // delta time / fixed timestep
        mGameState.player,    // player
        nullptr,              // map
        nullptr,              // collidable entities
        0                     // col. entity count
    );

    bossHealthRatio = mGameState.boss->getCurrHealth() / mGameState.boss->getMaxHealth();
    playerHealthRatio = mGameState.player->getCurrHealth() / mGameState.player->getMaxHealth();
    if (bossHealthRatio <= 0.0f) mGameState.nextSceneID = PHASE_3;
    if (playerHealthRatio <= 0.0f) mGameState.nextSceneID = LOSE_SCREEN;
}

void Phase2::render()
{
    mGameState.starboard->render();
    mGameState.boss->render();
    mGameState.player->render();
}

void Phase2::renderBackground()
{
    renderObject(&background, &ORIGIN, &SCREEN_SIZE);
}

void Phase2::renderUI()
{
    // renderObject(&playerHealthBar, &ORIGIN, &SCREEN_SIZE);
    DrawRectangleGradientH(565 * RES_RATIO, 832 * RES_RATIO,
                           540 * RES_RATIO * bossHealthRatio, 30 * RES_RATIO,
                           (Color){73, 0, 205, 255}, (Color){116, 64, 255, 255});

    renderObject(&bossHealthBar, &ORIGIN, &SCREEN_SIZE);

    DrawRectangleGradientH(567 * RES_RATIO, 70 * RES_RATIO,
                           473 * RES_RATIO * playerHealthRatio, 40 * RES_RATIO,
                           (Color){255, 246, 230, 255}, (Color){255, 255, 255, 255});
                           
    renderObject(&playerHealthBar, &ORIGIN, &SCREEN_SIZE);
}

void Phase2::shutdown()
{
    UnloadTexture(background);
    UnloadTexture(bossHealthBar);
    UnloadTexture(playerHealthBar);
    delete mGameState.player;
    mGameState.player = nullptr;
    delete mGameState.starboard;
    mGameState.starboard = nullptr;
    delete mGameState.boss;
    mGameState.boss = nullptr;
}