#include "Player.h"

Player::Player() : Entity(), mCurrHealth{ 100.0f }, mMaxHealth{ 100.0f }, mAttackDamage{ 10.0f },
    mAttackCooldown{ 0.5f }, mDragFactorX{ 0.5f }, mDragFactorY { 0.00003f } {}

Player::Player(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType) :
    Entity(position, scale, textureFilepath, entityType), mCurrHealth{ 100.0f }, mMaxHealth{ 100.0f },
    mAttackDamage{ 10.0f }, mAttackCooldown{ 0.5f }, mDragFactorX{ 0.5f }, mDragFactorY { 0.00003f } {}

Player::Player(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
    Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType) :
    Entity(position, scale, textureFilepath, textureType, spriteSheetDimensions, animationAtlas, entityType), 
    mCurrHealth{ 100.0f }, mMaxHealth{ 100.0f }, mAttackDamage{ 10.0f }, mAttackCooldown{ 0.5f },
    mDragFactorX{ 0.5f }, mDragFactorY { 0.00003f } {}

Player::~Player() 
{
    for (Projectile *p : mProjectiles) delete p;
}

void Player::update(float deltaTime, Entity *boss, Map *map,
                    Entity *collidableEntities, int collisionCheckCount)
{
    if (mEntityStatus == INACTIVE) return;

    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    mVelocity.x *= (1.0f - mDragFactorX * deltaTime);
    mVelocity.y *= (1.0f - mDragFactorY * deltaTime);

    if (mVelocity.y > mMaxVelocityY) mVelocity.y = mMaxVelocityY;
    else if (mVelocity.y < -mMaxVelocityY) mVelocity.y = -mMaxVelocityY;

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    for (size_t i = 0; i < mProjectiles.size(); ++i)
        mProjectiles[i]->update(deltaTime, boss);

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
    
    if (IsKeyPressed(KEY_SPACE) || IsKeyReleased(KEY_SPACE)) resetVelocityY();
    
    // if (mTextureType == ATLAS && GetLength(mMovement) != 0 && mIsCollidingBottom)
    if (mTextureType == ATLAS && GetLength(mVelocity))
        animate(deltaTime);
}

std::map<Direction, std::vector<int>> projectileAnimationAtlas = { { RIGHT, { 0 } } };

void Player::shoot(Vector2 mousePos)
{
    if (!canShoot) return;
    canShoot = false;
    mCounter = 0.0f;

    mProjectiles.push_back(
        new Projectile{
            mPosition, { 50, 50 }, "assets/playerAttack.png", ATLAS, { 1, 1 },
            projectileAnimationAtlas, ATTACK, 500.0f, mAttackDamage
        }
    );
    
    mProjectiles[mProjectiles.size()-1]->shootPlayer(mousePos);
}

void Player::render()
{
    Entity::render();
    for (size_t i = 0; i < mProjectiles.size(); ++i) mProjectiles[i]->render();
}