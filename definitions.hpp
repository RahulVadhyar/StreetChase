//only allow this to be defined once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

//window dimensions, 16:9 aspect ratio
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 16*SCREEN_HEIGHT/9

//textures, might eventually move to other files as strings
#define IMAGE1_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\container.jpg"
#define STONE_TEX_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\stone.jpg"
#define PLAYER_TEX_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\player.png"
#define CONTAINER_TEX_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\container.jpg"
#define GUN_TEX_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\gun.png"
#define BULLET_TEX_DIR "C:\\Users\\vadhy\\StreetChase\\Textures\\bullet.png"


#define VS_SHADER_DIR "C:\\Users\\vadhy\\StreetChase\\Shaders\\shader.vs"
#define FS_SHADER_DIR "C:\\Users\\vadhy\\StreetChase\\Shaders\\shader.fs"
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
#include <typeinfo>

//Third party libraries(OpenGL, stbi and glm)

#include <C:\Users\vadhy\StreetChase\glad\glad.h>
#define GLFW_DLL
#include <C:\Users\vadhy\StreetChase\GLFW\include\glfw3.h>
#include <C:\Users\vadhy\StreetChase\include\stb_image.h>
#include <C:\Users\vadhy\StreetChase\glm\glm.hpp>
#include <C:\Users\vadhy\StreetChase\glm\gtc\matrix_transform.hpp>
#include <C:\Users\vadhy\StreetChase\glm\gtc\type_ptr.hpp>

//Header files to be included
#include <C:\Users\vadhy\StreetChase\transforms.hpp> //contains the functions for transformations
#include "C:\Users\vadhy\StreetChase\shaders.hpp"
#include <C:\Users\vadhy\StreetChase\RenderClass.hpp> 
#include <C:\Users\vadhy\StreetChase\PhysicsClass.hpp>
#include <C:\Users\vadhy\StreetChase\BulletClass.hpp>
#include <C:\Users\vadhy\StreetChase\Weapons\simplebullet.hpp>
#include <C:\Users\vadhy\StreetChase\WeaponClass.hpp>
#include <C:\Users\vadhy\StreetChase\Weapons\simplegun.hpp>
#include <C:\Users\vadhy\StreetChase\HealthClass.hpp>
#include <C:\Users\vadhy\StreetChase\HealthBarClass.cpp>
#include <C:\Users\vadhy\StreetChase\EnemyClass.hpp>
#include <C:\Users\vadhy\StreetChase\PlayerClass.hpp>
#include <C:\Users\vadhy\StreetChase\window.hpp> //contains the functions for initalizing and terminating glfw and inputs processing
#include <C:\Users\vadhy\StreetChase\HUDClass.hpp>
#endif