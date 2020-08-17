
#include "DeathScreen.h"

void DeathScreen::Initialize() {
    state.nextScene = -1;
    state.player = new Entity();
}
void DeathScreen::Update(float deltaTime, float horizontalX) {
}

void DeathScreen::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 3.75, 0));
    Util::DrawText(program, fontTextureID, "You Died", 0.65f, -0.25f, glm::vec3(4,-3.5,0));
}
