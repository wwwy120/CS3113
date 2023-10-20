/**
* Author: Ariel Wang
* Assignment: Pong
* Date due: 2023-10-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECIATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION //for texture
#define LOG(argument) std::cout << argument << '\n' //for load_texture()


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

//------------------------------CONSTANTS------------------------------
const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 1.0f,
            BG_BLUE    = 0.651f,
            BG_GREEN   = 0.914f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const float MILLISECONDS_IN_SECOND = 1000.0;

//texture picture
//const char BALL_TEXTURE[] = "/Users/arielwang/Desktop/HW2-Pong/assets/art/flower.png";
//const char PLAYER_TEXTURE[] = "/Users/arielwang/Desktop/HW2-Pong/assets/art/player.png";
//const char TEXT_TEXTURE1[] = "/Users/arielwang/Desktop/HW2-Pong/assets/art/player1win.png";
//const char TEXT_TEXTURE2[] = "/Users/arielwang/Desktop/HW2-Pong/assets/art/player2win.png";
const char BALL_TEXTURE[] = "art/flower.png";
const char PLAYER_TEXTURE[] = "art/player.png";
const char TEXT_TEXTURE1[] = "art/player1win.png";
const char TEXT_TEXTURE2[] = "art/player2win.png";

//------------------------------Shader Path------------------------------
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

//------------------------------Global Variables------------------------------
SDL_Window* g_display_window;
bool g_game_is_running = true;
ShaderProgram g_shader_program;

glm::mat4 g_view_matrix;
glm::mat4 g_model_matrix1; //player1
glm::mat4 g_model_matrix2; //player2
glm::mat4 g_model_ball; //ball
glm::mat4 g_projection_matrix;

//Extra Credit
glm::mat4 g_model_text1; //text at the end of the game (player1 win)
glm::mat4 g_model_text2; //text at the end of the game (player2 win)

//delta time
float g_previous_ticks = 0.0f;

//game mode(t): 1 player or 2 players
bool g_game_is_two_players = true;

//ball translation
float g_translate_x = 0.0f;
float g_translate_y = 0.0f;

//is true when someone win the game
bool g_game_is_stop = false;

//text scaling
float g_scale = 0.0f;
int g_frame_counter = 0;
const int MAX_FRAME = 12;

//-------------------------------------other variables-------------------------------------
GLuint ball_texture_id;
GLuint player_texture_id;
GLuint text1_texture_id;
GLuint text2_texture_id;

glm::vec3 player1_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 player1_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 player2_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 player2_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_position    = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_movement    = glm::vec3(0.0f, 0.0f, 0.0f);

float player_speed = 50.0f;

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
    g_display_window = SDL_CreateWindow("HW2: Pong",
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
    
    ball_texture_id = load_texture(BALL_TEXTURE);
    player_texture_id = load_texture(PLAYER_TEXTURE);
    text1_texture_id = load_texture(TEXT_TEXTURE1);
    text2_texture_id = load_texture(TEXT_TEXTURE2);

    g_view_matrix        = glm::mat4(1.0f);
    g_model_matrix1      = glm::mat4(1.0f);
    g_model_matrix2      = glm::mat4(1.0f);
    g_model_ball         = glm::mat4(1.0f);
    g_model_text1        = glm::mat4(1.0f);
    g_model_text2        = glm::mat4(1.0f);
    
    g_projection_matrix = glm::ortho(-32.0f, 32.0f, -24.0f, 24.0f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());
    
    //bend texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}

void process_input()
{
    player1_movement = glm::vec3(0.0f);
    player2_movement = glm::vec3(0.0f);
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
            g_game_is_running = false;
        }
    }
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_T]){
        g_game_is_two_players = !g_game_is_two_players;
    }
    
    if(g_game_is_two_players){
        if (key_state[SDL_SCANCODE_S])
        {
            player1_movement.y = -1.0f;
        }
        else if (key_state[SDL_SCANCODE_W])
        {
            player1_movement.y = 1.0f;
        }
        if (key_state[SDL_SCANCODE_UP])
        {
            player2_movement.y = 1.0f;
        }
        else if (key_state[SDL_SCANCODE_DOWN])
        {
            player2_movement.y = -1.0f;
        }
    }
    else{
        if (key_state[SDL_SCANCODE_UP])
        {
            player1_movement.y = 1.0f;
            player2_movement.y = 1.0f;
        }
        else if (key_state[SDL_SCANCODE_DOWN])
        {
            player1_movement.y = -1.0f;
            player2_movement.y = -1.0f;
        }
    }
}



void update()
{
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    
    if(!g_game_is_stop){
        //delta time
        if (g_translate_x > 0){
            g_translate_x += 0.03f * delta_time;
        }
        else{
            g_translate_x -= 0.03f * delta_time;
        }
        if (g_translate_y > 0){
            g_translate_y += 0.05f * delta_time;
        }
        else{
            g_translate_y -= 0.05f * delta_time;
        }
        
    //-----------------------------------Requirement 2-----------------------------------
        //--------------------ball bounce off of the top and bottom--------------------
        if (ball_position.y < -23 || ball_position.y > 23) {
            g_translate_y = 0 - g_translate_y;
            if (ball_position.y < -23){
                ball_position.y = -23;
            }
            else if (ball_position.y > 23){
                ball_position.y = 23;
            }
        }
        
    //-----------------------------------Requirement 3-----------------------------------
        //--------------------stop the game--------------------
        if (ball_position.x < -32 || ball_position.x > 32) {
            g_game_is_stop = true;
        }
    
    //-----------------------------------Requirement 1-----------------------------------
        //--------------------ball bounce off of player1--------------------
        if (ball_position.x < -23 && ball_position.x > -25 ) {
            if(ball_position.y <= player1_position.y + 4 && ball_position.y >= player1_position.y - 4){
                g_translate_x = 0 - g_translate_x;
            }
        }
        
        //--------------------ball bounce off of player2--------------------
        if (ball_position.x > 23 && ball_position.x < 25) {
            if(ball_position.y <= player2_position.y + 4 && ball_position.y >= player2_position.y - 4){
                g_translate_x = 0 - g_translate_x;
            }
        }
        
        g_model_ball = glm::translate(g_model_ball, glm::vec3(g_translate_x, g_translate_y, 0.0f));
        
        ball_position.y += g_translate_y;
        ball_position.x += g_translate_x;
        
        
        //------------------------------update movement for user input------------------------------
        g_model_matrix1 = glm::mat4(1.0f);
        g_model_matrix2 = glm::mat4(1.0f);
        
        if(player1_position.y < -20 || player1_position.y > 20){
            if(player1_position.y < -20){
                player1_position.y = -20;
            }
            else if(player1_position.y > 20){
                player1_position.y = 20;
            }
        }
        else{
            player1_position += player1_movement * player_speed * delta_time;
        }
        g_model_matrix1 = glm::translate(g_model_matrix1, player1_position);

        if(player2_position.y < -20 || player2_position.y > 20){
            if(player2_position.y < -20){
                player2_position.y = -20;
            }
            else if(player2_position.y > 20){
                player2_position.y = 20;
            }
        }
        else{
            player2_position += player2_movement * player_speed * delta_time;
        }
        g_model_matrix2 = glm::translate(g_model_matrix2, player2_position);
    }
    
    //-----------------------------------Extra Credit 2-----------------------------------
    else{
        g_scale = 90.0f * delta_time;
        g_frame_counter += 1;
        if(g_frame_counter <= MAX_FRAME){
            if(ball_position.x > 23){
                g_model_text1 = glm::scale(g_model_text1, glm::vec3(g_scale, g_scale, 1.0f));
            }
            else if(ball_position.x < -23){
                g_model_text2 = glm::scale(g_model_text2, glm::vec3(g_scale, g_scale, 1.0f));
            }
        }
        
    }
    
}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    //------------------------------player1------------------------------
    float player1[] =
    {
        -20.0f, -4.0f, -28.0f, -4.0f, -28.0f, 4.0f,
        -20.0f, -4.0f, -28.0f, 4.0f, -20.0f, 4.0f
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, player1);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    float player1_texture[] =
    {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, player1_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_matrix1, player_texture_id);

    //------------------------------player2------------------------------
    float player2[] =
    {

        20.0f, -4.0f, 28.0f, -4.0f, 28.0f, 4.0f,
        20.0f, -4.0f, 28.0f, 4.0f, 20.0f, 4.0f
    };
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, player2);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    float player2_texture[] =
    {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, player2_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_matrix2, player_texture_id);

    //------------------------------ball------------------------------
    float ball[] =
    {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, ball);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
        
    float ball_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, ball_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_ball, ball_texture_id);
    //-----------------------------------Extra Credit 2-----------------------------------
    //------------------------------player1 win text------------------------------
    float text1[] =
    {
        -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f,
        -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, text1);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
        
    float text1_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, text1_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_text1, text1_texture_id);
    
    //------------------------------player2 win text------------------------------
    float text2[] =
    {
        -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f,
        -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, text2);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
        
    float text2_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, text2_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_text2, text2_texture_id);
    
    //------------------------------other------------------------------
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());


    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }
