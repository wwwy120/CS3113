/**
* Author: Ariel Wang
* Assignment: Simple 2D Scene
* Date due: 2023-09-30, 11:59pm
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

// The new stuff
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h" //for texture

enum Coordinate
{
    x_coordinate,
    y_coordinate
};

//-------------------------------------CONSTANTS-------------------------------------
const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.949f,
            BG_BLUE    = 0.902f,
            BG_GREEN   = 0.769f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

//heartbeat for rectangle
const float GROWTH_FACTOR = 1.05f;  // growth rate of 1.0% per frame
const float SHRINK_FACTOR = 0.95f;  // growth rate of -1.0% per frame
const int MAX_FRAME = 30;           // how fast the heart beat

//rotate the triangle (not rotating)
//const float INIT_TRIANGLE_ANGLE = glm::radians(45.0);

//rotating the triangle
//const float ROT_ANGLE = glm::radians(0.5f);

//translating the triangle
//float TRAN_VALUE = 0.025f;

//texture picture
const char PLAYER_SPRITE1[] = "/Users/arielwang/Desktop/SDLProject 2/assets/art/blue.png";
const char PLAYER_SPRITE2[] =  "/Users/arielwang/Desktop/SDLProject 2/assets/art/flower.png";

//translate in a circle
const float RADIUS = 2.0f; //radius of transformation circle
const float ROT_SPEED = 0.01f; // rotational speed
float g_angle = 0.0f;
float g_x_coords = RADIUS; //current x-coord
float g_y_coords = 0.0f; //current y-coord


//-------------------------------------Shader filepath-------------------------------------
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";


//-------------------------------------GLOBAL variables-------------------------------------
SDL_Window* g_display_window;
bool g_game_is_running = true;
ShaderProgram g_program;

//heartbeat for triangle
int g_frame_counter = 0;
bool g_is_growing = true;

glm::mat4 g_view_matrix;
glm::mat4 g_model_matrix1; //triangle
glm::mat4 g_model_matrix2; //rectangle
glm::mat4 g_projection_matrix;

//delta time
float g_previous_ticks = 0.0f;

float g_translate = 0.0f;
float g_rotate = 0.0f;

//-------------------------------------other variables-------------------------------------
GLuint player_texture_id1;
GLuint player_texture_id2;

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
    // Part 1: Initialise our program—whatever that means
    initialise();

    while (g_game_is_running)
    {
        // Part 2: If the player did anything—press a button, move the joystick—process it
        process_input();

        // Part 3: Using the game's previous state, and whatever new input we have, update the game's state
        update();

        // Part 4: Once updated, render those changes onto the screen
        render();
    }

    // Part 5: The game is over, so let's perform any shutdown protocols
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


void initialise(){
    
    //initialize the window
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW1: Draw a Simple 2D Scene",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    //initialize the camera
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    //load the shader
    g_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    //load texture
    player_texture_id1 = load_texture(PLAYER_SPRITE1);
    player_texture_id2 = load_texture(PLAYER_SPRITE2);
    
    // Initialise our view, model, and projection matrices
    g_view_matrix       = glm::mat4(1.0f);
    g_model_matrix1     = glm::mat4(1.0f);
    g_model_matrix2     = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_program.set_view_matrix(g_view_matrix);
    g_program.set_projection_matrix(g_projection_matrix);
    
    glUseProgram(g_program.get_program_id());
    
    
    
    //blend texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
};



void process_input(){
    SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            {
                g_game_is_running = false;
            }
        }
};



void update(){
    
    float ticks = (float) SDL_GetTicks() / 1000.0f;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    g_translate += 0.5f * delta_time;
    g_rotate += 90.0 * delta_time;            // 90-degrees per second
    

    g_model_matrix1 = glm::mat4(1.0f);
    g_model_matrix2 = glm::mat4(1.0f);
    
    
    
    //--------------------------------heartbeat--------------------------------
    glm::vec3 scale_vector;
    g_frame_counter += 1;
    
    if (g_frame_counter >= MAX_FRAME)
    {
        g_is_growing = !g_is_growing;
        g_frame_counter = 0;
    }
    
    scale_vector = glm::vec3(g_is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                            g_is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                             1.0f);
    //---------------------------------------------------------------------------------------
    //--------------------------------translating in a circle--------------------------------
    g_angle += ROT_SPEED;
    g_y_coords = RADIUS * glm::cos(g_angle);
    g_x_coords = RADIUS * glm::sin(g_angle);
    
    g_x_coords += 1.0f * delta_time;
    g_y_coords += 1.0f * delta_time;
    //---------------------------------------------------------------------------------------
    
    //EXTRA Credit & PART 2: rectangle translate in a pattern(circle) and triangle translate diagonally
    g_model_matrix2 = glm::translate(g_model_matrix2, glm::vec3(g_x_coords, g_y_coords, 0.0f));
    g_model_matrix1 = glm::translate(g_model_matrix1, glm::vec3(g_translate, g_translate, 0.0f));

    
//    EXTRA Credit: change scale (rectangle)
    g_model_matrix2 = glm::scale(g_model_matrix2, scale_vector);

    //PART 3: triangle is rotating
    g_model_matrix1 = glm::rotate(g_model_matrix1, glm::radians(g_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
   
};



void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    g_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}


void render(){

    
    //-------------------------triangle and texture-------------------------
    glClear(GL_COLOR_BUFFER_BIT);
    
    float triangle[] =
    {
        -1.0f, -1.0f,  // (x1, y1)
         1.0f, -1.0f,  // (x2, y2)
         0.0f,  1.0f   // (x3, y3)
    };
    
    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, triangle);
    glEnableVertexAttribArray(g_program.get_position_attribute());
    
    float triangle_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };
    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, triangle_texture);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    
    draw_object(g_model_matrix1, player_texture_id1);

    //-----------------------rectangle and texture-------------------------
    
    float rectangle[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
//        1.0f, 1.0f, 2.0f, 1.0f, 2.0f, 2.0f,
//        1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f
    };
    
    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, rectangle);
    glEnableVertexAttribArray(g_program.get_position_attribute());

    float rectangle_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
//        1.5f, 1.5f, 2.5f, 1.5f, 2.5f, 2.5f,
//        1.5f, 1.5f, 1.5f, 2.5f, 2.5f, 2.5f
    };
    
    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, rectangle_texture);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    
    draw_object(g_model_matrix2, player_texture_id2);
    
    
    glDisableVertexAttribArray(g_program.get_position_attribute());
    glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    

    SDL_GL_SwapWindow(g_display_window);
};


void shutdown(){
    SDL_Quit();
};

