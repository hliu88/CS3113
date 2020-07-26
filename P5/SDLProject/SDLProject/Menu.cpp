
#include "Menu.h"

void Menu::Initialize() {
    state.nextScene = -1;
    state.player = new Entity();
}
void Menu::Update(float deltaTime) {
}

void Menu::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Jump to The Top", 0.65f, -0.25f, glm::vec3(2.15,0.5,0));
    Util::DrawText(program, fontTextureID, "Press Return to Start", 0.4f, -0.25f, glm::vec3(3.3,-1,0));
}
