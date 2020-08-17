
#include "EndScreen.h"

void EndScreen::Initialize() {
    state.nextScene = -1;
    state.player = new Entity();
}
void EndScreen::Update(float deltaTime, float horizontalX) {
}

void EndScreen::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Mission Success", 0.65f, -0.25f, glm::vec3(2.5,-3.5,0));
}
