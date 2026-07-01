#include "Player.h"

Player::Player(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
    Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType) :
    Entity(position, scale, textureFilepath, textureType, spriteSheetDimensions, animationAtlas, entityType), 
    mCurrHealth{ 100.0f }, mMaxHealth{ 100.0f }, mAttackDamage{ 10.0f }, mAttackCooldown{ 0.5f },
    mDragFactorX{ 0.5f }, mDragFactorY { 0.00003f }, playerAttack { LoadTexture("assets/playerAttack.png") }
{
    mProjectiles = new Projectile[25];
    std::map<Direction, std::vector<int>> projectileAnimationAtlas = { { RIGHT, { 0 } } };

    for (int i = 0; i < 25; ++i)
    {
        mProjectiles[i].setPosition(mPosition);
        mProjectiles[i].setScale({ 50, 50 });
        mProjectiles[i].setTexture(&playerAttack);
        mProjectiles[i].setEntityType(ATTACK);
        mProjectiles[i].setSpriteSheetDimensions({ 1, 1 });
        mProjectiles[i].setAnimationAtlas(projectileAnimationAtlas);
        mProjectiles[i].setSpeed(500.0f);
        mProjectiles[i].setDamage(mAttackDamage);
        mProjectiles[i].deactivate();

        mProjectiles[i].setColliderDimensions({
            mProjectiles[i].getScale().x / 1.5f,
            mProjectiles[i].getScale().y / 1.5f
        });
    }
}

Player::~Player() { delete [] mProjectiles; }

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

    for (int i = 0; i < 25; ++i)
        mProjectiles[i].update(deltaTime, boss);

    for (int i = 0; i < 25; ++i)
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
    
    if (IsKeyPressed(KEY_SPACE) || IsKeyReleased(KEY_SPACE)) resetVelocityY();
    
    if (mTextureType == ATLAS && GetLength(mVelocity))
        animate(deltaTime);
}

void Player::shoot(Vector2 mousePos)
{
    if (!canShoot) return;
    canShoot = false;
    mCounter = 0.0f;

    for (int i = 0; i < 25; ++i)
    {
        if (!mProjectiles[i].isActive())
        {
            mProjectiles[i].setPosition(mPosition);
            mProjectiles[i].shootPlayer(mousePos);
            break;
        }
    }
}

void Player::render()
{
    Entity::render();
    for (int i = 0; i < 25; ++i) mProjectiles[i].render();
}