#include "Level1.h"
#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 11

#define LEVEL1_ENEMY_COUNT 1

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3,
    3, 1, 1, 0, 0, 1, 0, 0, 2, 2, 3,
    3, 2, 2, 0, 1, 2, 1, 1, 2, 2, 3,
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, -8, 0);
    state.player->startPt = glm::vec3(2, -8,0);
//    state.player->position = glm::vec3(2.82, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("charactersprite.png");
    
    state.player->animRight = new int[8] {16,16,16,16,16,16,16,16};
    state.player->animLeft = new int[8] {120,120,120,120,120,120,120,120};
    state.player->animJumpRight = new int[8] {65, 66, 67, 68, 68, 69, 70, 65};
    state.player->animJumpLeft = new int[8] {169,170,171,172,172,173,174,169};
    state.player->animStill = new int[8]{104,106,107,109,111,112,113,114};
    state.player->animStillJump = new int[8]{78,78,79,79,80,80,81,81};
    state.player->animAtkRight = new int[8]{39,40,41,42,43,44,45,46};
    state.player->animAtkLeft = new int[8]{143,144,145,146,147,148,149,150};
    state.player->animChargeLeft = new int[8]{198,198,198,198,198,198,198,198};
    state.player->animChargeRight = new int[8]{95,95,95,95,95,95,95,95};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 8;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 13;
    state.player->animRows = 16;
    
    state.player->height = 1.0f;
    state.player->width = 0.5f;
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = Util::LoadTexture("koopa.png");
    state.enemies[0].animLeft = new int[4]{0,1,2,3};
    state.enemies[0].animRight = new int[4]{4,5,6,7};
    state.enemies[0].animIndices = state.enemies[1].animRight;
    state.enemies[0].animFrames = 4;
    state.enemies[0].animRows = 1;
    state.enemies[0].animCols = 8;
    state.enemies[0].height = 1;
    state.enemies[0].width = 0.5;
    state.enemies[0].animSpeed = 0.175f;
//    state.enemies[0].position = glm::vec3(6,-9.5,0);
    state.enemies[0].position = glm::vec3(6, -10,0);
    state.enemies[0].acceleration = glm::vec3(0,-9.8f,0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].alwaysAnim = true;
    
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }
    state.player->CheckDamage(state.enemies, LEVEL1_ENEMY_COUNT);
    if(state.player->position.y >= 0 && state.player->position.x > 5.5 && state.player->position.x < 6.5){
        state.nextScene = 2;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
}

