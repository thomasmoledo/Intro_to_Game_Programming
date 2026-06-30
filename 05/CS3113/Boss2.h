#ifndef Boss2_H
#define Boss2_H

#include "Boss1.h"

class Boss2 : public Entity
{
private:
    std::vector<Entity*> mGridBars;

    float mAttackDamage,
          mAttackCooldown,
          mCounterAttack,
          mCounterCooldown,
          mCurrHealth,
          mMaxHealth;

    bool isAlive,
         canAttack,
         isMidAttack;

    float mNoDamageWindow = 3.0f;
    float mDamageWindow   = 1.0f;

    bool mDamageTextureApplied = false;

    void spawnGrid(Entity *player);
    void repositionGrid(Entity *player);
    void setGridTexture(bool isDamaging);
    void despawnGrid();
    bool checkGridCollision(Entity *player) const;

    Sound damageStart;

public:
    Boss2();
    Boss2(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    Boss2(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas,
        EntityType                            entityType
    );
    ~Boss2();

    void render() override;

    void update(
        float   deltaTime,
        Entity *player,
        Map    *map,
        Entity *collidableEntities,
        int     collisionCheckCount
    ) override;

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
};

#endif // Boss2_H