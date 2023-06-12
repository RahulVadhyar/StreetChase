//only allow this to be defined once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

//window dimensions, 16:9 aspect ratio
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 16*SCREEN_HEIGHT/9

//textures, might eventually move to other files as strings
#define IMAGE1_DIR "Textures/container.jpg"
#define STONE_TEX_DIR "Textures/stone.jpg"
#define PLAYER_TEX_DIR "Textures/player.png"
#define CONTAINER_TEX_DIR "Textures/container.jpg"
#define GUN_TEX_DIR "Textures/gun.png"
#define BULLET_TEX_DIR "Textures/bullet.png"
#define VS_SHADER_DIR "Shaders/shader.vs"
#define FS_SHADER_DIR "Shaders/shader.fs"
#define VS_HEALTHBAR_SHADER_DIR "Shaders/healthbar.vs"
#define FS_HEALTHBAR_SHADER_DIR "Shaders/healthbar.fs"
#define VS_TEXT_SHADER_DIR "Shaders/TextShader.vs"
#define FS_TEXT_SHADER_DIR "Shaders/TextShader.fs"
#define FONT1_DIR "fonts/AmaticSC-Bold.ttf"

//variable for stbi header
#define STB_IMAGE_IMPLEMENTATION
//debugging tools
// #define WIREFRAME_MODE //uncomment to enable wireframe mode
// #define INPUT_STATUS //uncomment to enable input status debugging
// #define PRINT_PLAYER_STATUS //uncomment to enable player status debugging
// #define PRINT_COLLISION_STATUS
// #define SHOW_FPS
// #define SHOW_SCREEN_X
// #define WEAPON_DEBUG
// #define GCC //uncomment if using GCC compiler

//standard libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <map>

//Third party libraries(OpenGL, stbi and glm)

#include "glad/glad.h"
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "include/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

//Header files to be included
#include <transforms.hpp> //contains the functions for transformations
#include "shaders.hpp"
// #include <TextClass.hpp>
#include <RenderClass.hpp> 
#include <PhysicsClass.hpp>
#include <BulletClass.hpp>
#include <Weapons/simplebullet.hpp>
#include <WeaponClass.hpp>
#include <Weapons/simplegun.hpp>
#include <HealthClass.hpp>
#include <PersonClass.cpp>
#include <HealthBarClass.cpp>
#include <EnemyClass.hpp>
#include <PlayerClass.hpp>
#include <HUDClass.hpp>
#include <MenuClass.hpp>
#include <window.hpp> //contains the functions for initalizing and terminating glfw and inputs processing

#endif