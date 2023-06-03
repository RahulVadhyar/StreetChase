#include </home/rahul/StreetChase/definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main

int main(){
    //Initalize OpenGl with a window
    GLFWwindow* window = Initalize();

    //create a shader
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");

    //create ground and player
    RenderObject ground(2.0f, 0.5f, &shader1, STONE_TEX_DIR);
    ground.y = -0.75f;
    RenderObject wall(0.1, 0.2, &shader1, STONE_TEX_DIR);
    wall.y = -0.2;
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);

    //game loop
    while(!glfwWindowShouldClose(window)){
        //get input
        ProcessInput(window, &player);

        //update player
        player.update();
        player.update_coords();
        player.collision(ground);
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