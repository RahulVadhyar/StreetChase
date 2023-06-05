#include <C:\Users\vadhy\StreetChase\definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main

//command:clang++ -target x86_64-pc-windows-msvc  main.cpp glad/glad.c -o main.exe -l"C:\Users\vadhy\StreetChase\GLFW\lib\glfw3dll.lib"&& .\main.exe 
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader shader1("C:\\Users\\vadhy\\StreetChase\\Shaders\\shader.vs", "C:\\Users\\vadhy\\StreetChase\\Shaders\\shader.fs");

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &shader1, STONE_TEX_DIR);
    RenderObject wall(-0.4, -0.3, 0.4, 0.2, &shader1, STONE_TEX_DIR);
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    // player.gravity = false;
    player.shouldRender = true;
    player.addCollisionObject(&ground);
    player.addCollisionObject(&wall);
    window.addRenderObject(&ground);
    window.addRenderObject(&wall);
    window.addRenderObject(&player);
    window.addPlayer(&player);

    //render and play the game
    window.play();

    return 0;
}