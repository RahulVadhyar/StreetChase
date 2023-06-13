class MenuClass{
    public:
        struct Text{
            std::string text;
            float x;
            float y;
        };
        struct MenuObjects{
            std::vector<HUDObject*> buttons;
            std::vector<Text> texts_to_draw;
            int menu_no;
        };
        
        MenuObjects main_menu;
        MenuObjects options_menu;
        MenuObjects* current_menu;
        bool start_game = false;
        bool close_window = false;
        Shader* hudshader;
        TextClass* text_renderer;
        bool isClicked = false;
        MenuClass(){
            main_menu.menu_no = 0;
            options_menu.menu_no = 1;

            hudshader = new Shader(VS_SHADER_DIR, FS_SHADER_DIR);
            text_renderer = new TextClass();
            addButton(0.0f, 0.5f, "Start Game", -0.07, &main_menu);
            addButton(0.0f, 0.2f, "Options", -0.05, &main_menu);
            addButton(0.0f, -0.4f, "Credits", -0.05, &main_menu);
            addButton(0.0f, -0.1f, "Exit", -0.025, &main_menu);

            addButton(0.0f, 0.5f, "Level editor", -0.07, &options_menu);
            addButton(0.0f, 0.2f, "Quality", -0.05, &options_menu);
            addButton(0.0f, -0.4f, "Back", -0.05, &options_menu);
            addButton(0.0f, -0.1f, "FPS", -0.025, &options_menu);
            current_menu = &main_menu;

        }

        void draw(){
            if(current_menu->buttons.empty()){
                std::cout << "No buttons" << std::endl;
                exit(-1);
            }
            for(int i = 0; i < static_cast<int>(current_menu->buttons.size()); ++i){
                current_menu->buttons[i]->draw();
                if(current_menu->buttons[i]->isClicked){
                    current_menu->buttons[i]->isClicked = false;
                    switch(current_menu->menu_no){
                        case 0:
                            switch (i){
                                case 0:
                                    start_game = true;
                                    break;
                                case 1:
                                    current_menu = &options_menu;
                                    break;
                                case 2:
                                    std::cout << "Credits" << std::endl;
                                    break;
                                case 3:
                                    close_window = true;
                                    break;
                                default:
                                    std::cout << "No such button" << std::endl;
                                    exit(-1);
                                }
                            break;
                        case 1:
                            switch(i){
                                case 0:
                                    std::cout << "Level editor" << std::endl;
                                    break;
                                case 1:
                                    std::cout << "Quality" << std::endl;
                                    break;
                                case 2:
                                    current_menu = &main_menu;
                                    break;
                                case 3:
                                    std::cout << "FPS" << std::endl;
                                    break;
                                default:
                                    std::cout << "No such button" << std::endl;
                                    exit(-1);
                                }
                            break;
                        default:
                            std::cout << "No such menu" << std::endl;
                            exit(-1);
                    }
                    
                }
            }
            for(auto text: current_menu->texts_to_draw){
                text_renderer->draw(text.text.c_str(), screen_status.width*(text.x+1)/2, screen_status.height*(text.y + 1)/2, 1, glm::vec3(1.0, 0.0, 0.0));
            }
        }

        void addButton(float x, float y, std::string text, float offset_x, MenuObjects* menu){
            auto button = new HUDObject(x, y, 0.3, 0.1, hudshader, CONTAINER_TEX_DIR);
            menu->buttons.push_back(button);
            menu->texts_to_draw.push_back({text, x + offset_x, y - static_cast<float>(0.03)});
        }
        
        ~MenuClass(){
            if(!main_menu.buttons.empty()){
                for(auto button: main_menu.buttons){
                    delete button;
                }
            }
            if(!options_menu.buttons.empty()){
                for(auto button: options_menu.buttons){
                    delete button;
                }
            }
            delete text_renderer;
            delete hudshader;
        }
};