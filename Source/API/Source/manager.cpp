#include "chronos.hpp"
#include "text.hpp"
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
        return 1;
    }

    int Manager::addPolygon(ShapeParams shapeParams, PolygonType polygonType, std::string texturePath){
        if(polygonType.triangle){
            return engine.shapeManager.addTriangle(shapeParams, texturePath);
        }
        else if(polygonType.rectangle){
            return engine.shapeManager.addRectangle(shapeParams, texturePath);
        }
        return 0;
    }
    int Manager::updatePolygon(int shapeNo, ShapeParams shapeParams){
        if(engine.shapeManager.shapes.count(shapeNo) == 0){
            return -1;
        }
        engine.shapeManager.shapes[shapeNo].params = shapeParams;
        return 0;
    }
    void Manager::removePolygon(int shapeNo){
        if(engine.shapeManager.shapes.count(shapeNo) > 0){
            engine.shapeManager.removeShape(shapeNo);
        }
    }
    int Manager::addText(Chronos::Text text){
        int textNo = nextFreeTextNo;
        nextFreeTextNo++;
        textMap[textNo] = text;
        engine.textManager.beginUpdate();
        for(auto& text : textMap){
            engine.textManager.add(text.second.text, text.second.x, text.second.y, Center);
        }
        engine.textManager.endUpdate();
        return textNo;

    }
    void Manager::removeText(int textNo){
        if(textMap.count(textNo) > 0){
            textMap.erase(textNo);
        }
        engine.textManager.beginUpdate();
        for(auto& text : textMap){
            engine.textManager.add(text.second.text, text.second.x, text.second.y, Center);
        }
        engine.textManager.endUpdate();
    }
};