#include </home/rahul/StreetChase/definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
int main(){
    //Initalize OpenGl with a window
    Window window;

    //create a shader
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(0, -1.0f, 2.0f, 1.0f, &shader1, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.3, 0.4, 0.2, &shader1, STONE_TEX_DIR);
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    //game loop 
    player.addCollisionObject(&ground);
    player.addCollisionObject(&wall);
    window.addRenderObject(&ground);
    window.addRenderObject(&wall);
    window.addRenderObject(&player);
    window.addPlayer(&player);
    window.play();

    return 0;
}