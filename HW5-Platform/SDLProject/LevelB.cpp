#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 36
#define LEVEL_HEIGHT 8

unsigned int LEVEL_DATA_B[] =
{
    39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 33, 34, 34, 35,  0,  0,  0,  0,  0,  0,  0,   0, 37, 38, 39,  0,  0,  0,  0,  0,  0,  0,  0,
    39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 33, 34, 34, 35,  0,  0,  0,  0,  0,  0,  0,   0, 37, 38, 39,  0,  0,  0,  0,  0,  0,  0,  0,
    39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 33, 34, 34, 35,  0,  0,  0,  0,  0,  0,  0,   0, 37, 38, 39,  0,  0,  0,  0,  0,  0,  0,  0,
    39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 49, 50, 50, 51,  0,  0,  0,  0,  0,  0,  0,   0, 37, 38, 39,  0,  0,  0,  0,  0,  0,  0,  0,
    39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,  0,   0, 53, 54, 55,  0,  0,  0,  0,  0,  0,  0,  0,
    39,  0,  0,  0,  0,  0,  0,  0,  0, 17, 18, 18, 18, 19,  0,  0,  0,  0,  0, 72, 73, 74, 24,  0,   0,  0,  0,  0,  0,  0,  0, 93, 94, 94, 94, 95,
    39, 18, 18, 18, 18, 18, 18, 18, 18, 34, 34, 34, 34, 35,  0, 21, 22, 22, 23,  0,  0,  0, 68, 71,  71, 22, 71, 23, 40,  0,  0,  0,  0,  0,  0,  0,
    39, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35,  0, 37, 38, 38, 39, 89,  0,  0, 37, 38, 38, 38, 38, 39, 56, 77, 78, 78, 78, 78, 78, 78
};

LevelB::~LevelB()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelB::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_B, map_texture_id, 1.0f, 16, 7);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(13.0f, -4.1f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/character.png");
    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT]  = new int[3] { 15, 16, 17 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[3] { 27, 28, 29 };
//    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 2, 6, 10, 14 };
//    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 3;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 12;
    m_state.player->m_animation_rows   = 8;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/images/enemy2.png");
    
    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(20.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.jump_sfx = Mix_LoadWAV("assets/audio/mixkit-player-jumping-in-a-video-game-2043.wav");
}

void LevelB::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }
}


void LevelB::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
