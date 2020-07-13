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

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType {WALKER, WAITNGO, PATROL};
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
    
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    
    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;
    int *animJumpLeft = NULL;
    int *animJumpRight = NULL;
    int *animStill = NULL;
    int *animStillJump = NULL;
    int *animAtkLeft = NULL;
    int *animAtkRight = NULL;
    int lastOrientation = 1;
    int life = 3;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    float animSpeed = 0.075;
    float renderSize = 1.0f;
    
    bool isActive = true;
    bool alwaysAnim = false;
    bool inAir = false;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool penetration = false;
    
    Entity();
    
    bool CheckCollision(Entity* other);
    bool CheckCollisionSword(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckPenetration(Entity* objects, int objectCount);
    void CheckDamage(Entity* enemies, int enemyCount);
    void Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemyCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void CheckHitReg(Entity* enemies, int enemyCount);
    void AI(Entity* player, Entity* platforms, int platformCount);
    void AIWalker();
    void AIWaitAndGO(Entity* player);
    void AIPatrol(Entity* player, Entity* platforms, int platformCount);
};
