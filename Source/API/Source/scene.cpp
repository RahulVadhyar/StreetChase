#include "chronos.hpp"

namespace Chronos{
    int Scene::createTexture(Texture* texture, std::string path){
        return 0;
    }
    int Scene::removeTexture(Texture* texture){
        return 0;
    }
    int Scene::addShape(PolygonParameters* polygon, Texture texture){
        return 0;
    }
    int Scene::removeShape(int shapeNo){
        return 0;
    }
    int Scene::addText(Text* text){
        return 0;
    }
    int Scene::removeText(Text* text){
        return 0;
    }
    int Scene::addCallback(void (*callback)(void* args), void* args){
        return 0;
    }
    int Scene::removeCallback(int callbackNo){
        return 0;
    }
};