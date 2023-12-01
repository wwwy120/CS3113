/**
* Author: Ariel Wang
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "Menu.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"

// ————— CONSTANTS ————— //
const int   WINDOW_WIDTH    = 640,
            WINDOW_HEIGHT   = 480;

const float BG_RED     = 1.0f,
            BG_GREEN   = 0.808f,
            BG_BLUE    = 0.965f,
            BG_OPACITY = 1.0f;

const int   VIEWPORT_X = 0,
            VIEWPORT_Y = 0,
            VIEWPORT_WIDTH = WINDOW_WIDTH,
            VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
            F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

// ————— GLOBAL VARIABLES ————— //
Scene*  g_current_scene;
Menu*   g_menu;
LevelA* g_level_a;
LevelB* g_level_b;
LevelC* g_level_c;
int lives = 3;

SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_game_start = false;
bool g_level_a_done = false;
bool g_level_b_done = false;
bool g_game_stop = false;
bool g_game_win = false;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks  = 0.0f;
float g_accumulator     = 0.0f;

void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

void initialise()
{
    // ————— VIDEO ————— //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("HW5: Platforms",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // ————— GENERAL ————— //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // ————— SCENE SETUP ————— //
    g_menu    = new Menu();
    g_level_a = new LevelA();
    g_level_b = new LevelB();
    g_level_c = new LevelC();
    switch_to_scene(g_menu);
    
    // ————— LOOPING BACKGROUND MUSIC FOR WHOLE GAME ————— //
    Mix_Music* g_music = Mix_LoadMUS("assets/audio/background.mp3");
    Mix_PlayMusic(g_music, -1);
    Mix_VolumeMusic(50.0f);

    // ————— BLENDING ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if(!g_game_start && key_state[SDL_SCANCODE_RETURN]){
        g_game_start = true;
        switch_to_scene(g_level_a);
    }

    while (SDL_PollEvent(&event))
    {
        // ————— KEYSTROKES ————— //
        switch (event.type) {
            // ————— END GAME ————— //
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;

            case SDLK_SPACE:
                // ————— JUMPING ————— //
                if (g_current_scene->m_state.player->m_collided_bottom)
                {
                    g_current_scene->m_state.player->m_is_jumping = true;
                    Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
                }
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    // ————— KEY HOLD ————— //
    

    if (key_state[SDL_SCANCODE_LEFT] && g_game_start)
    {
        g_current_scene->m_state.player->move_left();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
    }
    else if (key_state[SDL_SCANCODE_RIGHT] && g_game_start)
    {
        g_current_scene->m_state.player->move_right();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
    }

    // ————— NORMALISATION ————— //
    if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
    {
        g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
    }
}

void update()
{
    if(!g_game_stop){
        // ————— LEVEL CHANGE ————— //
        if(g_current_scene->m_state.player->get_position().x > 41){
            g_game_win = true;
            g_game_stop = true;
        }
        if(g_current_scene->m_state.player->get_position().x > 13 && g_current_scene->m_state.player->get_position().x < 26  && g_current_scene != g_level_b && g_game_start && !g_level_b_done){
            switch_to_scene(g_level_b);
            g_level_a_done = true;
        }
        else if(g_current_scene->m_state.player->get_position().x < 13 && g_current_scene != g_level_a && g_game_start && !g_level_a_done){
            switch_to_scene(g_level_a);
        }
        else if(g_current_scene->m_state.player->get_position().x > 26  && g_current_scene != g_level_c && g_game_start){
            switch_to_scene(g_level_c);
            g_level_b_done = true;
        }
        
        
        // ————— DELTA TIME / FIXED TIME STEP CALCULATION ————— //
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        delta_time += g_accumulator;

        if (delta_time < FIXED_TIMESTEP)
        {
            g_accumulator = delta_time;
            return;
        }

        while (delta_time >= FIXED_TIMESTEP) {
            // ————— UPDATING THE SCENE (i.e. map, character, enemies...) ————— //
            g_current_scene->update(FIXED_TIMESTEP);

            delta_time -= FIXED_TIMESTEP;
        }

        g_accumulator = delta_time;
        
        

        // ————— PLAYER CAMERA ————— //
        g_view_matrix = glm::mat4(1.0f);

        if (g_current_scene->m_state.player->get_position().x > LEVEL1_LEFT_EDGE) {
                g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, 3.75, 0));
        } else {
                g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
        }
        
        // ————— UPDATE LIVES OF PLAYER ————— //
        if(g_current_scene != g_menu){
            if(g_current_scene->m_state.player->check_collision(&g_current_scene->m_state.enemies[0], 1)){
                lives -= 1;
                g_current_scene->m_state.player->set_position(glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f)); //get away from AI
                if(lives == 0){
                    g_game_stop = true;
                }
            }
        }
    }
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&g_shader_program);
    
    // ————— WIN/LOSE TEXT ————— //
    if(g_game_stop){
        if(g_game_win){
            Utility::draw_text(&g_shader_program, Utility::load_texture("assets/fonts/font1.png"), "WIN", 2.0f, 0.1f, glm::vec3(39.0, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
        else{
            Utility::draw_text(&g_shader_program, Utility::load_texture("assets/fonts/font1.png"), "LOSE", 2.0f, 0.1f, glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
    }
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    // ————— DELETING LEVEL DATA (i.e. map, character, enemies...) ————— //
    delete g_level_a;
    delete g_level_b;
    delete g_level_c;
}

// ————— GAME LOOP ————— //
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
