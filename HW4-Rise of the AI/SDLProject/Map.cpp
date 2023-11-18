#include "Map.hpp"

Map::Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int tile_count_x, int tile_count_y)
{
    m_width = width;
    m_height = height;
    
    m_level_data = level_data;
    m_texture_id = texture_id;
    
    m_tile_size = tile_size;
    m_tile_count_x = tile_count_x;
    m_tile_count_y = tile_count_y;
    
    build();
}

void Map::build()
{
    // Since this is a 2D map, we need a nested for-loop
    for(int y_coord = 0; y_coord < m_height; y_coord++)
    {
        for(int x_coord = 0; x_coord < m_width; x_coord++)
        {
            // Get the current tile
            int tile = m_level_data[y_coord * m_width + x_coord];
            
            // If the tile number is 0 i.e. not solid, skip to the next one
            if (tile == 0) continue;
            
            // Otherwise, calculate its UV-coordinated
            float u_coord = (float) (tile % m_tile_count_x) / (float) m_tile_count_x;
            float v_coord = (float) (tile / m_tile_count_x) / (float) m_tile_count_y;
            
            // And work out their dimensions and posititions
            float tile_width = 1.0f/ (float)  m_tile_count_x;
            float tile_height = 1.0f/ (float) m_tile_count_y;
            
            float x_offset = -(m_tile_size / 2); // From center of tile
            float y_offset =  (m_tile_size / 2); // From center of tile
            
            // So we can store them inside our std::vectors
            m_vertices.insert(m_vertices.end(), {
                x_offset + (m_tile_size * x_coord) - 62,  y_offset +  -m_tile_size * y_coord + 26,
                x_offset + (m_tile_size * x_coord) - 62,  y_offset + (-m_tile_size * y_coord) - m_tile_size + 26,
                x_offset + (m_tile_size * x_coord) - 62 + m_tile_size, y_offset + (-m_tile_size * y_coord) - m_tile_size + 26,
                x_offset + (m_tile_size * x_coord) - 62, y_offset + -m_tile_size * y_coord + 26,
                x_offset + (m_tile_size * x_coord) - 62 + m_tile_size, y_offset + (-m_tile_size * y_coord) - m_tile_size + 26,
                x_offset + (m_tile_size * x_coord) - 62 + m_tile_size, y_offset +  -m_tile_size * y_coord + 26
            });
            
            m_texture_coordinates.insert(m_texture_coordinates.end(), {
                u_coord, v_coord,
                u_coord, v_coord + (tile_height),
                u_coord + tile_width, v_coord + (tile_height),
                u_coord, v_coord,
                u_coord + tile_width, v_coord + (tile_height),
                u_coord + tile_width, v_coord
            });
        }
    }
    
    // The bounds are dependent on the size of the tiles
//    m_left_bound   = 0 - (m_tile_size / 2);
//    m_right_bound  = (m_tile_size * m_width) - (m_tile_size / 2);
//    m_top_bound    = 0 + (m_tile_size / 2);
//    m_bottom_bound = -(m_tile_size * m_height) + (m_tile_size / 2);
    m_left_bound   = -32;
    m_right_bound  = 32;
    m_top_bound    = 28;
    m_bottom_bound = -28;
}

void Map::render(ShaderProgram *program)
{
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->set_model_matrix(model_matrix);
    
    glUseProgram(program->get_program_id());
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, m_vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, m_texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    
    glDrawArrays(GL_TRIANGLES, 0, (int) m_vertices.size() / 2);
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_position_attribute());
}

bool Map::is_solid(glm::vec3 position, float *penetration_x, float *penetration_y)
{
    *penetration_x = 0;
    *penetration_y = 0;

    int tile_x = 0;
    int tile_y = 0;

    if(position.x < 0){
        tile_x = floor((((int(floor(position.x))+64) % 64)) / 4);
    }
    else{
        tile_x = floor((((int(floor(position.x))+64) % 64)) / 4) + 16;
    }
    if(position.y > 0){
        tile_y = fabs((int(ceil(position.y)) - 1) / 4 - 6);
    }
    else{
        tile_y = fabs(((int(ceil(position.y))) / 4)) + 7;
    }
    
    int tile = m_level_data[(tile_y) * 32 + tile_x];

    // If we are out of bounds, it is not solid
    if (position.x < -62 || position.x > 62)  return false;
    if (position.y > 26  || position.y < -26) return false;
    
    if (tile != 0) return true;

    return false;
}
