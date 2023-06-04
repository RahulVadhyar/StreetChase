#include </home/rahul/StreetChase/definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader ground_shader("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");
    Shader wall_shader("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");
    Shader player_shader("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &ground_shader, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.3, 0.4, 0.2, &wall_shader, STONE_TEX_DIR);
    PlayerClass player(0.1f, 0.2f, &player_shader, PLAYER_TEX_DIR);
    wall.attachTexture(STONE_TEX_DIR);
    wall.texMix = 0.5f;
    // HUDObject hud1(0.0f, -0.9f, 0.2f, 0.2f, &shader1, STONE_TEX_DIR);


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