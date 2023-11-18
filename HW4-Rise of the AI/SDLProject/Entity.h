#include "Map.hpp"

enum AIType     { WALKER, JUMPER, HEARTBEAT};
enum GameStatus  { WIN, LOSE, CONTINUE};

class Entity
{
private:
    int *m_animation_right = NULL, // move to the right
    *m_animation_left  = NULL; // move to the left
    
    // ————— TRANSFORMATIONS ————— //
    glm::vec3 m_movement;
    glm::vec3 m_position;
    glm::mat4 m_model_matrix;
    glm::vec3 m_velocity;
    glm::vec3 m_transform = glm::vec3(0.0f);
    
    // ————— TEXTURES ————— //
    GLuint    m_texture_id;
    
    bool m_is_active = true;
    
    AIType     m_ai_type;
    
    // ––––– PHYSICS (COLLISIONS) ––––– //
        bool m_collided_top = false;
        bool m_collided_bottom = false;
        bool m_collided_left = false;
        bool m_collided_right = false;
    
    int current_frame = 0;
    
    bool m_disappear = false;
    
    float g_angle = 0.0f;
    float g_x_coords = 2.0f; //current x-coord
    float g_y_coords = 0.0f; //current y-coord
    
public:
    GameStatus status = CONTINUE;
    // ————— STATIC VARIABLES ————— //
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT  = 0,
    RIGHT = 1;
    int CURRENT_DIRECTION = 0;
    
    float m_height;
    float m_width;
    
    bool jump_start;
    bool jump_stop;
    int jump_height = 0;
    bool is_enemy;
    
    bool game_stop = false;

    // ————— ANIMATION ————— //
    int **m_walking = new int*[2]
    {
        m_animation_left,
        m_animation_right,
    };
    
    int m_animation_frames = 0,
        m_animation_index  = 0,
        m_animation_cols   = 0,
        m_animation_rows   = 0;
    
    int  *m_animation_indices = NULL;
    float m_animation_time    = 0.0f;

    // ————— METHODS ————— //
    Entity();
    ~Entity();
    
    void const check_collision(Entity* collidable_entities, int collidable_entity_count);
//    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Map* map);
    void const check_collision_y(Map* map);
    bool const check_collision(Entity* other) const;
    void disappear();
    void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index);
    void update(float delta_time, Entity* player, Entity* collidable_entities, int object_count, Map* map);
    void render(ShaderProgram *program);
    
    void move_left()  { m_movement.x = -1.0f;};
    void move_right() { m_movement.x = 1.0f; };
    void move_jump()  {
        if(jump_stop == true){
            jump_stop = false;
            jump_start = true;
            m_movement.y = 1.0f;
        }
    };
    
    // ————— GETTERS ————— //
    glm::vec3 const get_position()   const { return m_position;   };
    glm::vec3 const get_movement()   const { return m_movement;   };
    GLuint    const get_texture_id() const { return m_texture_id; };
    glm::vec3 const get_velocity()      const { return m_velocity;      };
    AIType     const get_ai_type()        const { return m_ai_type;         };
    
    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position)  { m_position   = new_position;     };
    void const set_movement(glm::vec3 new_movement)  { m_movement   = new_movement;     };
    void const set_texture_id(GLuint new_texture_id) { m_texture_id = new_texture_id;   };
    void const set_velocity(glm::vec3 new_velocity)  { m_velocity   = new_velocity;     };
    void const set_transform(glm::vec3 new_transform) {m_transform = new_transform;};
    void const set_width(int new_width) {m_width = new_width;};
    void const set_height(int new_height) {m_height = new_height;};
    void const set_ai_type(AIType new_ai_type) { m_ai_type = new_ai_type;              };
};
