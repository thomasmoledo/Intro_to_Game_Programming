#ifndef Boss1_H
#define Boss1_H

#include "Player.h"

class Boss1 : public Entity
{
private:
    Projectile *mProjectiles;
    Texture2D bossAttack;

    float mAttackDamage,
          mAttackCooldown,
          mCounter,
          mCurrHealth,
          mMaxHealth;

    bool isAlive,
         canShoot;

public:
    Boss1(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    ~Boss1();

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

#endif // Boss1_H