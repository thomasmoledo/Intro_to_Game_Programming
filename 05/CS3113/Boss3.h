#ifndef Boss3_H
#define Boss3_H

#include "Boss2.h"

class Boss3 : public Entity
{
private:
    std::vector<Projectile*> mProjectiles;

    float mAttackDamage,
          mAttackCooldown,
          mCircleCounter,
          mCounter,
          mCurrHealth,
          mMaxHealth;

    bool isAlive,
         canShoot;

public:
    Boss3();
    Boss3(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    Boss3(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas,
        EntityType                            entityType
    );
    ~Boss3();

    void update(
        float   deltaTime,
        Entity *player,
        Map    *map,
        Entity *collidableEntities,
        int     collisionCheckCount
    ) override;

    void render() override;

    float getCurrHealth() const { return mCurrHealth; }
    float getMaxHealth() const { return mMaxHealth; }
    float getAttackDamage() const { return mAttackDamage; }
    float getAttackCooldown() const { return mAttackCooldown; }

    void setCurrHealth(int newHealth) { mCurrHealth = newHealth; }
    void setMaxHealth(int newHealth) { mMaxHealth = newHealth; }
    void setAttackDamage(float newDamage) { mAttackDamage = newDamage; }
    void setAttackCooldown(float newCooldown) { mAttackCooldown = newCooldown; }

    void takeDamage(float damage) { mCurrHealth -= damage; }
    void die() { isAlive = false; }
    void shoot(Entity *player);
};

#endif // Boss3_H