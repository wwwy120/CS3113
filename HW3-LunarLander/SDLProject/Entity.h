class Entity
{
private:

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    // ————— TRANSFORMATIONS ————— //
    float     m_speed;
    glm::vec3 m_movement;
    glm::mat4 m_model_matrix;

    int m_width;
    int m_height;
    
    int m_frame_counter = 0;
    
    float m_left_or_right = 0;
public:
    // ————— STATIC VARIABLES ————— //
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT = 0,
        RIGHT = 1,
        UP    = 2,
        DOWN  = 3;
    
    // ————— PLATFORM SIZE ————— //
    glm::vec3 m_transform;

    // ————— ANIMATION ————— //
    
    float m_animation_time = 0.0f;

    GLuint    m_texture_id;
    
    bool m_win;
    bool m_game_stop = false;
    
    int m_fuel;

    // ————— METHODS ————— //
    Entity();

    bool const check_collision(Entity* other) const;

    void update(float delta_time, Entity* collidable_entities, int collidable_entity_count);
    void render(ShaderProgram* program);
    
    void move_left()  { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f; };
    void accelerate() { m_movement.x = 2.0f; };
    
    void game_status(Entity* other);

    // ————— GETTERS ————— //
    glm::vec3 const get_position()     const { return m_position; };
    glm::vec3 const get_velocity()     const { return m_velocity; };
    glm::vec3 const get_acceleration() const { return m_acceleration; };
    glm::vec3 const get_movement()     const { return m_movement; };
    float     const get_speed()        const { return m_speed; };

    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position) { m_position = new_position; };
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_position) { m_acceleration = new_position; };
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; };
    void const set_speed(float new_speed) { m_speed = new_speed; };
    void const set_transform(glm::vec3 new_transform) {m_transform = new_transform;};
    void const set_width(int new_width) {m_width = new_width;};
    void const set_height(int new_height) {m_height = new_height;};
    void const set_fuel(int new_fuel) {m_fuel = new_fuel;};
};
