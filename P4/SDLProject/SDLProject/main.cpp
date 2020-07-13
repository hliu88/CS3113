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
#include <vector>
#include <string>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 17
#define ENEMY_COUNT 3
struct GameState {
    Entity *player;
    Entity* platforms;
    Entity* enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

GLuint fontTextureID, heartTextureID;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
              float size, float spacing, glm::vec3 position);
void heartRender();
bool enemyDead();

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 4!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
//    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    fontTextureID = LoadTexture("font1.png");
    heartTextureID = LoadTexture("heart.png");
   
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4.5, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("charactersprite.png");
    
    state.player->animRight = new int[8] {16,17,18,19,20,13,14,15};
    state.player->animLeft = new int[8] {120,121,122,123,124,117,118,119};
    state.player->animJumpRight = new int[8] {65, 66, 67, 68, 68, 69, 70, 65};
    state.player->animJumpLeft = new int[8] {169,170,171,172,172,173,174,169};
    state.player->animStill = new int[8]{104,106,107,109,111,112,113,114};
    state.player->animStillJump = new int[8]{78,78,79,79,80,80,81,81};
    state.player->animAtkRight = new int[8]{39,40,41,42,43,44,45,46};
    state.player->animAtkLeft = new int[8]{143,144,145,146,147,148,149,150};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 8;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 13;
    state.player->animRows = 16;

    state.player->height = 1.0f;
    state.player->width = 0.4f;
    state.player->jumpPower = 8.0f;
    
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint platformTextureID = LoadTexture("platformPack_tile001.png");
    
    for(int i = 0; i < 12; i++){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-5 + i,-3.25f,0);
    }
    
    for(int i = 0; i < 4; i++){
        state.platforms[i + 12].entityType = PLATFORM;
        state.platforms[i + 12].textureID = platformTextureID;
        state.platforms[i + 12].position = glm::vec3(1.5 + i, -1.0f, 0);
    }
    
    state.platforms[16].entityType = PLATFORM;
    state.platforms[16].textureID = platformTextureID;
    state.platforms[16].position = glm::vec3(-4.5f,-2.25f,0);
    
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }
    
    state.enemies = new Entity[ENEMY_COUNT];
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = LoadTexture("bat.png");
    state.enemies[0].animLeft = new int[5]{0,1,2,3,4};
    state.enemies[0].animRight = new int[5]{15,16,17,18,19};
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].position = glm::vec3(3, -2.25, 0);
    state.enemies[0].speed = 0.5;
    state.enemies[0].animFrames = 5;
    state.enemies[0].aiType = WAITNGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 1.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].animRows = 6;
    state.enemies[0].animCols = 5;
    state.enemies[0].alwaysAnim = true;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = LoadTexture("koopa.png");
    state.enemies[1].animLeft = new int[4]{0,1,2,3};
    state.enemies[1].animRight = new int[4]{4,5,6,7};
    state.enemies[1].animIndices = state.enemies[1].animRight;
    state.enemies[1].animFrames = 4;
    state.enemies[1].animRows = 1;
    state.enemies[1].animCols = 8;
    state.enemies[1].height = 1;
    state.enemies[1].width = 0.5;
    state.enemies[1].animSpeed = 0.175f;
    state.enemies[1].position = glm::vec3(3,0,0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = PATROL;
    state.enemies[1].aiState = WALKING;
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = LoadTexture("slime.png");
    state.enemies[2].animIndices = new int[4]{0,1,2,3};
    state.enemies[2].animFrames = 4;
    state.enemies[2].animRows = 1;
    state.enemies[2].animCols = 4;
    state.enemies[2].animTime = 0.5;
    state.enemies[2].alwaysAnim = true;
    state.enemies[2].animSpeed = 0.15f;
    state.enemies[2].position = glm::vec3(2,0,0);
    state.enemies[2].acceleration = glm::vec3(0,-9.8f,0);
    state.enemies[2].renderSize = 0.5f;
    state.enemies[2].height = 0.5;
    state.enemies[2].width = 0.5;
    state.enemies[2].speed = 0.5f;
    state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = WALKING;
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
//                        if(state.player->collidedBottom){
//                            state.player->jump = true;
//                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(state.player->collidedBottom){
        state.player->inAir = false;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        if(!state.player->inAir){
            state.player->animIndices = state.player->animLeft;
        }
        state.player->movement.x = -1.0f;
        state.player->lastOrientation = -1;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        if(!state.player->inAir){
            state.player->animIndices = state.player->animRight;
        }
        state.player->lastOrientation = 1;
    }
    if(keys[SDL_SCANCODE_SPACE] && keys[SDL_SCANCODE_RIGHT]){
        if(state.player->collidedBottom){
            state.player->jump = true;
            state.player->inAir = true;
        }
        state.player->animIndices= state.player->animJumpRight;
    }
    else if(keys[SDL_SCANCODE_SPACE] && keys[SDL_SCANCODE_LEFT]){
        if(state.player->collidedBottom){
            state.player->jump = true;
            state.player->inAir = true;
        }
        state.player->animIndices= state.player->animJumpLeft;
    }
    else if(keys[SDL_SCANCODE_SPACE]){
        if(state.player->collidedBottom){
            state.player->jump = true;
            state.player->inAir = true;
        }
        state.player->movement.x = 0.00000001f;
        state.player->animIndices= state.player->animStillJump;
    }
    if(keys[SDL_SCANCODE_X] && keys[SDL_SCANCODE_RIGHT]){
        state.player->animIndices = state.player->animAtkRight;
        state.player->movement.x = 1.0f;
        state.player->CheckHitReg(state.enemies, ENEMY_COUNT);
    }
    else if(keys[SDL_SCANCODE_X] && keys[SDL_SCANCODE_LEFT]){
        state.player->animIndices = state.player->animAtkLeft;
        state.player->movement.x = -1.0f;
        state.player->CheckHitReg(state.enemies, ENEMY_COUNT);
    }
    else if(keys[SDL_SCANCODE_X]){
        if(state.player->lastOrientation == -1){
            state.player->animIndices = state.player->animAtkLeft;
        }
        else{
            state.player->animIndices = state.player->animAtkRight;
        }
        state.player->movement.x = 0.00000001f;
        state.player->CheckHitReg(state.enemies, ENEMY_COUNT);
    }
    

//    if (glm::length(state.player->movement) > 1.0f) {
//        state.player->movement = glm::normalize(state.player->movement);
//    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        
        for(int i = 0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
    }
    
    state.player->Render(&program);
    string outputText = "x" + to_string(state.player->life);
    DrawText(&program, fontTextureID, outputText, 0.5f, -0.25f, glm::vec3(4, 3, 0));
    heartRender();
    if(state.player->life == 0){
        DrawText(&program, fontTextureID, "GAME OVER! YOU LOSE!", 0.5f, -0.25f, glm::vec3(-2.5,0,0));
    }else{
        if(enemyDead()){
            DrawText(&program, fontTextureID, "YOU WIN!", 0.5f, -0.25f, glm::vec3(-1,0,0));
        }
    }
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}


void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
                             float size, float spacing, glm::vec3 position){
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    } // end of for loop
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void heartRender(){
    program.SetModelMatrix(modelMatrix);
    
    float vertices1[] = {-0.3, -0.3, 0.3, -0.3, 0.3, 0.3, -0.3, -0.3, 0.3, 0.3, -0.3, 0.3 };
    float texCoords1[] = {0.0,1.0,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, heartTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(3.6,3,0));
}

bool enemyDead(){
    bool bk = true;
    for(int i = 0; i < ENEMY_COUNT; i++){
        if(state.enemies[i].isActive == true){
            bk = false;
        }
    }
    return bk;
}
