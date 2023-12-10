#include "engine.hpp"

//this is the api for chronos. Any applications will include and link to this file

namespace Chronos{
    typedef int Texture;

    struct Initializer{
        int WindowWidth = 800;
        int WindowHeight = 600;
        int BackgroundColor[3];
    };
    struct PolygonParameters{
        float x = 0.0;
        float y = 0.0;
        float xSize = 1.0;
        float ySize = 1.0;
        float rotation = 0;
        bool show = true;
        bool triangle = false;
        bool rectangle = false;
    };
    struct Text{
        float x = 0.0;
        float y = 0.0;
        std::string texture;
        int textNo;
    };

    class Scene{
    public:
        int sceneNo;
        bool active = true;
        int createTexture(Texture* texture, std::string path);
        int removeTexture(Texture* texture);
        int addShape(PolygonParameters* polygon, Texture texture);
        int removeShape(int shapeNo);
        int addText(Text* text);
        int removeText(Text* text);
        int addCallback(void (*callback)(void* args), void* args);
        int removeCallback(int callbackNo);
    private:
        std::vector<PolygonParameters*> polygons;
        std::vector<int> shapes;
        std::vector<Text*> texts;
        std::vector<int> callbacks;   
    };
    
    class Manager{
    public:
        Manager(Initializer initializer);
        int addScene(Scene* scene);
        int removeScene(Scene* scene);
        int changeBackgroundColor(int r, int g, int b);
        int run();

    private:
        Engine engine;
        std::vector<Scene*> scenes;  
    };
};