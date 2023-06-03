#include </home/rahul/StreetChase/definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main
//command for number of lines : git ls-files | grep '\pp' | xargs wc -l
int main(){
    //Initalize OpenGl with a window
    GLFWwindow* window = Initalize();

    //create a shader
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(2.0f, 2.0f, &shader1, STONE_TEX_DIR);
    ground.y = -1.75f;
    RenderObject wall(0.4, 0.2, &shader1, STONE_TEX_DIR);
    wall.y = -0.4;
    wall.x = -0.3;
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);
    // player.y = 0.7f;
    //game loop
    while(!glfwWindowShouldClose(window)){
        //get input
        ProcessInput(window, &player);

        //update player
        player.update();
        player.getCollision(wall);
        player.getCollision(ground);
        
        //rendering commands
        clearScreen();
        ground.draw();
        wall.draw();
        player.draw();

        //update the screen
        swapBuffers(window);
    }

    //terminate glfw
    glfwTerminate();
    return 0;
}