#pragma once
#include "engine.hpp"
//this is the api for chronos. Any applications will include and link to this file

namespace Chronos{
    struct Initializer{
        int WindowWidth = 800;
        int WindowHeight = 600;
        int BackgroundColor[3];
    };
    
    struct Text{
        float x = 0.0;
        float y = 0.0;
        std::string text;
        int textNo;
    };
    struct PolygonParameters{
        float x = 0.0;
        float y = 0.0;
        float xSize = 1.0;
        float ySize = 1.0;
        float rotation = 0;
        bool show = true;
        int shapeNo = -1;
    };

    struct PolygonType{
        bool triangle = false;
        bool rectangle = false;
    };

    class Manager{
    public:
        Manager(Initializer initializer);
        GLFWwindow* getWindow();
        void drawFrame();
        int stop();
        // int addText(Chronos::Text text);
        // int addPolygon(PolygonParameters polygonParameters, PolygonType polygonType, std::string texturePath);
        // int addCallback(void (*callback)(void*), void* args);
        // void removePolygon(int shapeNo);
        // void removeText(int textNo);
        // void removeCallback(int callbackNo);
        int changeBackgroundColor(float r, float g, float b);

    private:
        std::vector<void (*)(void*)> callbacks;
        Engine engine;
    };
};