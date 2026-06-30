#include "Boss3.h"

Boss3::Boss3() : Entity(), mCurrHealth{ 800.0f }, mMaxHealth{ 800.0f },
    mAttackDamage{ 10.0f }, mAttackCooldown{ 0.3f }, isAlive { true } {}

Boss3::Boss3(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 100 }, mMaxHealth{ 100 },
    mAttackDamage{ 10.0f }, mAttackCooldown{ 0.3f }, isAlive { true } {}

Boss3::Boss3(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
    Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType) :
    Entity(position, scale, textureFilepath, textureType, spriteSheetDimensions, animationAtlas, entityType),
    mCurrHealth{ 100 }, mMaxHealth{ 100 }, mAttackDamage{ 10.0f }, mAttackCooldown{ 0.5f }, isAlive { true } {}

Boss3::~Boss3() {}

void Boss3::update(float deltaTime, Entity *player, Map *map,
                  Entity *collidableEntities, int collisionCheckCount)
{
    Entity::update(deltaTime, player, map, collidableEntities, collisionCheckCount);
    ++mCircleCounter;
    
    if (player) {
        for (size_t i = 0; i < mProjectiles.size(); ++i)
        mProjectiles[i]->update(deltaTime, player);

    for (size_t i = 0; i < mProjectiles.size(); ++i)
    {
        if (Vector2Distance(mPosition, mProjectiles[i]->getPosition()) > 2560.0f)
            mProjectiles[i]->deactivate();

        if (!mProjectiles[i]->isActive()) 
        {
            delete mProjectiles[i];
            mProjectiles[i] = nullptr;
            mProjectiles.erase(mProjectiles.begin() + i);
            --i;
        }
    }

    if (!canShoot) 
    {
        mCounter += deltaTime;
        if (mCounter >= mAttackCooldown) 
            { canShoot = true; mCounter = 0.0f; }
    }
    }
}

std::map<Direction, std::vector<int>> boss3AnimationAtlas = { { RIGHT, { 0, 1, 2 } } };

void Boss3::shoot(Entity *player)
{
    if (!canShoot) return;
    canShoot = false;
    mCounter = 0.0f;

    mProjectiles.push_back(
        new Projectile{
            mPosition, { 100, 100 }, "assets/starMap.png", ATLAS, { 1, 3 },
            boss3AnimationAtlas, ATTACK, 500.0f, mAttackDamage
        }
    );
    
    mProjectiles[mProjectiles.size()-1]->shootBoss3(player, mCircleCounter);
    ++mCircleCounter;
}

void Boss3::render()
{
    Entity::render();
    for (size_t i = 0; i < mProjectiles.size(); ++i) mProjectiles[i]->render();
}