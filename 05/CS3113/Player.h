#ifndef PLAYER_H
#define PLAYER_H
#include "Projectile.h"

class Player : public Entity
{
private:
    Projectile *mProjectiles;
    Texture2D playerAttack;

    float mAttackDamage = 20.0f,
          mAttackCooldown,
          mCounter,
          mDragFactorX,
          mDragFactorY,
          mMaxVelocityX = 300.0f,
          mMaxVelocityY = 1250.0f,
          mCurrHealth,
          mMaxHealth;

    bool mIsFlying = false,
         canShoot = true;

public:
    Player(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas,
        EntityType                            entityType
    );
    ~Player();

    void render() override;

    void resetColliderFlags()
    {
        mIsFlying          = false;
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    bool isFlying() const { return mIsFlying; }

    void update(float deltaTime, Entity *player, Map *map,
                Entity *collidableEntities, int collisionCheckCount) override;

    void moveUp(float acceleration) override
    {
        mAcceleration.y = -acceleration * 1.1f;
        mDirection = UP;
        mIsFlying = true;
    }

    float getCurrHealth() const { return mCurrHealth; }
    float getMaxHealth() const { return mMaxHealth; }
    float getAttackDamage() const { return mAttackDamage; }
    float getAttackCooldown() const { return mAttackCooldown; }
    float getMaxVelocityX() const { return mMaxVelocityX; }
    float getMaxVelocityY() const { return mMaxVelocityY; }

    void setCurrHealth(int newHealth) { mCurrHealth = newHealth; }
    void setMaxHealth(int newHealth) { mMaxHealth = newHealth; }
    void setAttackDamage(float newDamage) { mAttackDamage = newDamage; }
    void setAttackCooldown(float newCooldown) { mAttackCooldown = newCooldown; }
    void setMaxVelocityX(float newVelocity) { mMaxVelocityX = newVelocity; }
    void setMaxVelocityY(float newVelocity) { mMaxVelocityY = newVelocity; }

    void shoot(Vector2 mousePos);
    void takeDamage(float damage) { mCurrHealth -= damage; }
};

#endif // PLAYER_H