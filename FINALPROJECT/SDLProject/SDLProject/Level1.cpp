#include "Level1.h"
#define LEVEL1_WIDTH 132
#define LEVEL1_HEIGHT 8

#define ENEMY_COUNT 50
#define BULLET_COUNT 100

unsigned int level1_data[] =
{
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("PLANE.png");
    state.player->animUp = new int[1] {2};
    state.player->animDown = new int[1] {1};
    state.player->animStill = new int[1]{0};
    state.player->animIndices = state.player->animStill;
    state.player->animFrames = 1;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 3;
    state.player->animRows = 1;
    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.playerBullets = new Entity[50];
    GLuint bulletTextureID = Util::LoadTexture("blueLaser.png");
    for(int i = 0; i < 50; i++){
        state.playerBullets[i].isActive = false;
        state.playerBullets[i].entityType = PLAYERBULLET;
        state.playerBullets[i].textureID = bulletTextureID;
        state.playerBullets[i].speed = 5;
        state.playerBullets[i].height = 0.2f;
        state.playerBullets[i].width = 0.2f;
        state.playerBullets[i].renderSize = 0.2f;
        state.playerBullets[i].animStill = new int[1]{0};
        state.playerBullets[i].animIndices = state.playerBullets[i].animStill;
        state.playerBullets[i].animFrames = 1;
        state.playerBullets[i].animIndex = 0;
        state.playerBullets[i].animTime = 0;
        state.playerBullets[i].animCols = 1;
        state.playerBullets[i].animRows = 1;
        state.playerBullets[i].dps = 10;
    }
    
    state.turrentBullets = new Entity[50];
    GLuint turrentBulletTextureID = Util::LoadTexture("turrentBullet.png");
    for(int i = 0; i < 50; i++){
        state.turrentBullets[i].isActive = false;
        state.turrentBullets[i].entityType = TURRENTBULLET;
        state.turrentBullets[i].textureID = turrentBulletTextureID;
        state.turrentBullets[i].speed = 0.5;
        state.turrentBullets[i].height = 0.05f;
        state.turrentBullets[i].width = 0.05f;
        state.turrentBullets[i].renderSize = 0.2f;
        state.turrentBullets[i].animStill = new int[1]{0};
        state.turrentBullets[i].animIndices = state.turrentBullets[i].animStill;
        state.turrentBullets[i].animFrames = 1;
        state.turrentBullets[i].animIndex = 0;
        state.turrentBullets[i].animTime = 0;
        state.turrentBullets[i].animCols = 1;
        state.turrentBullets[i].animRows = 1;
        state.turrentBullets[i].dps = 10;
    }
    
    
    int turrentData[70] = {9, 10, 14, 15, 16, 19, 20, 23, 25, 26, 27, 28, 32, 33, 34, 35, 36, 40, 41, 43, 44, 45, 47, 49, 50, 53, 54, 55, 56, 57, 58, 59, 64, 66, 67, 69, 70, 72, 74, 75, 76, 77, 78, 80, 82, 83, 84, 85, 86, 89, 92, 93, 95, 97, 99, 100, 104, 105, 106, 107, 108, 110, 115, 116, 117, 118, 119, 120, 123, 125};
    std::vector<int> tData(turrentData, turrentData + sizeof(turrentData) / sizeof(int));
    state.turrentsDown = new Entity[70];
    state.turrentsUp = new Entity[70];
    GLuint turrentDown = Util::LoadTexture("turrentDown.png");
    GLuint turrentUp = Util::LoadTexture("turrentUp.png");
    for(int i = 0; i < 70; i++){
        state.turrentsUp[i].isActive = true;
        state.turrentsUp[i].entityType = TURRENT;
        state.turrentsUp[i].textureID = turrentUp;
        state.turrentsUp[i].height = 0.75;
        state.turrentsUp[i].width = 0.75;
        state.turrentsUp[i].renderSize = 0.75;
        state.turrentsUp[i].animLeft = new int[1]{0};
        state.turrentsUp[i].animStill = new int[1]{1};
        state.turrentsUp[i].animRight = new int[1]{2};
        state.turrentsUp[i].position = glm::vec3(tData[i], -7.25f, 0);
        state.turrentsUp[i].animIndices = state.turrentsUp[i].animLeft;
        state.turrentsUp[i].animFrames = 1;
        state.turrentsUp[i].animIndex = 0;
        state.turrentsUp[i].animTime = 0;
        state.turrentsUp[i].animCols = 3;
        state.turrentsUp[i].animRows = 1;
        state.turrentsUp[i].location = 1;
        
        state.turrentsDown[i].isActive = true;
        state.turrentsDown[i].entityType = TURRENT;
        state.turrentsDown[i].textureID = turrentDown;
        state.turrentsDown[i].height = 0.75;
        state.turrentsDown[i].width = 0.75;
        state.turrentsDown[i].renderSize = 0.75;
        state.turrentsDown[i].animLeft = new int[1]{0};
        state.turrentsDown[i].animStill = new int[1]{1};
        state.turrentsDown[i].animRight = new int[1]{2};
        state.turrentsDown[i].position = glm::vec3(tData[i] + 3, -0.25f, 0);
        state.turrentsDown[i].animIndices = state.turrentsDown[i].animLeft;
        state.turrentsDown[i].animFrames = 1;
        state.turrentsDown[i].animIndex = 0;
        state.turrentsDown[i].animTime = 0;
        state.turrentsDown[i].animCols = 3;
        state.turrentsDown[i].animRows = 1;
        state.turrentsDown[i].location = -1;
    }
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemy1TextureID = Util::LoadTexture("drone1.png");
    for(int i = 0; i < 50; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemy1TextureID;
        state.enemies[i].position = glm::vec3(rand() % 120 + 8, rand() % 7 - 7 , 0);
        state.enemies[i].speed = 0.75;
        state.enemies[i].aiType = WAITNGO;
        state.enemies[i].animIndices = new int[1]{0};;
        state.enemies[i].animFrames = 1;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 1;
        state.enemies[i].animRows = 1;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].isActive = true;
        state.enemies[i].width = 0.8f;
        state.enemies[i].height = 0.8f;
        state.enemies[i].renderSize = 0.8f;
        state.enemies[i].dps = 30;
        state.enemies[i].health = 10;
    }

    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

}
void Level1::Update(float deltaTime, float horizontalX) {
    state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);
    
    for(int i = 0; i < 50; i++){
        state.playerBullets[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);
        state.turrentBullets[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);
    }
    
    for(int i = 0; i < 50; i++){
        state.turrentsUp[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);
        state.turrentsDown[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);

    }
    
    for(int i = 0; i < 50; i++){
        state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map, horizontalX, state.turrentBullets);
    }
    
    if(state.player->position.x >= 50){
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    for(int i = 0; i < 50; i++){
        state.playerBullets[i].Render(program);
        state.turrentBullets[i].Render(program);
    }
    for(int i = 0; i < 50; i++){
        state.turrentsUp[i].Render(program);
        state.turrentsDown[i].Render(program);
    }
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    state.player->Render(program);

}

