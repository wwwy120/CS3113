/**
* Author: Ariel Wang
* Assignment: Lunar Lander
* Date due: 2023-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECIATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION //for texture
#define LOG(argument) std::cout << argument << '\n' //for load_texture()
#define NUMBER_OF_ENEMIES 3
#define FIXED_TIMESTEP 0.0166666f
#define ACC_OF_GRAVITY -1.62f
#define ACC_OF_MOVING 5.0f
#define PLATFORM_COUNT 14
#define FUEL 100


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h" //for texture
#include "Entity.h"
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

//------------------------------CONSTANTS------------------------------
const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.133f,
            BG_BLUE    = 0.259f,
            BG_GREEN   = 0.118f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const float MILLISECONDS_IN_SECOND = 1000.0;

//texture picture
//const char PLATFORM_TEXTURE[] = "/Users/arielwang/Desktop/HW3-LunarLander/assets/art/platform.png";
//const char PLAYER_TEXTURE[] = "/Users/arielwang/Desktop/HW3-LunarLander/assets/art/player.png";
//const char TEXT_WIN_TEXTURE[] = "/Users/arielwang/Desktop/HW3-LunarLander/assets/art/win.png";
//const char TEXT_LOSE_TEXTURE[] = "/Users/arielwang/Desktop/HW3-LunarLander/assets/art/lose.png";
//const char TEXT_FUEL_TEXTURE[] = "/Users/arielwang/Desktop/HW3-LunarLander/assets/art/fuelOut.png";
const char PLATFORM_TEXTURE[] = "/art/platform.png";
const char PLAYER_TEXTURE[] = "/art/player.png";
const char TEXT_WIN_TEXTURE[] = "/art/win.png";
const char TEXT_LOSE_TEXTURE[] = "/art/lose.png";

//------------------------------Shader Path------------------------------
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

//------------------------------Global Variables------------------------------
SDL_Window* g_display_window;
bool g_game_is_running = true;
ShaderProgram g_shader_program;

glm::mat4 g_view_matrix;
glm::mat4 g_projection_matrix;

//Extra Credit
glm::mat4 g_model_text_win; //text at the end of the game
glm::mat4 g_model_text_lose; //text at the end of the game

//delta time
float g_previous_ticks = 0.0f;
float g_time_accumulator = 0.0f;

//text scaling
float g_scale = 0.0f;
int g_frame_counter = 0;
//-------------------------------------other variables-------------------------------------
GLuint surface_texture_id;
GLuint player_texture_id;
GLuint text_win_texture_id;
GLuint text_lose_texture_id;

//variables for function load_texture()
const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0; // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0; // this value MUST be zero

//-------------------------------------function prototypes---------------------------------
void initialise();
void process_input();
void update();
void render();
void shutdown();

struct GameState
{
    Entity* player;
    Entity* platforms;
    Entity* text;
};

GameState g_game_state;


// The game will reside inside the main
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

GLuint load_texture(const char* filepath)
{
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
    
    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }
    
    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    // STEP 3: Setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // STEP 4: Releasing our file from memory and returning our texture id
    stbi_image_free(image);
    
    return textureID;
}


void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW3: Lunar Lander",
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
    g_projection_matrix = glm::ortho(-32.0f, 32.0f, -24.0f, 24.0f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());
    

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
    
    // ————— PLAYER ————— //
    g_game_state.player = new Entity();
    g_game_state.player->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
    g_game_state.player->set_movement(glm::vec3(0.0f));
    g_game_state.player->set_acceleration(glm::vec3(0.0f, ACC_OF_GRAVITY * 0.1f, 0.0f));
    g_game_state.player->set_velocity(glm::vec3(10.0f, 0.0f, 0.0f));
    g_game_state.player->set_width(2.8);
    g_game_state.player->set_height(4);
    g_game_state.player->m_texture_id = load_texture(PLAYER_TEXTURE);
    g_game_state.player->set_fuel(FUEL);

    g_game_state.player->m_animation_time = 0.0f;
    
    // ————— PLATFORM ————— //
    g_game_state.platforms = new Entity[PLATFORM_COUNT];

    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        g_game_state.platforms[i].m_texture_id = load_texture(PLATFORM_TEXTURE);
        if(i == 0){
            g_game_state.platforms[i].set_position(glm::vec3(-30.0f, -20.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 2.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(2 * 4);
        }
        else if(i == 1){
            g_game_state.platforms[i].set_position(glm::vec3(-26.0f, -18.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 3.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(3 * 4);
        }
        else if(i == 2){
            g_game_state.platforms[i].set_position(glm::vec3(-20.0f, -16.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(2.0f, 4.0f, 1.0f));
            g_game_state.platforms[i].set_width(2 * 4);
            g_game_state.platforms[i].set_height(4 * 4);
        }
        else if(i == 3){
            g_game_state.platforms[i].set_position(glm::vec3(-14.0f, -20.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 2.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(2 * 4);
        }
        else if(i == 4){
            g_game_state.platforms[i].set_position(glm::vec3(-10.0f, -22.0f, 0.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(1 * 4);
        }
        else if(i == 5){
            g_game_state.platforms[i].set_position(glm::vec3(-4.0f, -14.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(2.0f, 5.0f, 1.0f));
            g_game_state.platforms[i].set_width(2 * 4);
            g_game_state.platforms[i].set_height(5 * 4);
        }
        else if(i == 6){
            g_game_state.platforms[i].set_position(glm::vec3(2.0f, -16.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 4.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(4 * 4);
        }
        else if(i == 7){
            g_game_state.platforms[i].set_position(glm::vec3(6.0f, -18.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 3.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(3 * 4);
        }
        else if(i == 8){
            g_game_state.platforms[i].set_position(glm::vec3(10.0f, -22.0f, 0.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(1 * 4);
        }
        else if(i == 9){
            g_game_state.platforms[i].set_position(glm::vec3(14.0f, -10.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 7.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(7 * 4);
        }
        else if(i == 10){
            g_game_state.platforms[i].set_position(glm::vec3(18.0f, -14.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 5.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(5 * 4);
        }
        else if(i == 11){
            g_game_state.platforms[i].set_position(glm::vec3(22.0f, -20.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 2.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(2 * 4);
        }
        else if(i == 12){
            g_game_state.platforms[i].set_position(glm::vec3(26.0f, -22.0f, 0.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(1 * 4);
        }
        else if(i == 13){
            g_game_state.platforms[i].set_position(glm::vec3(30.0f, -16.0f, 0.0f));
            g_game_state.platforms[i].set_transform(glm::vec3(1.0f, 4.0f, 1.0f));
            g_game_state.platforms[i].set_width(1 * 4);
            g_game_state.platforms[i].set_height(4 * 4);
        }

        g_game_state.platforms[i].update(0.0f, NULL, 0);
    }
    // ————— TEXT ————— //
    g_game_state.text = new Entity[3];
    g_game_state.text[0].set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    g_game_state.text[0].m_texture_id = load_texture(TEXT_WIN_TEXTURE);
    g_game_state.text[0].set_transform(glm::vec3(0.1f, 0.1f, 1.0f));
    g_game_state.text[0].set_movement(glm::vec3(0.0f));
    g_game_state.text[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    g_game_state.text[0].set_velocity(glm::vec3(10.0f, 0.0f, 0.0f));
    g_game_state.text[0].update(0.0f, NULL, 0);
    g_game_state.text[1].set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    g_game_state.text[1].m_texture_id = load_texture(TEXT_LOSE_TEXTURE);
    g_game_state.text[1].set_transform(glm::vec3(0.1f, 0.1f, 1.0f));
    g_game_state.text[1].set_movement(glm::vec3(0.0f));
    g_game_state.text[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    g_game_state.text[1].set_velocity(glm::vec3(10.0f, 0.0f, 0.0f));
    g_game_state.text[1].update(0.0f, NULL, 0);
    
    //bend texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    g_game_state.player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
            g_game_is_running = false;
        }
    }
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_LEFT])
    {
        g_game_state.player->move_left();
    }
    else if (key_state[SDL_SCANCODE_RIGHT])
    {
        g_game_state.player->move_right();
    }
    else if (key_state[SDL_SCANCODE_SPACE])
    {
        g_game_state.player->accelerate();
    }
}

void update()
{
    // ————— DELTA TIME ————— //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    g_previous_ticks = ticks;

    // ————— FIXED TIMESTEP ————— //
    // STEP 1: Keep track of how much time has passed since last step
    delta_time += g_time_accumulator;

    // STEP 2: Accumulate the ammount of time passed while we're under our fixed timestep
    if (delta_time < FIXED_TIMESTEP)
    {
        g_time_accumulator = delta_time;
        return;
    }

    // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the objects' update function invocation
    while (delta_time >= FIXED_TIMESTEP)
    {
        // Notice that we're using FIXED_TIMESTEP as our delta time
        g_game_state.player->update(FIXED_TIMESTEP, g_game_state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }

    g_time_accumulator = delta_time;
    
    if(g_game_state.player->m_game_stop){
        if(g_game_state.player->m_win){
            g_game_state.text[0].set_transform(glm::vec3(900.0 * delta_time, 900.0 * delta_time, 1.0f));
            g_game_state.text[0].update(0, NULL, 0);
        }
        else{
            g_game_state.text[1].set_transform(glm::vec3(900.0 * delta_time, 900.0 * delta_time, 1.0f));
            g_game_state.text[1].update(0, NULL, 0);
        }
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ————— PLAYER ————— //
    g_game_state.player->render(&g_shader_program);
    
    // ————— PLATFORM ————— //
    for (int i = 0; i < PLATFORM_COUNT; i++){
        g_game_state.platforms[i].render(&g_shader_program);
    }
    
    // ————— TEXT ————— //
    g_game_state.text[0].render(&g_shader_program);
    g_game_state.text[1].render(&g_shader_program);
    
    // ————— GENERAL ————— //
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }
