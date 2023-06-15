//for resizing the viewport in the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    //update the screen status(defined in definitions.hpp)
    screen_status.height = height;
    screen_status.width = width;
    #ifdef VIEWPORT_SIZE_DEBUG
    std::cout << "[VIEWPORT_SIZE_DEBUG]: Viewport size changed to x: " << width << " y: " << height << std::endl;
    #endif
}

//Handles window, input, rendering and collisions
class Window{
    public:
        GLFWwindow* window = nullptr;
        PlayerClass *player = nullptr;

        std::vector<RenderObject*> render_objects;
        std::vector<RenderObject*> mouse_callback_objects;
        std::vector<PersonClass*> persons;

        float* screen_x; //position of screen in world
        float prev_time;

        Window(){
            //initialize glfw
            glfwInit();
            windowInitDebug("GLFW initialized");

            //tell glfw what version of opengl we are using
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            windowInitDebug("GLFW version set to 3.3");
            
            //tell glfw we are using core profile
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            windowInitDebug("GLFW profile set to core");

            //if single buffer is defined, use single buffer
            #ifdef SINGLE_BUFFER
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
            #endif
            
            //create the window
            window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Street Chase", NULL, NULL);
            windowInitDebug("GLFW window created");

            if(window == NULL){
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(-1); }

            //make the window the current context
            glfwMakeContextCurrent(window);

            //initialize glad
            if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
                std::cout << "Failed to initialize GLAD" << std::endl; 
                exit(-1);
            }
            windowInitDebug("GLAD initialized");

            //this is for text rendering
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            //set the viewport
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            windowInitDebug("Viewport set to x: " + std::to_string(SCREEN_WIDTH) + " y: " + std::to_string(SCREEN_HEIGHT));

            //tell glfw to call framebuffer_size_callback on window resize
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            windowInitDebug("Callbacks and input mode set");

            //initialize screen_x and prev time
            screen_x = new float(1.0f);
            prev_time = glfwGetTime();

            //debugging: wireframe mode
            #ifdef WIREFRAME_MODE
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            #endif
        }

    public:
        void addRenderObject(RenderObject* object){
            render_objects.push_back(object);
            object->addScreenX(this->screen_x);
            windowVectorStatus("Added render object: " + std::to_string((long)object));
        }

    public:
        void addPlayer(PlayerClass* player){
            this->player = player;
            windowVectorStatus("Added player: " + std::to_string((long)player));
        }

    public:
        void addPerson(PersonClass* person){
            persons.push_back(person);
            person->addScreenX(this->screen_x);
            windowVectorStatus("Added person: " + std::to_string((long)person));
        }

    //Menu loop
    public:
        void mainMenu(){
            auto menu = MenuClass();
            while(!glfwWindowShouldClose(window)){
                show_fps();
                clear(0.58,0.58,0.60);
                processMenuInput(&menu);
                menu.draw();
                swap();
                //if menu says to start the game, quit the menu loop
                if(menu.start_game){
                    break;
                }   
                //if menu says to close window, close the window
                if(menu.close_window){
                    glfwSetWindowShouldClose(window, true);
                }
            }
        }

    //same as processInput but for menu
    private:
        void processMenuInput(MenuClass* menu){
            InputStatus inputs;
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                glfwSetWindowShouldClose(window, true);
            }
            inputs.up = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
            inputs.down = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
            inputs.left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
            inputs.right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
            inputs.jump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
            glfwGetCursorPos(window, &inputs.mouse_x, &inputs.mouse_y);
            inputs.left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            inputs.right_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
            if(inputs.left_click){
                if(!menu->isClicked){
                float pos_x = (inputs.mouse_x/screen_status.width)*2 - 1.0f;
                float pos_y = -(inputs.mouse_y/screen_status.height)*2 + 1.0f;
                    if(!menu->current_menu->buttons.empty()){
                        for(int i = 0; i < static_cast<int>(menu->current_menu->buttons.size()); ++i){
                            auto object = menu->current_menu->buttons[i];           
                            if(object->x + object->width/2 > pos_x && object->x - object->width/2 < pos_x && object->y + object->height/2 > pos_y && object->y - object->height/2 < pos_y){
                                object->isClicked = true;
                            }
                        }
                    }
                }
                menu->isClicked = true;
            } else {
                menu->isClicked = false;
            }
        }

    public:
        void play(){
            while(!glfwWindowShouldClose(window)){
                show_fps();
                clear(1.0, 1.0, 1.0);
                windowRenderDebug("Cleared screen");
                processInput();
                windowRenderDebug("Processed input");
                if(player != nullptr){
                    player->update();
                    windowRenderDebug("Updated player");
                } else {
                    std::cout << "No player" << std::endl;
                    exit(-1);
                }
                for(auto object : render_objects){
                    if(object != nullptr){
                        object->draw();
                        windowRenderDebug("Drew object: " + std::to_string((long)object));
                    }
                }

                for(auto &object : persons){
                    if(object != nullptr){
                        //if person is dead, remove him from the game
                        if(object->is_dead){
                            delete object;
                            windowRenderDebug("Deleting object: " + std::to_string((long)object));
                            for(auto object2: persons){
                                if(object2 != nullptr){
                                    object2->removeCollisionObject(object);
                                    windowRenderDebug("Removing object: " + std::to_string((long)object) + " from object: " + std::to_string((long)object2));
                                }
                            }
                            player->removeCollisionObject(object);
                            windowRenderDebug("Removing object: " + std::to_string((long)object) + " from player");
                            object = nullptr;
                        } else{
                            object->update();
                            object->draw();
                            windowRenderDebug("Updated and drew object: " + std::to_string((long)object));
                        }
                    }
                }
                swap();
                windowRenderDebug("Swapped buffers");
            }
        }
    
    public:
        void addMouseCallbackObject(RenderObject* object){
            mouse_callback_objects.push_back(object);
        }

    private:
        void clear(float r, float g, float b){
            glClearColor(r, g, b, 0.0f); //state setting function
            glClear(GL_COLOR_BUFFER_BIT); //state using function
        }
    
    private:
        void swap(){
            glfwSwapBuffers(window);
            #ifdef SINGLE_BUFFER
            glFlush();
            #endif
            glfwPollEvents();
        }

    private:
        void processInput(){
            //store inputs from opengl in the struct and call the necessary callbacks and give struct to player
            InputStatus inputs;
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                glfwSetWindowShouldClose(window, true);
            }
            inputs.up = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
            inputs.down = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
            inputs.left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
            inputs.right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
            inputs.jump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
            glfwGetCursorPos(window, &inputs.mouse_x, &inputs.mouse_y);
            inputs.left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            inputs.right_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
            player->input_status = inputs;
            if(inputs.left_click){
            float pos_x = (inputs.mouse_x/screen_status.width)*2 - 1.0f;
            float pos_y = -(inputs.mouse_y/screen_status.height)*2 + 1.0f;
                if(!mouse_callback_objects.empty()){
                    for(int i = 0; i < static_cast<int>(mouse_callback_objects.size()); ++i){
                        auto object = mouse_callback_objects[i];             
                        if(object->x + object->width/2 > pos_x && object->x - object->width/2 < pos_x && object->y + object->height/2 > pos_y && object->y - object->height/2 < pos_y)
                            object->isClicked = true;
                        else
                            object->isClicked = false;
                    }
                }
            }
        }
    
    private:    
        bool shouldClose(){
            return glfwWindowShouldClose(window);
        }

    public:
        ~Window(){
            glfwTerminate();
            delete this->screen_x;
        }   
    
    private:
        void show_fps(){
            #ifdef SHOW_FPS
            float current_time = glfwGetTime();
            std::cout << "FPS: " << 1.0f/(current_time - prev_time) << std::endl;
            prev_time = current_time;
            #endif
        }
    
    private:
        void show_screen_x(){
            #ifdef SHOW_SCREEN_X
            std::cout << "Screen X: " << *screen_x << std::endl;
            #endif
        }

    private:
        void windowInitDebug(std::string message){
            #ifdef WINDOW_INIT_DEBUG
            std::cout << "[WINDOW_INIT_DEBUG]: " << message << std::endl;
            #endif
        }

    private:
        void windowVectorStatus(std::string message){
            #ifdef WINDOW_VECTOR_STATUS
            std::cout << "[WINDOW_VECTOR_STATUS]: " << message << std::endl;
            #endif
        }

    private:
        void windowRenderDebug(std::string message){
            #ifdef WINDOW_RENDER_DEBUG
            std::cout << "[WINDOW_RENDER_DEBUG]: " << message << std::endl;
            #endif
        }
};