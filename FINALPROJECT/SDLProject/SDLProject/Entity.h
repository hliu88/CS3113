#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
#include <stdlib.h>

enum EntityType {PLAYER, PLATFORM, ENEMY, PLAYERBULLET, TURRENT, TURRENTBULLET, HP};

enum AIType {WALKER, WAITNGO};
enum AIState{ IDLE, WALKING, ATTACKING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    
    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animStill = NULL;
    int *animUp = NULL;
    int *animDown = NULL;
    int *animLeft = NULL;
    int *animRight = NULL;
    int location = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    float renderSize = 1.0f;
    float shootInterval = 0;
    float health = 100;
    float dps = 0;
    
    bool chargeShot = false;
    float chargeVal = 0;
    
    bool isActive = true;
    bool shoot = false;
    
    bool dead = false;
    bool success = false;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount, Entity* hp, int hpCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);
    void CheckCollisionEdge(float horizontalX);
    
    void turrentUpdate(Entity* player, Entity* turrentBullets);
    
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map, float horizontalX, Entity* turrentBullets, Entity* hp, int hpCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void AI(Entity* player);
    void AIWalker();
    void AIWaitAndGO(Entity* player);
};
