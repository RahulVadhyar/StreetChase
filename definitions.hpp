#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 16*SCREEN_HEIGHT/9



#define IMAGE1_DIR "/home/rahul/StreetChase/Textures/container.jpg"
#define STONE_TEX_DIR "/home/rahul/StreetChase/Textures/stone.jpg"
#define PLAYER_TEX_DIR "/home/rahul/StreetChase/Textures/player.png"
#define STB_IMAGE_IMPLEMENTATION

//debugging toold
// #define WIREFRAME_MODE
// #define INPUT_STATUS
#define PRINT_PLAYER_STATUS



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>

#include </home/rahul/StreetChase/glad/glad.h>
#include <GLFW/glfw3.h>
#include </home/rahul/StreetChase/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include </home/rahul/StreetChase/transforms.hpp>
#include </home/rahul/StreetChase/shaders.hpp>
#include </home/rahul/StreetChase/RenderClass.hpp>
#include </home/rahul/StreetChase/PhysicsClass.hpp>
#include </home/rahul/StreetChase/PlayerClass.hpp>
#include </home/rahul/StreetChase/window.hpp>
#endif