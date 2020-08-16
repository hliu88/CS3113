#include "Scene.h"
#include <vector>
#include <stdlib.h>
class Level1 : public Scene {

public:
    void Initialize() override;
    void Update(float deltaTime, float horizontalX) override;
    void Render(ShaderProgram *program) override;
};
