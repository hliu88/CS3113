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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, DVDMatrix, spaceinvaderMatrix, projectionMatrix;

float DVD_x = rand() % 10 - 5;
float DVD_y = rand() % 7 - 3.5;
float DVD_vertical = 1;
float DVD_horizontal = 1;
float spaceinvader_rotate = 0.0f;
float spaceinvader_x = 0.0f;
float spaceinvader_y = 1.0f;
float spaceinvader_scale = 2.0f;

GLuint DVDTextureID;
GLuint spaceinvaderTextureID;

GLuint LoadTexture(const char* filePath){
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if(image ==NULL){
        std::cout << "Unable to load image. Make sure the path is correct" << std::endl;
        assert(false);
    }

    GLuint textureID; //Create texture
    glGenTextures(1, &textureID); //Create texture
    glBindTexture(GL_TEXTURE_2D, textureID); //Bind texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    //Sending pixel data
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Texture Filtering

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 1!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    DVDMatrix = glm::mat4(1.0f);
    spaceinvaderMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    DVDTextureID = LoadTexture("dvdlogo.png");
    spaceinvaderTextureID = LoadTexture("spaceinvader.jpg");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void movement(float& x, float& indicator, float deltaTime, float boarder, float edge){
    if (indicator == 1){
        if (x + (1.0f * deltaTime) < (boarder-edge)){
            x += 1.0f * deltaTime;
        }else{
            x -= 1.0f * deltaTime;
            indicator = 0;
        }
    }
    if (indicator == 0){
        if (x - (1.0f * deltaTime) >= -(boarder-edge)){
            x -= 1.0f * deltaTime;
        }else{
            x += 1.0f * deltaTime;
            indicator = 1;
        }
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    

    movement(DVD_x, DVD_horizontal, deltaTime, 5, 0.5);
    movement(DVD_y, DVD_vertical, deltaTime, 3.5, 0.0);
    
    spaceinvader_rotate += 90.0f * deltaTime;
    spaceinvader_scale -= 0.3f * deltaTime;
    spaceinvader_x += 0.1f * deltaTime;
    
    DVDMatrix = glm::mat4(1.0f);
    DVDMatrix = glm::translate(DVDMatrix, glm::vec3(DVD_x, DVD_y, 0.0f));
    
    spaceinvaderMatrix = glm::mat4(1.0f);
    spaceinvaderMatrix = glm::rotate(spaceinvaderMatrix, glm::radians(spaceinvader_rotate), glm::vec3(0.0f,0.0f,1.0f));
    spaceinvaderMatrix = glm::scale(spaceinvaderMatrix, glm::vec3(spaceinvader_scale, spaceinvader_scale, 1.0f));
    spaceinvaderMatrix = glm::translate(spaceinvaderMatrix, glm::vec3(spaceinvader_x, spaceinvader_y, 0.0f));
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = {-0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5,-0.5,0.5,0.5,-0.5,0.5};
    float texCoords[] = {0.0,1.0,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(DVDMatrix);
    glBindTexture(GL_TEXTURE_2D, DVDTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program.SetModelMatrix(spaceinvaderMatrix);
    glBindTexture(GL_TEXTURE_2D, spaceinvaderTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
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
