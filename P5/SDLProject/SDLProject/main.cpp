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

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[4];

Mix_Music *music;
Mix_Chunk *bounce;
Mix_Chunk *jump;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Jump To The Top!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
//    glViewport(0, 0, 640, 480);
    glViewport(0, 0, 1280, 960);
    
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
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);
    
    jump = Mix_LoadWAV("jump.wav");
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    
    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if(currentScene == sceneList[0]){
                            SwitchToScene(sceneList[1]);
                        }
                        break;
                    case SDLK_LEFT:
                        break;
                        
                    case SDLK_RIGHT:
                        break;
                        
                    case SDLK_SPACE:
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_SPACE]){
        currentScene->state.player->chargeJump = true;
        if(currentScene->state.player->lastOrientation == 1 && currentScene->state.player->collidedBottom){
            currentScene->state.player->animIndices = currentScene->state.player->animChargeRight;
        }else if(currentScene->state.player->collidedBottom){
            currentScene->state.player->animIndices = currentScene->state.player->animChargeLeft;
        }
        if(currentScene->state.player->jumpPower >= 7){
        }else{
            currentScene->state.player->jumpPower += 0.1f;
        }
    }else if(currentScene->state.player->chargeJump == true){
        if(currentScene->state.player->collidedBottom){
            Mix_PlayChannel(-1, jump, 0);
            currentScene->state.player->velocity.y += currentScene->state.player->jumpPower;
            currentScene->state.player->velocity.x = currentScene->state.player->lastOrientation * currentScene->state.player->velocity.y / 2;
            currentScene->state.player->inAir = true;
            if(currentScene->state.player->lastOrientation == 1){
                currentScene->state.player->animIndices = currentScene->state.player->animJumpRight;
            }else{
                currentScene->state.player->animIndices = currentScene->state.player->animJumpLeft;
            }
        }
        currentScene->state.player->chargeJump = false;
        currentScene->state.player->jumpPower = 0;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        if(currentScene->state.player->collidedBottom){
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
            currentScene->state.player->lastOrientation = -1;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        if(currentScene->state.player->collidedBottom){
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
            currentScene->state.player->lastOrientation = 1;
        }
    }
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
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    
    if(currentScene->state.player->position.y < -10){
        currentScene->state.player->respawn = true;
        currentScene->state.player->position = currentScene->state.player->startPt;
        currentScene->state.player->lives -= 1;
    }
    if(currentScene == sceneList[3] && currentScene->state.player->position.y >= -0.5 && currentScene->state.player->position.y <= 0 && currentScene->state.player->position.x <= 8.4999 && currentScene->state.player->position.x >= 6.5){
//        viewMatrix = glm::translate(viewMatrix, glm::vec3(0,3,0));
        currentScene->state.player->win = true;
        currentScene->state.player->canMove = 0;
    }
    if(currentScene->state.player->isAlive() && currentScene->state.player->position.y < -6){
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 6.5, 0));
    }
    else if (currentScene->state.player->isAlive() && currentScene->state.player->position.y >= -6) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
    }
    else if(currentScene->state.player->isAlive() && currentScene->state.player->position.y < -2) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    else if(currentScene->state.player->isAlive() && currentScene->state.player->position.y > 0.8f){
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5,0.8,0));
    }
    
    if(!currentScene->state.player->isAlive()){
        currentScene->state.player->loss = true;
        currentScene->state.player->canMove = 0;
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5,6.5,0));
        currentScene->state.player->isActive = false;
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
    if(currentScene->state.player->loss){
        GLuint fontTextureID = Util::LoadTexture("font1.png");
        Util::DrawText(&program, fontTextureID, "You Lost!", 0.5f, -0.25f, glm::vec3(4,-6.5,0));
    }
    if(currentScene->state.player->win){
        GLuint fontTextureID = Util::LoadTexture("font1.png");
        Util::DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f, glm::vec3(4,0,0));
    }
//    Util::heartRender(&program, currentScene->state.player->position.y);
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
        
        if(currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        
        Render();
    }
    
    Shutdown();
    return 0;
}
