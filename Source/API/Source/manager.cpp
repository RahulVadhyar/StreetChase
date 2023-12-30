#include "chronos.hpp"

namespace Chronos{
    Manager::Manager(Initializer initializer){
        engine.width = initializer.WindowWidth;
        engine.height = initializer.WindowHeight;
        engine.initWindow();
		engine.initVulkan();        
    }
    GLFWwindow* Manager::getWindow(){
        return engine.window;
    }
    void Manager::drawFrame(){

        engine.drawFrame();
    }   
    int Manager::changeBackgroundColor(float r, float g, float b){
        if(r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1){
            return -1;
        }
        engine.bgColor[0] = r;
        engine.bgColor[1] = g;
        engine.bgColor[2] = b;
        return 0;
    }
    int Manager::stop(){
        vkDeviceWaitIdle(engine.device.device);
        engine.cleanup();
        return 0;
    }

};