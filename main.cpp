#include <C:\Users\vadhy\StreetChase\definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main

//command:clang++ main.cpp glad/glad.c  -o main.exe "C:\Users\vadhy\StreetChase\GLFW\lib\glfw3dll.lib" -Wall -Wextra -O3&& .\main.exe
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader ground_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader wall_shader(VS_SHADER_DIR, FS_SHADER_DIR);
    Shader player_shader(VS_SHADER_DIR, FS_SHADER_DIR);

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &ground_shader, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.3, 0.4, 0.2, &wall_shader, STONE_TEX_DIR);
    PlayerClass player(0.1f, 0.2f, &player_shader, PLAYER_TEX_DIR);

    wall.attachTexture(PLAYER_TEX_DIR);
    wall.attachTexture(CONTAINER_TEX_DIR);
    wall.mixValue[0] = 0.5f;
    wall.mixValue[1] = 0.5f;

    // player.gravity = false;
    player.shouldRender = true;
    player.addCollisionObject(&ground);
    player.addCollisionObject(&wall);
    window.addRenderObject(&ground);
    window.addRenderObject(&wall);
    window.addRenderObject(&player);
    // window.addRenderObject(&hud1);
    window.addPlayer(&player);

    //render and play the game
    window.play();

    return 0;
}