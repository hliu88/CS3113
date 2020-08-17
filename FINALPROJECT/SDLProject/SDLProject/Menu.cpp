
#include "Menu.h"

void Menu::Initialize() {
    state.nextScene = -1;
    state.player = new Entity();
}
void Menu::Update(float deltaTime, float horizontalX) {
}

void Menu::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Space Fighter", 0.65f, -0.25f, glm::vec3(2.5,-3,0));
    Util::DrawText(program, fontTextureID, "Press Return to Start", 0.4f, -0.25f, glm::vec3(3.4,-4.5,0));
}
