#include "Boss1.h"

Boss1::Boss1(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 260 }, mMaxHealth{ 260 },
    mAttackDamage{ 10.0f }, mAttackCooldown{ 0.75f }, isAlive { true },
    bossAttack { LoadTexture("assets/starMap.png") }
{
    mProjectiles = new Projectile[25];
    std::map<Direction, std::vector<int>> bossAnimationAtlas = { { RIGHT, { 0, 1, 2 } } };
    for (int i = 0; i < 25; ++i)
    {
        mProjectiles[i].setPosition(mPosition);
        mProjectiles[i].setScale({ 100, 100 });
        mProjectiles[i].setTexture(&bossAttack);
        mProjectiles[i].setEntityType(ATTACK);
        mProjectiles[i].setSpriteSheetDimensions({ 1, 3 });
        mProjectiles[i].setAnimationAtlas(bossAnimationAtlas);
        mProjectiles[i].setSpeed(750.0f);
        mProjectiles[i].setDamage(mAttackDamage);
        mProjectiles[i].setTextureType(ATLAS);
        mProjectiles[i].deactivate();

        mProjectiles[i].setColliderDimensions({
            mProjectiles[i].getScale().x / 2.0f,
            mProjectiles[i].getScale().y / 2.0f
        });
    }
}

Boss1::~Boss1() { delete [] mProjectiles; UnloadTexture(bossAttack); }

void Boss1::update(float deltaTime, Entity *player, Map *map,
                  Entity *collidableEntities, int collisionCheckCount)
{
    Entity::update(deltaTime, player, map, collidableEntities, collisionCheckCount);
    
    if (player) {
        setPosition({
            Lerp(getPosition().x, player->getPosition().x + 800.0f, deltaTime * 2.0f), 
            Lerp(getPosition().y, player->getPosition().y, deltaTime * 3.0f)
        });

        for (size_t i = 0; i < 25; ++i)
            mProjectiles[i].update(deltaTime, player);

        for (size_t i = 0; i < 25; ++i)
        {
            if (Vector2Distance(mPosition, mProjectiles[i].getPosition()) > 2560.0f)
                mProjectiles[i].deactivate();
        }

        if (!canShoot) 
        {
            mCounter += deltaTime;
            if (mCounter >= mAttackCooldown) 
                { canShoot = true; mCounter = 0.0f; }
        }
    }
}


void Boss1::shoot(Entity *player)
{
    if (!canShoot) return;
    canShoot = false;
    mCounter = 0.0f;

    for (int i = 0; i < 25; ++i)
    {
        if (!mProjectiles[i].isActive())
        {
            mProjectiles[i].setPosition(mPosition);
            mProjectiles[i].shootBoss1(player);
            break;
        }
    }
}

void Boss1::render()
{
    Entity::render();
    for (size_t i = 0; i < 25; ++i) mProjectiles[i].render();
}