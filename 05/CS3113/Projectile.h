#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Entity.h"

class Projectile : public Entity
{
private:
    float mDamage;
    Sound shoot;
    Sound hitmarker;

    void hitTarget(Entity *target);

public:
    Projectile();
    Projectile(Vector2 startPos, Vector2 scale, const char *textureFilepath,
        TextureType textureType, Vector2 spriteSheetDimensions, std::map<Direction,
        std::vector<int>> animationAtlas, EntityType entityType, float speed, float dmg);
    ~Projectile();

    void shootPlayer(Vector2 mousePos);
    void shootBoss1(Entity *player);
    void shootBoss3(Entity *player, float variable);
    
    void update(float deltaTime, Entity *target);
};

#endif // PROJECTILE_H