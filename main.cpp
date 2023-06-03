#include </home/rahul/StreetChase/definitions.hpp>
//command to compile: g++ main.cpp glad/glad.c -lglfw -lGL -Iglm -o main &&./main

int main(){
    //Initalize OpenGl with a window
    GLFWwindow* window = Initalize();
    Shader shader1("/home/rahul/StreetChase/Shaders/shader.vs", "/home/rahul/StreetChase/Shaders/shader.fs");
    RenderObject ground(2.0f, 0.5f, &shader1, STONE_TEX_DIR);
    ground.y = -0.75f;
    PlayerClass player(0.1f, 0.2f, &shader1, PLAYER_TEX_DIR);

    //wireframe mode
    #ifdef WIREFRAME_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif

    while(!glfwWindowShouldClose(window)){
        
        ProcessInput(window, &player);
        player.update();
        player.collision(ground);
        player.update_coords();
        //rendering commands
        clearScreen();
        ground.draw();
        player.draw();
        swapBuffers(window);
    }
    
    
    //terminate glfw
    glfwTerminate();
    return 0;
}