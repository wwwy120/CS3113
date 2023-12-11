#include "Scene.h"

class StartPage : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 0;
    int STAR_COUNT = 10000;
    
    // ————— CONSTRUCTOR ————— //
    ~StartPage();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
