#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Map.hpp"

Entity::Entity()
{
    m_position     = glm::vec3(0.0f);
    m_velocity        = glm::vec3(0.0f);
    m_model_matrix = glm::mat4(1.0f);
    jump_stop      = true;
    jump_start     = false;
    m_transform = glm::vec3(1.0f);
    
}

Entity::~Entity()
{
    delete [] m_animation_left;
    delete [] m_animation_right;
    delete [] m_walking;
}


void Entity::draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float) (index % m_animation_cols) / (float) m_animation_cols;
    float v_coord = (float) (index / m_animation_cols) / (float) m_animation_rows;
    
    // Step 2: Calculate its UV size
    float width = 1.0f / (float) m_animation_cols;
    float height = 1.0f / (float) m_animation_rows;
    
    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };
    
    float vertices[] =
    {
        -2.0, -2.0, 2.0, -2.0,  2.0, 2.0,
        -2.0, -2.0, 2.0,  2.0, -2.0, 2.0
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

void const Entity::check_collision(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if(collidable_entity->m_disappear == false){
            if (check_collision(collidable_entity))
            {
                float x_distance = fabs(m_position.x - collidable_entity->m_position.x) - ((m_width + collidable_entity->m_width) / 2.0f);
                float y_distance = fabs(m_position.y - collidable_entity->m_position.y) - ((m_height + collidable_entity->m_height) / 2.0f);
                if (x_distance >= y_distance) {
                    status = LOSE;
                    return;
                }
                else if (y_distance <= 0 && m_position.y < collidable_entity->m_position.y){
                    status = LOSE;
                    return;
                }
                else{
                    collidable_entity->disappear();
                    status = CONTINUE;
                    return;
                }
            }
        }
    }
}
void Entity::disappear(){
    m_transform = glm::vec3(0.0f, 0.0f, 0.0f);
    m_position.x = -100;
    m_width = 0.0f;
    m_height = 0.0f;
    m_disappear = true;
}

void const Entity::check_collision_y(Map* map)
{
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + ((m_height - 8) / 2), 1.0f);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + ((m_height - 8) / 2), 1.0f);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + ((m_height - 8) / 2), 1.0f);
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - ((m_height - 8) / 2), 1.0f);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - ((m_height - 8) / 2), 1.0f);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - ((m_height - 8) / 2), 1.0f);

    float penetration_x = 0;
    float penetration_y = 0;

    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y))
    {
        m_collided_top = true;
    }
    else{
        m_collided_top = false;
    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y))
    {
        m_collided_bottom = true;
    }
    else{
        m_collided_bottom = false;
    }
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - ((m_width - 3) / 2), m_position.y,1.0f);
    glm::vec3 right = glm::vec3(m_position.x + ((m_width - 3) / 2), m_position.y, 1.0f);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y))
    {
        m_collided_left = true;
    }
    else{
        m_collided_left = false;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y))
    {
        m_collided_right = true;
    }
    else{
        m_collided_right = false;
    }
    
}

bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

void Entity::update(float delta_time, Entity* player, Entity* collidable_entities, int object_count, Map* map)
{
    if(is_enemy){
        if(m_ai_type == WALKER){
            if(m_position.x <= 6.0f && m_movement.x == -1.0f){
                m_movement.x = 1.0f;
                m_animation_indices = m_walking[RIGHT];
            }
            else if(m_position.x >= 38.0f && m_movement.x == 1.0f){
                m_movement.x = -1.0f;
                m_animation_indices = m_walking[LEFT];
            }
        }
        else if(m_ai_type == JUMPER){
            if(m_position.y <= -6.0f && m_movement.y == -1.0f){
                m_movement.y = 1.0f;
            }
            else if(m_position.y >= 4.0f && m_movement.y == 1.0f){
                m_movement.y = -1.0f;
            }
        }
        else if(m_ai_type == HEARTBEAT){

            g_angle += 0.02f;
            g_y_coords = 8.0f * glm::cos(g_angle);
            g_x_coords = 8.0f * glm::sin(g_angle);
                
            g_x_coords += 10.0f * delta_time;
            g_y_coords += 10.0f * delta_time;
            
            m_position = glm::vec3(g_x_coords, g_y_coords, 0.0f);
        }
    }
    check_collision_y(map);
    check_collision_x(map);
    if(!is_enemy){
        check_collision(collidable_entities, object_count);
        if(status == LOSE){
            return;
        }
        int dead = 0;
        for(int i = 0; i < object_count; ++i){
            if(collidable_entities[i].m_disappear){
                dead ++;
            }
        }
        if(dead == object_count){
            status = WIN;
        }
    }
    
    if(m_collided_bottom){
        if(!is_enemy){
            m_movement.y = -1.0f;
        }
    }
    if(m_collided_top){
        if(!is_enemy){
            m_movement.y = 0.0f;
        }
    }
    if(!m_collided_top && !m_collided_bottom){
        if(!is_enemy){
            m_movement.y = -1.0f;
        }
    }
    if(m_movement.x == 1.0f || m_movement.x == -1.0f){
        if (m_animation_indices != NULL)
        {
            if (glm::length(m_movement) != 0)
            {
                m_animation_time += delta_time;
                float frames_per_second = (float) 1 / SECONDS_PER_FRAME;
                
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
        if(m_movement.x == 1.0f){
            if(m_collided_right){
                m_movement.x = 0.0f;
            }
        }
        else if(m_movement.x == -1.0f){
            if(m_collided_left){
                m_movement.x = 0.0f;
            }
        }
    }
    if(jump_stop == false){ //still on the process of jumping
        if(jump_start == true){ //at up direction
            if(jump_height < 100){
                if(m_collided_bottom){
                    jump_start = false;
                }
                m_movement.y = 1.0;
                jump_height += 1;
            }
            else{
                jump_start = false; //at down direction
            }
        }
        if(jump_start == false){
            if(jump_height > 0 && !m_collided_top){
                m_movement.y = -1.0;
                jump_height -= 1;
            }
            else{
                m_movement.y = 0.0f;
                jump_height = 0;
                jump_stop = true; //at down direction
            }
        }
    }
    m_position += m_movement * m_velocity * delta_time;
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, m_transform);
}


void Entity::render(ShaderProgram *program)
{
    program->set_model_matrix(m_model_matrix);
    
    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }
    
    float vertices[]   = { -2.0, -2.0, 2.0, -2.0, 2.0, 2.0, -2.0, -2.0, 2.0, 2.0, -2.0, 2.0 };
    float tex_coords[] = {  0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
