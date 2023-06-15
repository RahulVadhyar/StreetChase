//This is the menu class which fully handles input processing and rendering of the menu
class MenuClass{
    public:
        //Storing text info
        struct Text{
            std::string text;
            float x;
            float y;
        };

        //Storing menu objects
        struct MenuObjects{
            std::vector<HUDObject*> buttons;
            std::vector<Text> texts_to_draw;
            int menu_no;
        };
        
        //create main menu and options menu, and a pointer to the current menu
        MenuObjects main_menu;
        MenuObjects options_menu;
        MenuObjects* current_menu;

        //create the bools for start game and close window
        bool start_game = false;
        bool close_window = false;
        bool isClicked = false;

        //create the shader and text renderer
        Shader* hudshader;
        TextClass* text_renderer;
        
        MenuClass(){
            //create the needed shaders(for HUDObject) and text renderer
            hudshader = new Shader(VS_SHADER_DIR, FS_SHADER_DIR);
            menuAllocDeallocDebug("Created hud shader: " + std::to_string((long)hudshader));
            text_renderer = new TextClass();
            menuAllocDeallocDebug("Created text renderer: " + std::to_string((long)text_renderer));

            //create the main menu and options menu

            //main menu
            main_menu.menu_no = 0;
            addButton(0.0f, 0.5f, "Start Game", -0.07, &main_menu);
            addButton(0.0f, 0.2f, "Options", -0.05, &main_menu);
            addButton(0.0f, -0.4f, "Credits", -0.05, &main_menu);
            addButton(0.0f, -0.1f, "Exit", -0.025, &main_menu);

            //options menu
            options_menu.menu_no = 1;
            addButton(0.0f, 0.5f, "Level editor", -0.07, &options_menu);
            addButton(0.0f, 0.2f, "Quality", -0.05, &options_menu);
            addButton(0.0f, -0.4f, "Back", -0.05, &options_menu);
            addButton(0.0f, -0.1f, "FPS", -0.025, &options_menu);
            current_menu = &main_menu;
        }

    public:
        //draw call
        void draw(){
            //there should be buttons in every menu, else nuke the program.
            if(current_menu->buttons.empty()){
                std::cout << "No buttons" << std::endl;
                exit(-1);
            }
            //cycle through every button in the current menu and draw it along with handling the click event
            for(int i = 0; i < static_cast<int>(current_menu->buttons.size()); ++i){
                current_menu->buttons[i]->draw(); //drawing
                if(current_menu->buttons[i]->isClicked){ //click event handling
                    current_menu->buttons[i]->isClicked = false;
                    switch(current_menu->menu_no){ //which menu are we on?
                        case 0: //main menu
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
                        case 1: //options menu
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
            //finally render the text for each of the buttons
            menuDrawDebug("Rendering text: ");
            for(auto text: current_menu->texts_to_draw){
                auto x = screen_status.width*(text.x+1)/2;
                auto y = screen_status.height*(text.y+1)/2;
                text_renderer->draw(text.text.c_str(), x, y, 1, glm::vec3(1.0, 0.0, 0.0));
                menuDrawDebug("Text: " + text.text + " x: " + std::to_string(x) + " y: " + std::to_string(y));
            }
            MenuDebug();
        }

    public:
        //create each of the menu buttons as a HUDobject
        void addButton(float x, float y, std::string text, float offset_x, MenuObjects* menu){
            auto button = new HUDObject(x, y, 0.3, 0.1, hudshader, CONTAINER_TEX_DIR);
            menuAllocDeallocDebug("Created button: " + std::to_string((long)button));
            menu->buttons.push_back(button);
            menu->texts_to_draw.push_back({text, x + offset_x, y - static_cast<float>(0.03)});
        }
    
    public:
        //need to deallocate all the memory that we manually allocated
        ~MenuClass(){
            if(!main_menu.buttons.empty()){
                for(auto button: main_menu.buttons){
                    delete button;
                    menuAllocDeallocDebug("Deleted Main menu button: " + std::to_string((long)button));
                }
            }
            if(!options_menu.buttons.empty()){
                for(auto button: options_menu.buttons){
                    delete button;
                    menuAllocDeallocDebug("Deleted Options menu button: " + std::to_string((long)button));
                }
            }
            delete text_renderer;
            menuAllocDeallocDebug("Deleted text renderer");
            delete hudshader;
            menuAllocDeallocDebug("Deleted hudshader");
        }

    private:
        void menuDrawDebug(std::string message){
            #ifdef MENU_DRAW_DEBUG
            std::cout << "[MENU_DRAW_DEBUG]: " << message << std::endl;
            #endif
        }

    private:
        void menuAllocDeallocDebug(std::string message){
            #ifdef MENU_ALLOC_DEALLOC_DEBUG
            std::cout << "[MENU_ALLOC_DEALLOC_DEBUG]: " << message << std::endl;
            #endif
        }

    private:
        void MenuDebug(){
            #ifdef MENU_DRAW_DEBUG
            std::cout << "[MENU_DRAW_DEBUG]: Menu debug" << std::endl;
            std::cout << "[MENU_DRAW_DEBUG]: Current menu: " << current_menu->menu_no << std::endl;
            std::cout << "[MENU_DRAW_DEBUG]: Texts: " << std::endl;
            for(auto text: current_menu->texts_to_draw){
                std::cout << text.text << " " << text.x << " " << text.y << std::endl;
            
            }
            #endif
        }
};