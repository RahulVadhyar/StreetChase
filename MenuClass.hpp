class MenuClass{
    public:
        struct Text{
            std::string text;
            float x;
            float y;
        };
        std::vector<HUDObject*> buttons;
        std::vector<TextClass*> texts;
        std::vector<Text> texts_to_draw;
        bool start_game = false;
        Shader* hudshader;
        TextClass* text_renderer;
        MenuClass(){
            hudshader = new Shader(VS_SHADER_DIR, FS_SHADER_DIR);
            text_renderer = new TextClass();
            addButton(0.0f, 0.5f, "Start Game", -0.07);
            addButton(0.0f, 0.2f, "Options", -0.05);
            addButton(0.0f, -0.1f, "Exit", -0.025);
            addButton(0.0f, -0.4f, "Credits", -0.05);

        }
        void draw(){
            if(buttons.empty()){
                std::cout << "No buttons" << std::endl;
                exit(-1);
            }
            for(int i = 0; i < static_cast<int>(buttons.size()); ++i){
                buttons[i]->draw();
                if(buttons[i]->isClicked){
                    std::cout << "Button clicked"<< buttons[i]->y << std::endl;
                    buttons[i]->isClicked = false;
                    if(i == 0){
                        start_game = true;
                    }
                }
            }
            for(auto text: texts_to_draw){
                text_renderer->draw(text.text.c_str(), screen_status.width*(text.x+1)/2, screen_status.height*(text.y + 1)/2, 1, glm::vec3(1.0, 0.0, 0.0));
            }
        }

        void addButton(float x, float y, std::string text, float offset_x){
            auto button = new HUDObject(x, y, 0.3, 0.1, hudshader, CONTAINER_TEX_DIR);
            buttons.push_back(button);
            texts_to_draw.push_back({text, x + offset_x, y - 0.03});
        }
        
        ~MenuClass(){
            if(!buttons.empty()){
                for(auto button: buttons){
                    delete button;
                }
            }
            delete text_renderer;
            delete hudshader;
        }
};