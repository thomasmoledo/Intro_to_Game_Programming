#include "Boss2.h"

Boss2::Boss2(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 300 }, mMaxHealth{ 300 },
    mAttackDamage{ 25.0f }, mAttackCooldown{ 5.0f }, mCounterAttack{ 0.0f },
    mCounterCooldown{ 0.0f }, isAlive{ true }, canAttack{ true }, isMidAttack{ false },
    mNoDamageWindow{ 3.0f }, mDamageWindow{ 2.0f }, mDamageTextureApplied{ false },
    verticalGridTexture1{ LoadTexture("assets/gridLineVertical1.png") },
    verticalGridTexture2{ LoadTexture("assets/gridLineVertical2.png") },
    horizontalGridTexture1{ LoadTexture("assets/gridLineHorizontal1.png") },
    horizontalGridTexture2{ LoadTexture("assets/gridLineHorizontal2.png") }
{
    damageStart = LoadSound("assets/phase2Explosion.mp3");
    SetSoundVolume(damageStart, 0.2f);

    mGridLines = new Entity[GRID_LINE_COUNT];
    for (int i = 0; i < GRID_LINE_COUNT; ++i)
    {
        if (i % 2 == 0)
        {
            mGridLines[i].setPosition({ 0.0f, 0.0f });
            mGridLines[i].setScale({ 40.0f, 4000.0f });
            mGridLines[i].setColliderDimensions({ 40.0f, 4000.0f });
            mGridLines[i].setTexture(&verticalGridTexture1);
            mGridLines[i].setEntityType(VERTICAL);
            mGridLines[i].deactivate();
        
        }
        else
        {
            mGridLines[i].setPosition({ 0.0f, 0.0f });
            mGridLines[i].setScale({ 4000.0f, 40.0f });
            mGridLines[i].setColliderDimensions({ 4000.0f, 40.0f });
            mGridLines[i].setTexture(&horizontalGridTexture1);
            mGridLines[i].setEntityType(HORIZONTAL);
            mGridLines[i].deactivate();
        }
            
    }

}

Boss2::~Boss2()
{
    UnloadSound(damageStart);
    UnloadTexture(verticalGridTexture1);
    UnloadTexture(verticalGridTexture2);
    UnloadTexture(horizontalGridTexture1);
    UnloadTexture(horizontalGridTexture2);
    delete[] mGridLines;
}

void Boss2::spawnGrid(Entity *player)
{
    if (!player) return;
    for (int i = 0; i < GRID_LINE_COUNT; ++i) mGridLines[i].activate();
    repositionGrid(player);
    setGridTexture(false);
}

void Boss2::repositionGrid(Entity *player)
{
    if (!player) return;

    const float spacing = 400.0f;
    Vector2 centre = player->getPosition();

    size_t j = 0;
    for (int i = -7; i <= 7; ++i)
    {
        mGridLines[j++].setPosition({ centre.x + (i * spacing), centre.y });
        mGridLines[j++].setPosition({ centre.x, centre.y + (i * spacing) });
    }
}

void Boss2::setGridTexture(bool isDamaging)
{
    for (int i = 0; i < GRID_LINE_COUNT; ++i)
    {
        if (mGridLines[i].getEntityType() == VERTICAL)
            mGridLines[i].setTexture(isDamaging ? &verticalGridTexture2 : &verticalGridTexture1);
        else
            mGridLines[i].setTexture(isDamaging ? &horizontalGridTexture2 : &horizontalGridTexture1);
    }
    if (isDamaging) PlaySound(damageStart);
}

void Boss2::despawnGrid()
{
    for (int i = 0; i < GRID_LINE_COUNT; ++i) mGridLines[i].deactivate();
}

bool Boss2::checkGridCollision(Entity *player) const
{
    if (!player) return false;

    Vector2 playerPos = player->getPosition();
    Vector2 playerDim = player->getColliderDimensions();

    for (int i = 0; i < GRID_LINE_COUNT; ++i)
    {
        if (!mGridLines[i].isActive()) continue;

        Vector2 barPos = mGridLines[i].getPosition();
        Vector2 barDim = mGridLines[i].getColliderDimensions();

        float xDistance = fabs(barPos.x - playerPos.x) -
                           ((barDim.x + playerDim.x) / 2.0f);
        float yDistance = fabs(barPos.y - playerPos.y) -
                           ((barDim.y + playerDim.y) / 2.0f);

        if (xDistance < 0.0f && yDistance < 0.0f) return true;
    }

    return false;
}

void Boss2::update(float deltaTime, Entity *player, Map *map,
                  Entity *collidableEntities, int collisionCheckCount)
{
    Entity::update(deltaTime, player, map, collidableEntities, collisionCheckCount);

    if (!isMidAttack)
    {
        setPosition({
            Lerp(getPosition().x, player->getPosition().x + 800.0f, deltaTime * 2.0f), 
            Lerp(getPosition().y, player->getPosition().y - 100.0f, deltaTime * 3.0f)
        });
        mCounterCooldown += deltaTime;

        if (mCounterCooldown >= mAttackCooldown)
        {
            mCounterCooldown      = 0.0f;
            mCounterAttack        = 0.0f;
            isMidAttack           = true;
            mDamageTextureApplied = false;
            spawnGrid(player);
        }
    }
    else
    {
        setPosition({player->getPosition().x + 1000.f, player->getPosition().y - 1000.0f});
        mCounterAttack += deltaTime;

        bool inTelegraphPhase = mCounterAttack < mNoDamageWindow;
        bool inDamagePhase    = !inTelegraphPhase &&
            mCounterAttack < (mNoDamageWindow + mDamageWindow);

        if (inDamagePhase)
        {
            if (!mDamageTextureApplied)
            {
                setGridTexture(true);
                mDamageTextureApplied = true;
            }

            if (player && checkGridCollision(player))
                player->takeDamage(mAttackDamage * deltaTime);
        }

        if (mCounterAttack >= (mNoDamageWindow + mDamageWindow))
        {
            despawnGrid();
            isMidAttack            = false;
            mCounterAttack         = 0.0f;
            mDamageTextureApplied  = false;
        }
    }
}

void Boss2::render()
{
    Entity::render();
    for (int i = 0; i < GRID_LINE_COUNT; ++i) mGridLines[i].render();
}