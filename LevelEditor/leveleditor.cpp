#include </home/rahul/StreetChase/LevelEditor/leveleditor_definitons.hpp>

int main(){
    //Initalize  window
    Window window;

    //create a shader
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(0, -1.0f, 20.0f, 1.0f, &shader1, STONE_TEX_DIR);
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    player.shouldRender = false;
    player.addCollisionObject(&ground);
    window.addRenderObject(&ground);
    window.addRenderObject(&player);
    window.addPlayer(&player);

    //render and play the game
    window.play();

    return 0;
}