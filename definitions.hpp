//only allow this to be defined once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

//window dimensions, 16:9 aspect ratio
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 16*SCREEN_HEIGHT/9

//screen height and width
struct ScreenStatus{
    float height;
    float width;
};
ScreenStatus screen_status = {SCREEN_HEIGHT, SCREEN_WIDTH};

//sound
#define MUSIC_DIR "Music/music1.mp3"

//fonts
#define FONT1_DIR "fonts/AmaticSC-Bold.ttf"

//variable for stbi header
#define STB_IMAGE_IMPLEMENTATION
//DEBUGGING tools

//person class
// #define INPUT_STATUS //uncomment to enable input status debugging
// #define PRINT_PLAYER_STATUS //uncomment to enable player status debugging
// #define PRINT_PLAYER_COLLISION_STATUS
// #define PLAYER_UPDATE_DEBUG

//menu debug
// #define MENU_ALLOC_DEALLOC_DEBUG
// #define MENU_DRAW_DEBUG

//window debug
// #define WIREFRAME_MODE //uncomment to enable wireframe mode
// #define SHOW_FPS
// #define SHOW_SCREEN_X
// #define SINGLE_BUFFER
// #define VIEWPORT_SIZE_DEBUG
// #define WINDOW_INIT_DEBUG
// #define WINDOW_VECTOR_STATUS
// #define WINDOW_RENDER_DEBUG

//HealthBarClass debug
// #define HEALTHBAR_DEBUG

//PhysicsClass debug
// #define PHYSICS_UPDATE_DEBUG
// #define PHYSICS_COLLISION_DEBUG

//RenderClass debug
// #define RENDER_DRAW_DEBUG
//  #define RENDER_INIT_DEBUG

//AnimationClass debug
// #define ANIMATION_DRAW_DEBUG
//#define ANIMATION_INIT_DEBUG

//AnimationRenderClass debug
// #define ANIMATION_RENDER_DRAW_DEBUG
// #define ANIMATION_RENDER_INIT_DEBUG

//BulletClass debug
//  #define BULLET_DEBUG

//PersonClass debug
// #define PERSON_UPDATE_BULLET_DEBUG
// #define PERSON_FIRE_WEAPON_DEBUG
// #define PERSON_ADD_WEAPON_DEBUG

//EnemyClass debug
// #define ENEMY_DEBUG

//HUDClass debug
// #define HUD_DEBUG

//BackgroundClass debug
// #define BACKGROUND_DEBUG

//HealthClass debug
// #define HEALTH_DEBUG

//Shaders debug
// #define SHADER_INIT_DEBUG

//WeaponClass debug
//  #define WEAPON_DEBUG

//need to implement
//TextClass debug

//standard libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <map>
#include <thread>

//Third party libraries(OpenGL, stbi, glm, freetype and irrKlang)

#include "glad/glad.h"
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "include/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
// #include "SoundEngine/irrKlang-64bit-1.6.0/include/irrKlang.h"

//Header files to be included
#include "locations.hpp" //location for all the textures and sahaders
#include "transforms.hpp" //contains the functions for transformations
#include "shaders.hpp"
#include "TextClass.hpp"
#include "RenderClass.hpp"
#include "AnimationRenderClass.hpp"
#include "AnimationClass.hpp"
#include "PhysicsClass.hpp"
#include "BulletClass.hpp"
#include "Weapons/simplebullet.hpp"
#include "WeaponClass.hpp"
#include "Weapons/simplegun.hpp"
#include "HealthClass.hpp"
#include "PersonClass.cpp"
#include "HealthBarClass.cpp"
#include "EnemyClass.hpp"
#include "PlayerClass.hpp"
#include "HUDClass.hpp"
#include "BackgroundClass.hpp"
#include "MenuClass.hpp"
#include "window.hpp" //contains the functions for initalizing and terminating glfw and inputs processing

#endif