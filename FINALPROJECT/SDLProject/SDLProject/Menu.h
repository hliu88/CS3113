#include "Scene.h"
class Menu : public Scene {

public:
    void Initialize() override;
    void Update(float deltaTime, float horizontalX) override;
    void Render(ShaderProgram *program) override;
};
