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

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[2];

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Final Project!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    sceneList[0] = new Level1();
//    sceneList[1] = new Level2();
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
                    case SDLK_DOWN:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
//                        if(currentScene->state.player->collidedBottom){
//                            currentScene->state.player->jump = true;
//                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }else{
        currentScene->state.player->animIndices = currentScene->state.player->animStill;
    }
    
    if(keys[SDL_SCANCODE_RIGHT]){
        currentScene->state.player->movement.x = 1.0f;
    }
    else if(keys[SDL_SCANCODE_LEFT]){
        currentScene->state.player->movement.x = -1.0f;
    }
    if(keys[SDL_SCANCODE_X]){
        if(currentScene->state.player->chargeShot == true &&
           currentScene->state.player->chargeVal >= 1){
            for(int i = 0; i < 100; i++){ //******** beware 100 shots capacity only
                if(currentScene->state.playerBullets[i].isActive == false){
                    currentScene->state.playerBullets[i].isActive = true;
                    currentScene->state.playerBullets[i].position = currentScene->state.player->position;
                    currentScene->state.playerBullets[i].position.x += 0.3;
                    currentScene->state.playerBullets[i].movement.x = 1;
                    currentScene->state.player->chargeShot = false;
                    currentScene->state.player->chargeVal = 0;
                    return;
                }
            }
        }
        else if(currentScene->state.player->chargeShot == false){
            currentScene->state.player->chargeShot = true;
            currentScene->state.player->chargeVal += 0.05;
        }
        else if(currentScene->state.player->chargeShot == true){
            currentScene->state.player->chargeVal += 0.05;
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
float horizontalX = -5.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
        return;
    }
    
    horizontalX -= 0.01f;
    horizontalX -= 0.05f;
    viewMatrix = glm::mat4(1.0f);
//    if (currentScene->state.player->position.x > 5) {
//    }else{
//    }
    viewMatrix = glm::translate(viewMatrix, glm::vec3(horizontalX, 3.75, 0));
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP, horizontalX);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
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
