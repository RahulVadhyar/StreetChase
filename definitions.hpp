//only allow this to be defined once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

//window dimensions, 16:9 aspect ratio
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 16*SCREEN_HEIGHT/9

//textures, might eventually move to other files as strings
#define IMAGE1_DIR "/home/rahul/StreetChase/Textures/container.jpg"
#define STONE_TEX_DIR "/home/rahul/StreetChase/Textures/stone.jpg"
#define PLAYER_TEX_DIR "/home/rahul/StreetChase/Textures/player.png"
//variable for stbi header
#define STB_IMAGE_IMPLEMENTATION

//debugging tools
// #define WIREFRAME_MODE //uncomment to enable wireframe mode
// #define INPUT_STATUS //uncomment to enable input status debugging
// #define PRINT_PLAYER_STATUS //uncomment to enable player status debugging
// #define PRINT_COLLISION_STATUS

//standard libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <vector>

//Third party libraries(OpenGL, stbi and glm)
#include </home/rahul/StreetChase/glad/glad.h>
#include <GLFW/glfw3.h>
#include </home/rahul/StreetChase/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Header files to be included
#include </home/rahul/StreetChase/transforms.hpp> //contains the functions for transformations
#include </home/rahul/StreetChase/shaders.hpp>
#include </home/rahul/StreetChase/RenderClass.hpp> 
#include </home/rahul/StreetChase/PhysicsClass.hpp>
#include </home/rahul/StreetChase/PlayerClass.hpp>
#include </home/rahul/StreetChase/window.hpp> //contains the functions for initalizing and terminating glfw and inputs processing
// #include </home/rahul/StreetChase/HUDClass.hpp>
#endif