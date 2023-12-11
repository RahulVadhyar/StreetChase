#include "chronos.hpp"

namespace Chronos{
    Manager::Manager(Initializer initializer){
        engine.width = initializer.WindowWidth;
        engine.height = initializer.WindowHeight;
        engine.initWindow();
		engine.initVulkan();        
    }
    int Manager::addText(Chronos::Text text){
        
        return 0;
    }
    int Manager::addPolygon(PolygonParameters polygonParameters, PolygonType polygonType, std::string texturePath){
        return 0;
    }
    int Manager::addCallback(void (*callback)(void*), void* args){
        return 0;
    }
    void Manager::removePolygon(int shapeNo){
        
    }
    void Manager::removeText(int textNo){
        
    }
    void Manager::removeCallback(int callbackNo){
        
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
    int Manager::run(){
		engine.mainLoop();
		engine.cleanup();
        return 0;
    }
};