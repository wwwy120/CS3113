#define LOG(argument) std::cout << argument << '\n'
#define STB_IMAGE_IMPLEMENTATION
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define NUMBER_OF_ENEMIES 3
#define FIXED_TIMESTEP 0.0166666f

#ifdef _WINDOWS
    #include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "Entity.h"
#include <vector>
#include <ctime>
#include "cmath"
//#include "Map.h"
#include "Map.hpp"

// ————— STRUCTS AND ENUMS —————//
struct GameState
{
    Entity *player;
    Entity *enemies;
    
    Map *map;
};

unsigned int LEVEL_1_DATA[] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0, 10, 11, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 73, 74,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  58,  0, 0,  0,  17, 18, 18, 18, 18, 19,
    0,  0,  0,  0,  0, 58,  0,  0,  5,  6,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  33, 34, 34, 34, 34, 35,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  92, 33, 34, 34, 34, 34, 35,
    0,  0,  0, 92,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  33, 34, 34, 34, 34, 35,
    22, 23, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 26, 27, 27, 28, 33, 34, 34, 34, 34, 35,
    38, 39, 0,  0,  0,  58, 0,  0,  0, 21, 22, 22, 22, 22, 22, 22, 23,  0,  0,  0,  0,  0,  0,  0, 0,  0,  33, 34, 34, 34, 34, 35,
    38, 39, 0,  0,  0,  0,  0,  0,  0, 37, 38, 38, 38, 38, 38, 38, 39,  0,  0,  0,  0,  0,  0,  0, 0,  0,  33, 34, 34, 34, 34, 35,
    38, 39, 0,  0,  0,  0, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 33, 34, 34, 34, 34, 35,
    38, 39, 0,  0,  0,  0, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 33, 34, 34, 34, 34, 35
};

const int LEVEL1_WIDTH = 32;
const int LEVEL1_HEIGHT = 14;

// ————— CONSTANTS ————— //
const int WINDOW_WIDTH  = 1280,
          WINDOW_HEIGHT = 560;

const float BG_RED     = 1.0f,
            BG_BLUE    = 0.808f,
            BG_GREEN   = 0.965f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;
const char CHARACTER_FILEPATH[] = "/Users/arielwang/Desktop/HW4-Rise of the AI/assets/character.png";
const char ENEMY3_FILEPATH[]  = "/Users/arielwang/Desktop/HW4-Rise of the AI/assets/enemy_heartbeat.png";
const char MAP_TILESET_FILEPATH[] = "/Users/arielwang/Desktop/HW4-Rise of the AI/assets/tilemap.png";
const char FONT_FILEPATH[] = "/Users/arielwang/Desktop/HW4-Rise of the AI/assets/font.png";

const int NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL  = 0;
const GLint TEXTURE_BORDER   = 0;

// ————— VARIABLES ————— //
GameState g_game_state;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

// ———— GENERAL FUNCTIONS ———— //
GLuint load_texture(const char* filepath)
{
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
    
    if (image == NULL)
    {        LOG("Unable to load image. Make sure the path is correct.");        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    
    return textureID;
}

const int FONTBANK_SIZE = 16;

void draw_text(ShaderProgram *program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int spritesheet_index = (int) text[i];  // ascii value of character
        float offset = (screen_size + spacing) * i;
        
        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float) (spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float) (spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
        });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
        });
    }

    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
    
    program->set_model_matrix(model_matrix);
    glUseProgram(program->get_program_id());
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int) (text.size() * 6));
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW4: Rise of the AI",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix       = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-64.0f, 64.0f, -28.0f, 28.0f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
    
    // ————— MAP SET-UP ————— //
    GLuint map_texture_id = load_texture(MAP_TILESET_FILEPATH);
    g_game_state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, LEVEL_1_DATA, map_texture_id, 4.0f, 16, 7);  // Look at this beautiful initialisation. That's literally it
    
    // ————— PLAYER ————— //
    g_game_state.player = new Entity();
    g_game_state.player->set_position(glm::vec3(14.0f, 14.0f, 1.0f));
    g_game_state.player->set_movement(glm::vec3(0.0f));
    g_game_state.player->set_velocity(glm::vec3(8.0f, 8.0f, 0.0f));
    g_game_state.player->set_texture_id(load_texture(CHARACTER_FILEPATH));
    g_game_state.player->m_width = 4.0f;
    g_game_state.player->m_height = 4.0f;
    
    g_game_state.player->m_walking[g_game_state.player->LEFT]  = new int[3] { 15, 16, 17 };
    g_game_state.player->m_walking[g_game_state.player->RIGHT] = new int[3] { 27, 28, 29 };

    g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->LEFT];
    g_game_state.player->m_animation_time    = 0.0f;
    g_game_state.player->m_animation_frames  = 3;
    g_game_state.player->m_animation_index   = 0;
    g_game_state.player->m_animation_cols    = 12;
    g_game_state.player->m_animation_rows    = 8;
    
    g_game_state.player->is_enemy = false;
    
    // ————— ENEMIES ————— //
    g_game_state.enemies = new Entity[NUMBER_OF_ENEMIES];
    
    //jumping ai
    g_game_state.enemies[0].set_position(glm::vec3(33.0f, -6.0f, 1.0f));
    g_game_state.enemies[0].set_movement(glm::vec3(0.0f, 1.0f, 0.0f));
    g_game_state.enemies[0].set_velocity(glm::vec3(0.0f, 8.0f, 0.0f));
    g_game_state.enemies[0].set_texture_id(load_texture(CHARACTER_FILEPATH));
    g_game_state.enemies[0].m_width = 4.0f;
    g_game_state.enemies[0].m_height = 4.0f;
    
    g_game_state.enemies[0].m_walking[g_game_state.enemies[0].LEFT]  = new int[3] { 10, 11, 12 };
    
    g_game_state.enemies[0].m_animation_indices = g_game_state.enemies[0].m_walking[g_game_state.enemies[0].LEFT];
    g_game_state.enemies[0].m_animation_time    = 0.0f;
    g_game_state.enemies[0].m_animation_frames  = 3;
    g_game_state.enemies[0].m_animation_index   = 0;
    g_game_state.enemies[0].m_animation_cols    = 12;
    g_game_state.enemies[0].m_animation_rows    = 8;
        
    g_game_state.enemies[0].is_enemy = true;
    g_game_state.enemies[0].set_ai_type(JUMPER);
    
    //walking ai
    g_game_state.enemies[1].set_position(glm::vec3(18.0f, -18.0f, 1.0f));
    g_game_state.enemies[1].set_movement(glm::vec3(-1.0f, 0.0f, 1.0f));
    g_game_state.enemies[1].set_velocity(glm::vec3(1.0f, 0.0f, 0.0f));
    g_game_state.enemies[1].set_texture_id(load_texture(CHARACTER_FILEPATH));
    g_game_state.enemies[1].m_width = 4.0f;
    g_game_state.enemies[1].m_height = 4.0f;
    
    g_game_state.enemies[1].m_walking[g_game_state.enemies[1].LEFT]  = new int[3] { 18, 19, 20 };
    g_game_state.enemies[1].m_walking[g_game_state.enemies[1].RIGHT] = new int[3] { 30, 31, 32 };
    g_game_state.enemies[1].m_animation_indices = g_game_state.enemies[1].m_walking[g_game_state.enemies[1].LEFT];
    g_game_state.enemies[1].m_animation_time    = 0.0f;
    g_game_state.enemies[1].m_animation_frames  = 3;
    g_game_state.enemies[1].m_animation_index   = 0;
    g_game_state.enemies[1].m_animation_cols    = 12;
    g_game_state.enemies[1].m_animation_rows    = 8;
        
    g_game_state.enemies[1].is_enemy = true;
    g_game_state.enemies[1].set_ai_type(WALKER);
    
    
    //heartbeat ai
    g_game_state.enemies[2].set_position(glm::vec3(-14.0f, -8.0f, 1.0f));
    g_game_state.enemies[2].set_movement(glm::vec3(0.0f));
    g_game_state.enemies[2].set_texture_id(load_texture(ENEMY3_FILEPATH));
    g_game_state.enemies[2].m_width = 4.0f;
    g_game_state.enemies[2].m_height = 4.0f;
    
    g_game_state.enemies[2].m_walking[g_game_state.enemies[2].LEFT]  = new int[1] {0};
    g_game_state.enemies[2].m_animation_indices = g_game_state.enemies[2].m_walking[g_game_state.enemies[2].LEFT];
    g_game_state.enemies[2].m_animation_time    = 0.0f;
    g_game_state.enemies[2].m_animation_frames  = 1;
    g_game_state.enemies[2].m_animation_index   = 0;
    g_game_state.enemies[2].m_animation_cols    = 3;
    g_game_state.enemies[2].m_animation_rows    = 2;
        
    g_game_state.enemies[2].is_enemy = true;
    g_game_state.enemies[2].set_ai_type(HEARTBEAT);
    
    

    // ————— GENERAL ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_game_state.player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q: g_game_is_running = false;
                    default:     break;
                }
                
            default:
                break;
        }
    }
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_LEFT])
    {
        g_game_state.player->move_left();
        g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->LEFT];
        g_game_state.player->CURRENT_DIRECTION = g_game_state.player->LEFT;
    }
    else if (key_state[SDL_SCANCODE_RIGHT])
    {
        g_game_state.player->move_right();
        g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->RIGHT];
        g_game_state.player->CURRENT_DIRECTION = g_game_state.player->RIGHT;
    }
    else if (key_state[SDL_SCANCODE_UP])
    {
        g_game_state.player->set_movement(glm::vec3(0.0f, 1.0f, 0.0f));
        g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->RIGHT];
        g_game_state.player->CURRENT_DIRECTION = g_game_state.player->RIGHT;
    }
    else if (key_state[SDL_SCANCODE_DOWN])
    {
        g_game_state.player->set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
        g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->RIGHT];
        g_game_state.player->CURRENT_DIRECTION = g_game_state.player->RIGHT;
    }
    if (key_state[SDL_SCANCODE_SPACE])
    {
        g_game_state.player->move_jump();
        g_game_state.player->m_animation_indices = g_game_state.player->m_walking[g_game_state.player->CURRENT_DIRECTION];
    }
    
    if (glm::length(g_game_state.player->get_movement()) > 1.0f)
    {
        g_game_state.player->set_movement(glm::normalize(g_game_state.player->get_movement()));
    }
}

void update()
{
    if(g_game_state.player->status == CONTINUE){
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        delta_time += g_accumulator;

        if (delta_time < FIXED_TIMESTEP)
        {
            g_accumulator = delta_time;
            return;
        }
        
        while (delta_time >= FIXED_TIMESTEP)
        {
            // Notice that we're using FIXED_TIMESTEP as our delta time
            g_game_state.player->update(FIXED_TIMESTEP, g_game_state.player, g_game_state.enemies, 3, g_game_state.map);
            for(int i = 0 ; i < NUMBER_OF_ENEMIES; ++i){
                g_game_state.enemies[i].update(FIXED_TIMESTEP, &g_game_state.enemies[i], NULL, 0, g_game_state.map);
            }
            delta_time -= FIXED_TIMESTEP;
            
            
        }

        g_accumulator = delta_time;
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_game_state.map->render(&g_shader_program);
    
    g_game_state.player->render(&g_shader_program);
    
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    {
        g_game_state.enemies[i].render(&g_shader_program);
    }
    if(g_game_state.player->status == WIN){
        glClear(GL_COLOR_BUFFER_BIT);
        draw_text(&g_shader_program, load_texture(FONT_FILEPATH), "WIN", 20.0f, 1, glm::vec3(-40.0f, 0.0f, 0.0f));
    }
    else if(g_game_state.player->status == LOSE){
        glClear(GL_COLOR_BUFFER_BIT);
        draw_text(&g_shader_program, load_texture(FONT_FILEPATH), "LOSE", 20.0f, 1, glm::vec3(-40.0f, 0.0f, 0.0f));
    }
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() {
    delete g_game_state.map;
    SDL_Quit();
}

// ————— DRIVER GAME LOOP ————— /
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }
    
    shutdown();
    return 0;
}
