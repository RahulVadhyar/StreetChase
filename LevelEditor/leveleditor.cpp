#include </home/rahul/StreetChase/LevelEditor/leveleditor_definitons.hpp>
//command to compile: g++ LevelEditor/leveleditor.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &shader1, STONE_TEX_DIR);
    LevelEditor editor(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    editor.shouldRender = false;
    editor.addCollisionObject(&ground);
    window.addRenderObject(&ground);
    window.addRenderObject(&editor);
    window.addPlayer(&editor);

    //render and play the game
    window.play();

    return 0;
}