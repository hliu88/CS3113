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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vector>
#include <string>
#include "Entity.h"

#define PLATFORM_COUNT 30

struct GameState {
    Entity* platforms;
    Entity* ship;
};

GameState state;
GLuint fontTextureID;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool stopped = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath);
void createPlatform();
void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
              float size, float spacing, glm::vec3 position);

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    state.ship = new Entity();
    state.ship->position = glm::vec3(-2.5,3,0);
//    state.ship->position = glm::vec3(3,-3,0);
    state.ship->movement = glm::vec3(0);
    state.ship->acceleration = glm::vec3(0,-9.81f,0);
    state.ship->speed = 5.0f;
    state.ship->textureID = LoadTexture("lunarlander.png");
    state.ship->animRight = new int[1]{1};
    state.ship->animDown = new int[1]{0};
    state.ship->animLeft = new int[1]{1};
    state.ship->animUp = new int[1]{2};
    state.ship->animIndices = state.ship->animDown;
    state.ship->animFrames = 1;
    state.ship->animIndex = 0;
    state.ship->animTime = 0;
    state.ship->animCols = 3;
    state.ship->animRows = 1;
    state.ship->size = 0.5;
    state.ship->height = 0.8f;
    state.ship->width = 1.0f;
    state.ship->jumpPower = 1.0f;
    
    fontTextureID = LoadTexture("font1.png");
    
    createPlatform();
}

void ProcessInput() {
    state.ship->movement = glm::vec3(0);
    state.ship->animIndices = state.ship->animDown;
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
//                        state.ship->acceleration.x -= 0.01f;
//                        state.ship->animIndices = state.ship->animLeft;
                        break;
                    case SDLK_RIGHT:
//                        state.ship->acceleration.x += 0.01f;
//                        state.ship->animIndices = state.ship->animRight;
                        break;
                    case SDLK_SPACE:
//                        if(state.ship->collidedBottom){
//                            state.ship->jump = true;
//                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) {
//        state.ship->movement.x = -1.0f;
        state.ship->acceleration.x -= 0.01f;
        state.ship->animIndices = state.ship->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
//        state.ship->movement.x = 1.0f;
        state.ship->acceleration.x += 0.01f;
        state.ship->animIndices = state.ship->animRight;
    }
    else if(keys[SDL_SCANCODE_UP]){
        state.ship->movement.y = 1.0f;
//        state.ship->acceleration.y += 0.1f;
        state.ship->animIndices = state.ship->animUp;
    }
//    if (glm::length(state.ship->movement) > 1.0f) {
//        state.ship->movement = glm::normalize(state.ship->movement);
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
        state.ship->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    state.ship->Render(&program);
    if(state.ship->missionFailed == true){
        DrawText(&program, fontTextureID, "Mission Failed!", 0.5f, -0.25f, glm::vec3(-1.5, 0, 0));
    }else if(state.ship->missionSuccess == true){
        DrawText(&program, fontTextureID, "Mission Successful!", 0.5f, -0.25f, glm::vec3(-2, 0, 0));
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

void createPlatform(){
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("moonsurfacebox.png");
    GLuint landingTextureID = LoadTexture("moonlandingbox.png");
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(-1,-3.25f,0);
    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(0,-3.25f,0);
    state.platforms[2].textureID = platformTextureID;
    state.platforms[2].position = glm::vec3(1,-3.25f,0);
    state.platforms[3].textureID = landingTextureID;
    state.platforms[3].position = glm::vec3(2,-3.25f,0);
    state.platforms[3].landingPadLeft = true;
    state.platforms[4].textureID = landingTextureID;
    state.platforms[4].position = glm::vec3(3,-3.25f,0);
    state.platforms[4].landingPadRight = true;
    state.platforms[5].textureID = platformTextureID;
    state.platforms[5].position = glm::vec3(4,-3.25f,0);
    state.platforms[6].textureID = platformTextureID;
    state.platforms[6].position = glm::vec3(-2,-3.25f,0);
    state.platforms[7].textureID = platformTextureID;
    state.platforms[7].position = glm::vec3(-3,-3.25f,0);
    state.platforms[8].textureID = platformTextureID;
    state.platforms[8].position = glm::vec3(-4,-3.25f,0);
    state.platforms[9].textureID = platformTextureID;
    state.platforms[9].position = glm::vec3(-5,-2.25f,0);
    state.platforms[10].textureID = platformTextureID;
    state.platforms[10].position = glm::vec3(5,-3.25f,0);
    state.platforms[11].textureID = platformTextureID;
    state.platforms[11].position = glm::vec3(-5,-3.25f,0);
    state.platforms[12].textureID = platformTextureID;
    state.platforms[12].position = glm::vec3(-5,-1.25f,0);
    state.platforms[13].textureID = platformTextureID;
    state.platforms[13].position = glm::vec3(-5,-0.25f,0);
    state.platforms[14].textureID = platformTextureID;
    state.platforms[14].position = glm::vec3(-5,0.75f,0);
    state.platforms[15].textureID = platformTextureID;
    state.platforms[15].position = glm::vec3(-5,1.75f,0);
    state.platforms[16].textureID = platformTextureID;
    state.platforms[16].position = glm::vec3(-5,2.75f,0);
    state.platforms[17].textureID = platformTextureID;
    state.platforms[17].position = glm::vec3(-5,3.75f,0);
    state.platforms[18].textureID = platformTextureID;
    state.platforms[18].position = glm::vec3(5,-3.25f,0);
    state.platforms[19].textureID = platformTextureID;
    state.platforms[19].position = glm::vec3(5,-1.25f,0);
    state.platforms[20].textureID = platformTextureID;
    state.platforms[20].position = glm::vec3(5,-0.25f,0);
    state.platforms[21].textureID = platformTextureID;
    state.platforms[21].position = glm::vec3(5,0.75f,0);
    state.platforms[22].textureID = platformTextureID;
    state.platforms[22].position = glm::vec3(5,1.75f,0);
    state.platforms[23].textureID = platformTextureID;
    state.platforms[23].position = glm::vec3(5,2.75f,0);
    state.platforms[24].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(5,3.75f,0);
    state.platforms[25].textureID = platformTextureID;
    state.platforms[25].position = glm::vec3(5,-2.25f,0);
    state.platforms[26].textureID = platformTextureID;
    state.platforms[26].position = glm::vec3(1,0,0);
    state.platforms[27].textureID = platformTextureID;
    state.platforms[27].position = glm::vec3(2,0,0);
    state.platforms[28].textureID = platformTextureID;
    state.platforms[28].position = glm::vec3(3,0,0);
    state.platforms[29].textureID = platformTextureID;
    state.platforms[29].position = glm::vec3(4,0,0);
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, 0);
    }
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
