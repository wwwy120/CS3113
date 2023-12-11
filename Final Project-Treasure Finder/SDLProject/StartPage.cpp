#include "StartPage.h"
#include "Utility.h"

#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 12

unsigned int LEVEL_DATA_STARTPAGE[] =
{
     1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
    112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
    160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
    176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191
};

StartPage::~StartPage()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.bomb_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void StartPage::initialise()
{
    GLuint start_texture_id = Utility::load_texture("assets/images/start-page.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_STARTPAGE, start_texture_id, 1.0f, 16, 12);
    
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
//    m_state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/character.png");
    m_state.player->star_get = 0;
    
    // Walking
//    m_state.player->m_walking[m_state.player->LEFT]  = new int[3] { 15, 16, 17 };
//    m_state.player->m_walking[m_state.player->RIGHT] = new int[3] { 27, 28, 29 };
////    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 2, 6, 10, 14 };
////    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };
//
//    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
//    m_state.player->m_animation_frames = 3;
//    m_state.player->m_animation_index  = 0;
//    m_state.player->m_animation_time   = 0.0f;
//    m_state.player->m_animation_cols   = 12;
//    m_state.player->m_animation_rows   = 8;
//    m_state.player->set_height(0.8f);
//    m_state.player->set_width(0.8f);
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
//
//    m_state.bgm = Mix_LoadMUS("assets/audio/background.mp3");
//    Mix_PlayMusic(m_state.bgm, -1);
//    Mix_VolumeMusic(50.0f);

}

void StartPage::update(float delta_time)
{
//    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
}


void StartPage::render(ShaderProgram *program)
{
    m_state.map->render(program);
}
