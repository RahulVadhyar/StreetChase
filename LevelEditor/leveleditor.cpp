#include <C:\Users\vadhy\StreetChase\LevelEditor\leveleditor_definitons.hpp>
//command to compile: g++ LevelEditor/leveleditor.cpp glad/glad.c -o LevelEditor.exe "C:\Users\vadhy\StreetChase\GLFW\lib\glfw3dll.lib" -Wall -Wextra -O3&& .\LevelEditor.exe
int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader shader1(shader_dir::vs::shader, shader_dir::fs::shader);

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &shader1, STONE_TEX_DIR);
    LevelEditor editor(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    HUDObject hud1(0.0f, -0.9f, 0.2f, 0.2f, &shader1, CONTAINER_TEX_DIR);

    editor.shouldRender = false;
    editor.addCollisionObject(&ground);
    window.addRenderObject(&ground);
    window.addRenderObject(&editor);
    window.addRenderObject(&hud1);
    window.addPlayer(&editor);

    //render and play the game
    window.play();

    return 0;
}