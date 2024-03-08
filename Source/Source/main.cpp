#include "chronos.hpp"

int main(){

    std::filesystem::path cwdpath("../");
    std::filesystem::current_path(cwdpath);
    Chronos::Manager::Initializer initializer;
    initializer.WindowWidth = 800;
    initializer.WindowHeight = 600;
    initializer.BackgroundColor[0] = 0;
    initializer.BackgroundColor[1] = 0;
    initializer.BackgroundColor[2] = 0;

    Chronos::Manager::Manager manager(initializer);
    
    Chronos::Manager::ShapeParams triangleParams;
    triangleParams.x = 0.5;
    triangleParams.y = 0.5;
    triangleParams.xSize = 0.5;
    triangleParams.ySize = 0.5;
    triangleParams.rotation = 0;
    Chronos::Manager::PolygonType triangleType;
    triangleType.triangle = true;
    int triangle = manager.addPolygon(triangleParams, triangleType, "Assets/texture.jpg");

    Chronos::Manager::ShapeParams rectangleParams;
    rectangleParams.x = -0.5;
    rectangleParams.y = -0.5;
    rectangleParams.xSize = 0.5;
    rectangleParams.ySize = 0.5;
    rectangleParams.rotation = 0;
    Chronos::Manager::PolygonType rectangleType;
    rectangleType.rectangle = true;
    int rectangle = manager.addPolygon(rectangleParams, rectangleType, "Assets/texture.jpg");


    Chronos::Engine::TextParams textParams;
    textParams.x = 0.0f;
    textParams.y = 0.0f;
    textParams.text = "This is a test";
    textParams.rotation = 0;
    textParams.scale = 1.0;

    int textNo = manager.addText(textParams);

    while (!glfwWindowShouldClose(manager.getWindow())) {
        if (glfwGetKey(manager.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(manager.getWindow(), true);
        }
        manager.drawFrame();
    }
    
    return 0;
}