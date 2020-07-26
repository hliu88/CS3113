#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType {WALKER, WAITNGO};
enum AIState{IDLE, WALKING, ATTACKING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    glm::vec3 startPt;
    
    Mix_Chunk *bounce;
    Mix_Chunk *land;
    
    float width = 1;
    float height = 1;
    
    float jumpPower = 0;
    
    float speed;
    float canMove = 1;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;
    int *animJumpRight = NULL;
    int *animJumpLeft = NULL;
    int *animStill = NULL;
    int *animStillJump = NULL;
    int *animAtkRight = NULL;
    int *animAtkLeft = NULL;
    int *animChargeLeft = NULL;
    int *animChargeRight = NULL;
    int lastOrientation = -1;
    int lives = 3;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    float animSpeed = 0.075;
    float renderSize = 1.0f;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    bool inAir = false;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool collidedBottomE = false;
    bool collidedLeftE = false;
    bool collidedRightE = false;
    bool chargeJump = false;
    bool win = false;
    bool loss = false;
    bool respawn = false;
    bool alwaysAnim = false;
    
    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsYE(Map *map);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsXE(Map *map);
    void CheckCollisionsY(Map* map);
    bool isAlive();
    
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void AI(Entity* player, Map* map);
    void AIWalker(Map* map);
    void AIWaitAndGO(Entity* player, Map* map);
    void CheckDamage(Entity* enemies, int enemyCount);
    bool CheckCollision(Map* map);
};
