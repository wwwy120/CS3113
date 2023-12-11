#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

Entity::Entity()
{
    // ––––– PHYSICS ––––– //
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
//    m_acceleration = glm::vec3(0.0f);

    // ––––– TRANSLATION ––––– //
    m_movement = glm::vec3(0.0f);
    m_speed = 0;
    m_model_matrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
    delete[] m_animation_left;
    delete[] m_animation_right;
    delete[] m_animation_up;
    delete[] m_animation_down;
    delete[] m_walking;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::ai_activate(Entity* player)
{
    switch (m_ai_type)
    {
    case GUARD:
        ai_guard(player);
        break;

    default:
        break;
    }
}

void Entity::ai_guard(Entity* player)
{
    switch (m_ai_state) {
    case IDLE:
        if (glm::distance(m_position, player->get_position()) < 5.0f) m_ai_state = WALKING;
        break;

    case WALKING:
        m_movement = glm::vec3(0.0f,0.0f,0.0f);
        if (m_position.x > player->get_position().x) {
            m_movement.x = -1.0f;
        }
        else if(m_position.x < player->get_position().x){
            m_movement.x = 1.0f;
        }
        if(m_position.y > player->get_position().y) {
            m_movement.y = -1.0f;
        }
        else if (m_position.y < player->get_position().y){
            m_movement.y = 1.0f;
        }
            
        if(m_movement.x == -1.0f){
            if(m_movement.y == 1.0f){
                if(fabs(m_position.x - player->get_position().x) > fabs(m_position.y - player->get_position().y)){
                    m_animation_indices = m_walking[LEFT];
                }
                else{
                    m_animation_indices = m_walking[UP];
                }
            }
            else if(m_movement.y == -1.0f){
                if(fabs(m_position.x - player->get_position().x) > fabs(m_position.y - player->get_position().y)){
                    m_animation_indices = m_walking[LEFT];
                }
                else{
                    m_animation_indices = m_walking[DOWN];
                }
            }
            else{
                m_animation_indices = m_walking[LEFT];
            }
        }
        else if(m_movement.x == 1.0f){
            if(m_movement.y == 1.0f){
                if(fabs(m_position.x - player->get_position().x) > fabs(m_position.y - player->get_position().y)){
                    m_animation_indices = m_walking[RIGHT];
                }
                else{
                    m_animation_indices = m_walking[UP];
                }
            }
            else if(m_movement.y == -1.0f){
                if(fabs(m_position.x - player->get_position().x) > fabs(m_position.y - player->get_position().y)){
                    m_animation_indices = m_walking[RIGHT];
                }
                else{
                    m_animation_indices = m_walking[DOWN];
                }
            }
            else{
                m_animation_indices = m_walking[RIGHT];
            }
        }
        break;

    case ATTACKING:
        break;

    default:
        break;
    }
}


void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map)
{
    if (!m_is_active) return;
    
    if(m_entity_type == BOMB && m_is_active){
        if(bomb_timer != 0){
            bomb_timer -= 1;
            m_explode = false;
        }
        else{
            m_is_active = false;
            bomb_timer = 300;
            disappear();
            m_explode = true;
        }
    }
    
    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;


    if (m_entity_type == ENEMY) ai_activate(player);
    
    if (m_animation_indices != NULL)
    {
        if (glm::length(m_movement) != 0)
        {
            m_animation_time += delta_time;
            float frames_per_second = (float)1 / SECONDS_PER_FRAME;

            if (m_animation_time >= frames_per_second)
            {
                m_animation_time = 0.0f;
                m_animation_index++;

                if (m_animation_index >= m_animation_frames)
                {
                    m_animation_index = 0;
                }
            }
        }
    }
    
    if(m_entity_type == PLAYER || m_entity_type == ENEMY){
        check_collision_x(map);
        check_collision_y(map);
        if(m_collided_top && m_movement.y > 0.0f){
            m_movement.y = 0.0f;
        }
        else if(m_collided_bottom && m_movement.y < 0.0f){
            m_movement.y = 0.0f;
        }
        if(m_collided_left && m_movement.x < 0.0f){
            m_movement.x = 0.0f;
        }
        else if(m_collided_right && m_movement.x > 0.0f){
            m_movement.x = 0.0f;
        }
    }
    m_velocity = m_movement * m_speed;
    m_position.y += m_velocity.y * delta_time;
    m_position.x += m_velocity.x * delta_time;
    
    for(int i = 0; i < object_count; ++i){
        if(m_entity_type == PLAYER && check_collision(&objects[i])){
            alive = false;
            m_position.x = 0;
            m_position.y = 0;
        }
    }

    if(m_entity_type == ENEMY && !alive){
        m_is_active = false;
        m_position.x = -1000;
        m_position.y = -1000;
    }
    
    if(m_entity_type == STAR && m_appear){
        if(check_collision(player)){
            player->star_get += 1;
            m_position.x = -1000;
            m_position.y = -1000;
        }
    }
    
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    
    if(m_entity_type == MUD){
        m_model_matrix = glm::scale(m_model_matrix, glm::vec3(8.0f, 8.0f, 1.0f));
    }
    

}


void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(top, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }
    if (map->is_solid(top_left, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }
    if (map->is_solid(top_right, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;
    }
    if (map->is_solid(bottom_left, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;
    }
    if (map->is_solid(bottom_right, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;

    }
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(left, &penetration_x, &penetration_y))
    {
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y))
    {
        m_collided_right = true;
    }
}



void Entity::render(ShaderProgram* program)
{
    program->set_model_matrix(m_model_matrix);

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}


bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    // If either entity is inactive, there shouldn't be any collision
    if (!m_is_active || !other->m_is_active) return false;

    float x_distance = fabs(m_position.x - other->m_position.x) - 0.905f;
    float y_distance = fabs(m_position.y - other->m_position.y) - 0.905f;

    return x_distance < 0.0f && y_distance < 0.0f;
}

bool Entity::check_collision(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];
        
        float x_distance = fabs(m_position.x - collidable_entity->m_position.x) - (0.905f);
        float y_distance = fabs(m_position.y - collidable_entity->m_position.y) - (0.905f);

        if(collidable_entity->m_disappear == false){
            if (check_collision(collidable_entity))
            {
                if (x_distance >= y_distance) {
                    return true;
                }
                else if (y_distance <= 0 && m_position.y < collidable_entity->m_position.y){
                    return true;
                }
                else{
                    collidable_entity->disappear();
                    return false;
                }
            }
        }
    }
    return false;
}
void Entity::disappear(){
    m_position.x = -1000;
    m_position.y = -1000;
    m_width = 0.0f;
    m_height = 0.0f;
    m_disappear = true;
}


