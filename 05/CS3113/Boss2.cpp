#include "Boss2.h"

Boss2::Boss2() : Entity(), mCurrHealth{ 100 }, mMaxHealth{ 100 },
    mAttackDamage{ 25.0f }, mAttackCooldown{ 5.0f }, mCounterAttack{ 0.0f },
    mCounterCooldown{ 0.0f }, isAlive{ true }, canAttack{ true }, isMidAttack{ false },
    mNoDamageWindow{ 3.0f }, mDamageWindow{ 2.0f } 
    {
        damageStart = LoadSound("assets/phase2Explosion.mp3");
        SetSoundVolume(damageStart, 0.2f);
    }

Boss2::Boss2(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 100 }, mMaxHealth{ 100 },
    mAttackDamage{ 25.0f }, mAttackCooldown{ 5.0f }, mCounterAttack{ 0.0f },
    mCounterCooldown{ 0.0f }, isAlive{ true }, canAttack{ true }, isMidAttack{ false },
    mNoDamageWindow{ 3.0f }, mDamageWindow{ 2.0f } 
    {
        damageStart = LoadSound("assets/phase2Explosion.mp3");
        SetSoundVolume(damageStart, 0.2f);
    }

Boss2::Boss2(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
    Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType) :
    Entity(position, scale, textureFilepath, textureType, spriteSheetDimensions, animationAtlas, entityType),
    mCurrHealth{ 100.0f }, mMaxHealth{ 100.0f }, mAttackDamage{ 25.0f }, mAttackCooldown{ 5.0f },
    mCounterAttack{ 0.0f }, mCounterCooldown{ 0.0f }, isAlive{ true }, canAttack{ true }, isMidAttack{ false },
    mNoDamageWindow{ 3.0f }, mDamageWindow{ 2.0f } 
    {
        damageStart = LoadSound("assets/phase2Explosion.mp3");
        SetSoundVolume(damageStart, 0.2f);
    }

Boss2::~Boss2()
{
    for (Entity *bar : mGridBars) delete bar;
    mGridBars.clear();
}

void Boss2::spawnGrid(Entity *player)
{
    if (mGridBars.empty())
    {
        const float barThickness        = 40.0f;
        const float verticalBarHeight   = 2000.0f;
        const float horizontalBarWidth  = 4000.0f;

        for (int i = -5; i <= 11; ++i)
        {
            mGridBars.push_back(new Entity(
                { 0.0f, 0.0f },
                { barThickness, verticalBarHeight },
                "assets/gridLineVertical1.png",
                VERTICAL
            ));

            mGridBars.push_back(new Entity(
                { 0.0f, 0.0f },
                { horizontalBarWidth, barThickness },
                "assets/gridLineHorizontal1.png",
                HORIZONTAL
            ));
        }
    }

    repositionGrid(player);
    setGridTexture(false);

    for (Entity *bar : mGridBars) bar->activate();
}

void Boss2::repositionGrid(Entity *player)
{
    if (!player) return;

    const float spacing = 400.0f;
    Vector2 centre = player->getPosition();

    size_t idx = 0;
    for (int i = -5; i <= 11; ++i)
    {
        mGridBars[idx++]->setPosition({ centre.x + (i * spacing), centre.y });
        mGridBars[idx++]->setPosition({ centre.x, centre.y + (i * spacing) });
    }
}

void Boss2::setGridTexture(bool isDamaging)
{
    for (Entity *bar : mGridBars)
    {
        if (bar->getEntityType() == VERTICAL)
            bar->setTexture(isDamaging ? "assets/gridLineVertical2.png" : "assets/gridLineVertical1.png");
        else
            bar->setTexture(isDamaging ? "assets/gridLineHorizontal2.png" : "assets/gridLineHorizontal1.png");
        
    } if (isDamaging) PlaySound(damageStart);
}

void Boss2::despawnGrid()
{
    for (Entity *bar : mGridBars) bar->deactivate();
}

bool Boss2::checkGridCollision(Entity *player) const
{
    if (!player) return false;

    Vector2 playerPos = player->getPosition();
    Vector2 playerDim = player->getColliderDimensions();

    for (Entity *bar : mGridBars)
    {
        if (!bar->isActive()) continue;

        Vector2 barPos = bar->getPosition();
        Vector2 barDim = bar->getColliderDimensions();

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
            // Swap to the "danger" sprite exactly once when the damage
            // phase begins, not every frame.
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

    for (Entity *bar : mGridBars)
    {
        if (!bar->isActive()) continue;
        bar->render();
    }
}