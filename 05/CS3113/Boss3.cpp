#include "Boss3.h"

Boss3::Boss3(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 300 }, mMaxHealth{ 300 },
    mAttackDamage{ 10.0f }, mAttackCooldown{ 0.1f }, isAlive { true }, mCircleCounter { 5.25f }, mCounter { 0.0f },
    canShoot { true },
    bossAttack { LoadTexture("assets/starMap.png") }
{
    mProjectiles = new Projectile[100];
    std::map<Direction, std::vector<int>> bossAnimationAtlas = { { RIGHT, { 0, 1, 2 } } };
    for (int i = 0; i < 100; ++i)
    {
        mProjectiles[i].setPosition(mPosition);
        mProjectiles[i].setScale({ 100, 100 });
        mProjectiles[i].setTexture(&bossAttack);
        mProjectiles[i].setEntityType(ATTACK);
        mProjectiles[i].setSpriteSheetDimensions({ 1, 3 });
        mProjectiles[i].setAnimationAtlas(bossAnimationAtlas);
        mProjectiles[i].setSpeed(350.0f);
        mProjectiles[i].setDamage(mAttackDamage);
        mProjectiles[i].setTextureType(ATLAS);
        mProjectiles[i].deactivate();

        mProjectiles[i].setColliderDimensions({
            mProjectiles[i].getScale().x / 2.0f,
            mProjectiles[i].getScale().y / 2.0f
        });
    }
}

Boss3::~Boss3() { delete [] mProjectiles; UnloadTexture(bossAttack); }

void Boss3::update(float deltaTime, Entity *player, Map *map,
                  Entity *collidableEntities, int collisionCheckCount)
{
    Entity::update(deltaTime, player, map, collidableEntities, collisionCheckCount);
    ++mCircleCounter;
    
    if (player) {
        for (int i = 0; i < 100; ++i)
            mProjectiles[i].update(deltaTime, player);

        for (int i = 0; i < 100; ++i)
        {
            if (Vector2Distance(mPosition, mProjectiles[i].getPosition()) > 2560.0f)
                mProjectiles[i].deactivate();
        }
    }

    if (!canShoot) 
    {
        mCounter += deltaTime;
        if (mCounter >= mAttackCooldown) 
            { canShoot = true; mCounter = 0.0f; }
    }
}

void Boss3::shoot(Entity *player)
{
    if (!canShoot) return;
    canShoot = false;
    mCounter = 0.0f;
    
    for (int i = 0; i < 100; ++i)
    {
        if (!mProjectiles[i].isActive())
        {
            mProjectiles[i].setPosition(mPosition);
            mProjectiles[i].shootBoss3(player, mCircleCounter);
            break;
        }
    }
    ++mCircleCounter;
}

void Boss3::render()
{
    Entity::render();
    for (int i = 0; i < 100; ++i) mProjectiles[i].render();
}