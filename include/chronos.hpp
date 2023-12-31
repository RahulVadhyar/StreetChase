#pragma once
#include "engine.hpp"
#include "structs.hpp"
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
        int addText(Chronos::Text text);
        int addPolygon(ShapeParams shapeParams, PolygonType polygonType, std::string texturePath);
        int updatePolygon(int shapeNo, ShapeParams shapeParams);
        void removePolygon(int shapeNo);
        void removeText(int textNo);
        int changeBackgroundColor(float r, float g, float b);

    private:
        int nextFreeTextNo = 0;
        std::map<int, Text> textMap;
        Engine engine;
    };
};