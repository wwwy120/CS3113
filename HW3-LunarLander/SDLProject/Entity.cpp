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

Entity::Entity()
{
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);
    m_speed = 0.0f;
    m_model_matrix = glm::mat4(1.0f);
    m_transform = glm::vec3(1.0f);
}

bool const Entity::check_collision(Entity* other) const
{
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);
    
    return (x_distance < 0.0f && y_distance < 0.0f) || (m_position.x <= -32 || m_position.x >= 32);
}

void Entity::game_status(Entity* other){
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);
    if(m_position.x <= -32 || m_position.x >= 32){
        m_game_stop = true;
        m_win = false;
        return;
    }
    if(x_distance < 0.0f && y_distance < 0.0f){
        m_game_stop = true;
        if(x_distance <= y_distance){
            m_win = true;
            return;
        }
        else{
            m_win = false;
            return;
        }
    }
}


void Entity::update(float delta_time, Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        game_status(&collidable_entities[i]);
        if (check_collision(&collidable_entities[i])) return;
    }

    //gravity
    m_velocity.y += m_acceleration.y * delta_time;
    m_position.y += m_velocity.y * delta_time;
    
    //move left or right
    if(m_movement.x == 1.0f){
        m_left_or_right = 1.0f;
        m_acceleration.x += 2.0f * delta_time;
        m_velocity.x += m_acceleration.x * delta_time;
        m_position.x += m_velocity.x * delta_time;
    }
    else if(m_movement.x == -1.0f){
        m_left_or_right = -1.0f;
        m_acceleration.x += -2.0f * delta_time;
        m_velocity.x += m_acceleration.x * delta_time;
        m_position.x += m_velocity.x * delta_time;
    }
    else if(m_movement.x == 2.0f && m_fuel > 0){
        m_fuel -= 1;
        if(m_left_or_right == 1.0f){
            m_acceleration.x += 5.0f * delta_time;
            m_velocity.x += m_acceleration.x * delta_time;
            m_position.x += m_velocity.x * delta_time;
        }
        else if(m_left_or_right == -1.0f){
            m_acceleration.x += -5.0f * delta_time;
            m_velocity.x += m_acceleration.x * delta_time;
            m_position.x += m_velocity.x * delta_time;
        }
    }
    else{
        if(m_velocity.x < -0.5){
            m_acceleration.x += 10.0f * delta_time;
        }
        else if(m_velocity.x > 0.5){
            m_acceleration.x -= 10.0f * delta_time;
        }
        else{
            m_velocity.x = 0.0f;
            m_acceleration.x = 0.0f;
        }
        m_velocity.x += m_acceleration.x * delta_time;
        m_position.x += m_velocity.x * delta_time;
    }

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, m_transform);

}

void Entity::render(ShaderProgram* program)
{
    program->set_model_matrix(m_model_matrix);

    float vertices[]   = { -2.0, -2.0, 2.0, -2.0, 2.0, 2.0, -2.0, -2.0, 2.0, 2.0, -2.0, 2.0};
    float tex_coords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
