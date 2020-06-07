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

using namespace std;
SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, leftWhiteBarMatrix, rightWhiteBarMatrix, ballMatrix;

float lastTicks = 0.0f;
const float leftWhiteBar_x = -4.9f;
const float rightWhiteBar_x = 4.9f;
float ball_x = 0.0f;
float ball_y = 0.0f;
float ball_speed = 3.0f;
float bar_speed = 3.0f;
float indicator_x = 1;
float indicator_y = 1;

glm::vec3 leftBar_position = glm::vec3(leftWhiteBar_x,0,0);
glm::vec3 leftBar_movement = glm::vec3(0,0,0);
glm::vec3 rightBar_position = glm::vec3(rightWhiteBar_x,0,0);
glm::vec3 rightBar_movement = glm::vec3(0,0,0);
glm::vec3 ball_position = glm::vec3(0,0,0);
glm::vec3 ball_movement = glm::vec3(0,0,0);

GLuint playerTextureID;
GLuint whiteBarTextureID;
GLuint ballTextureID;

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
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    whiteBarTextureID = LoadTexture("whitebar.png");
    ballTextureID = LoadTexture("ball.jpg");
}

void ProcessInput(){
    leftBar_movement = glm::vec3(0);
    rightBar_movement = glm::vec3(0);
    SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                case SDL_WINDOWEVENT_CLOSE:
                    gameIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    break;
            }
        }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_UP]){
        if(rightBar_position.y + 0.5 >= 3.5){
            rightBar_movement.y = 0.0f;
        }else{
            rightBar_movement.y = 1.0f;
        }
    }
    else if(keys[SDL_SCANCODE_DOWN]){
        if(rightBar_position.y - 0.5 <= -3.5){
            rightBar_movement.y =0.0f;
        }else{
            rightBar_movement.y = -1.0f;
        }
    }
    if(keys[SDL_SCANCODE_W]){
        if(leftBar_position.y + 0.5 >= 3.5){
            leftBar_movement.y = 0.0f;
        }else{
            leftBar_movement.y = 1.0f;
        }
    }
    else if(keys[SDL_SCANCODE_S]){
        if(leftBar_position.y - 0.5 <= -3.5){
            leftBar_movement.y =0.0f;
        }else{
            leftBar_movement.y = -1.0f;
        }
    }
}
 // boarder x:5, y:3.5
 // ball x:0.15(0.1), y:0.15(0.1)
 // bar radius x:0.05 y:0.5
void movement(glm::vec3& ball_position, float& indicator_x, float& indicator_y, const glm::vec3& rightBar_position, const glm::vec3& leftBar_position, float& ball_speed, float& bar_speed, float deltaTime){
    if(indicator_x == 1){
        if(ball_position.x + (ball_speed * deltaTime) < 4.75f){
            ball_position.x += ball_speed * deltaTime;
        }else if(ball_position.x + (ball_speed * deltaTime) >= 4.75f && (rightBar_position.y - 0.6) <= ball_position.y && ball_position.y <= (rightBar_position.y + 0.6)){
            indicator_x = 0;
        }else{
            ball_speed = 0;
        }
    }
    else if(indicator_x == 0){
        if(ball_position.x + (ball_speed * deltaTime) > -4.75f){
            ball_position.x -= ball_speed * deltaTime;
        }else if(ball_position.x + (ball_speed * deltaTime) <= -4.75f && (leftBar_position.y - 0.6) <= ball_position.y && ball_position.y <= (leftBar_position.y + 0.6)){
            indicator_x = 1;
        }else{
            ball_speed = 0;
        }
    }
    if(indicator_y == 1){
        if(ball_position.y + (ball_speed * deltaTime) < (3.8 - 0.1)){
                ball_position.y += ball_speed * deltaTime;
            }else{
    //            ball_position.x -= ball_speed * deltaTime;
                indicator_y = 0;
            }
        }
    else if(indicator_y == 0){
        if(ball_position.y - (ball_speed * deltaTime) >= -(3.8 - 0.1)){
            ball_position.y -= ball_speed * deltaTime;
        }else{
            indicator_y = 1;
        }
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    rightBar_position += rightBar_movement * bar_speed * deltaTime;
    leftBar_position += leftBar_movement * bar_speed * deltaTime;
    
    movement(ball_position, indicator_x, indicator_y, rightBar_position, leftBar_position, ball_speed, bar_speed, deltaTime);
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
    
    rightWhiteBarMatrix = glm::mat4(1.0f);
    rightWhiteBarMatrix = glm::translate(rightWhiteBarMatrix, rightBar_position);
    
    leftWhiteBarMatrix = glm::mat4(1.0f);
    leftWhiteBarMatrix = glm::translate(leftWhiteBarMatrix, leftBar_position);
}

//void renderTexture();

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    float vertices[] = { -0.05, -0.7, 0.05, -0.7, 0.05, 0.7, -0.05, -0.7, 0.05, 0.7, -0.05, 0.7 };
    float texCoords[] = { 0.0, 1.0 , 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(leftWhiteBarMatrix);
    glBindTexture(GL_TEXTURE_2D, whiteBarTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(rightWhiteBarMatrix);
    glBindTexture(GL_TEXTURE_2D, whiteBarTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    float vertices1[] = {-0.15,-0.15,0.15,-0.15,0.15,0.15,-0.15,-0.15,0.15,0.15,-0.15,0.15};
    float texCoords1[] = {0.0,1.0,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
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
