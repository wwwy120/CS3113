#include "Scene.h"

class HelpPage : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 0;
    
    // ————— CONSTRUCTOR ————— //
    ~HelpPage();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
