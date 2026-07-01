#include "Projectile.h"

Projectile::Projectile() : Entity(), mDamage { 0.0f }, hitmarker { LoadSound("assets/hitmarker.mp3") }
{
    SetSoundVolume(hitmarker, 0.33f);
}

Projectile::Projectile(Vector2 startPos, Vector2 scale, const char *textureFilepath,
    TextureType textureType, Vector2 spriteSheetDimensions, std::map<Direction,
    std::vector<int>> animationAtlas, EntityType entityType, float speed, float dmg) 
    : Entity(startPos, scale, textureFilepath, textureType, spriteSheetDimensions, animationAtlas, entityType),
    mDamage { dmg }, hitmarker { LoadSound("assets/hitmarker.mp3") } {}


Projectile::~Projectile() {}

void Projectile::hitTarget(Entity *target)
{
    target->takeDamage(mDamage);
    if (target->getCurrHealth() <= 0.0f) target->die();
    PlaySound(hitmarker);
    deactivate();
}

void Projectile::shootPlayer(Vector2 mousePos)
{
    activate();
    mMovement = {
        mousePos.x - ORIGIN.x,
        mousePos.y - ORIGIN.y
    };
    Normalise(&mMovement);
}

void Projectile::shootBoss1(Entity *player)
{
    activate();
    mMovement = {
        player->getPosition().x - mPosition.x,
        player->getPosition().y - mPosition.y
    };
    Normalise(&mMovement);
} 

void Projectile::shootBoss3(Entity *player, float variable)
{
    activate();
    mMovement = {
        10 * sin(variable * 3),
        10 * cos(variable * 3)
    };
    Normalise(&mMovement);
}

void Projectile::update(float deltaTime, Entity *target)
{
    if (mEntityStatus == INACTIVE) return;

    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;
    mVelocity.y = mMovement.y * mSpeed;

    mPosition.y += mVelocity.y * deltaTime;
    mPosition.x += mVelocity.x * deltaTime;

    if (isColliding(target)) hitTarget(target);

    if (mTextureType == ATLAS && GetLength(mVelocity) != 0)
        animate(deltaTime);
}