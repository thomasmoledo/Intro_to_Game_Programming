#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

class Projectile;

enum Direction    { LEFT, UP, RIGHT, DOWN                             }; // For walking
enum EntityStatus { ACTIVE, INACTIVE                                  };
enum EntityType   { PLAYER, BOSS, PLATFORM, ATTACK, VERTICAL, HORIZONTAL, BLOCK, NPC, EMPTY };
enum AIType       { WANDERER, FOLLOWER                                };
enum AIState      { WALKING, IDLE, FOLLOWING                          };

class Entity
{
protected:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int>                      mAnimationIndices;
    Direction                             mDirection;
    int                                   mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    bool  mIsJumping    = false;
    float mJumpingPower = 0.0f;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    AIType  mAIType;
    AIState mAIState;

    bool isColliding(Entity *other) const;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);

    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);

    virtual void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);
    void AIActivate(Entity *target);
    void AIWander();
    void AIFollow(Entity *target);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    Entity(
        Vector2 position,
        Vector2 scale,
        const char *textureFilepath,
        TextureType textureType,
        Vector2 spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas,
        EntityType entityType
    );
    virtual ~Entity();

    virtual void update(
        float   deltaTime,
        Entity *player,
        Map    *map,
        Entity *collidableEntities,
        int     collisionCheckCount
    );
    virtual void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping     = true;     }
    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }
    void displayCollider();

    bool isActive() { return mEntityStatus == ACTIVE; }

    virtual void moveUp()    { mMovement.y = -1; mDirection = UP;    }
    virtual void moveDown()  { mMovement.y =  1; mDirection = DOWN;  }
    virtual void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    virtual void moveRight() { mMovement.x =  1; mDirection = RIGHT; }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getJumpingPower()          const { return mJumpingPower;          }
    bool        isJumping()                const { return mIsJumping;             }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }
    EntityType  getEntityType()            const { return mEntityType;            }
    AIType      getAIType()                const { return mAIType;                }
    AIState     getAIState()               const { return mAIState;               }

    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    virtual bool isFlying()  const { return false; }

    std::map<Direction, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions)
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed = newSpeed;                       }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setJumpingPower(float newJumpingPower)
        { mJumpingPower = newJumpingPower;         }
    void setAngle(float newAngle)
        { mAngle = newAngle;                       }
    void setEntityType(EntityType entityType)
        { mEntityType = entityType;                }
    void setDirection(Direction newDirection)
    {
        mDirection = newDirection;

        if (mTextureType == ATLAS) mAnimationIndices = mAnimationAtlas.at(mDirection);
    }
    void setAIState(AIState newState)
        { mAIState = newState;                     }
    void setAIType(AIType newType)
        { mAIType = newType;                       }

    // For Player
    virtual void moveUp(float acceleration) { return; }
    virtual void resetVelocityY() { mVelocity.y /= 2.0f; }

    virtual float getCurrHealth() const { return 0; }
    virtual float getMaxHealth() const { return 0; }
    virtual float getAttackDamage() const { return 0.0f; }
    virtual float getAttackCooldown() const { return 0.0f; }
    virtual float getMaxVelocityX() const { return 0.0f; }
    virtual float getMaxVelocityY() const { return 0.0f; }

    virtual void setCurrHealth(int newHealth) { return; }
    virtual void setMaxHealth(int newHealth) { return; }
    virtual void setAttackDamage(float newDamage) { return; }
    virtual void setAttackCooldown(float newCooldown) { return; }
    virtual void setMaxVelocityX(float newVelocity) { return; }
    virtual void setMaxVelocityY(float newVelocity) { return; }

    virtual void setProjectileVector(std::vector<Projectile*> *vectorPtr) { return; }
    virtual void shoot(Vector2 mousePos) { return; }

    // For Boss
    virtual void takeDamage(float damage) { return; }
    virtual void die() { return; }
    virtual void shoot(Entity *player) { return; }
};

#endif // ENTITY_H
