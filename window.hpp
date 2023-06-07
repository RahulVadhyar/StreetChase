struct ScreenStatus{
    float height;
    float width;
};
ScreenStatus screen_status = {SCREEN_HEIGHT, SCREEN_WIDTH};

//for resizing the viewport in the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    screen_status.height = height;
    screen_status.width = width;
}

class Window{
    public:
    GLFWwindow* window;
    PlayerClass *player;
    std::vector<RenderObject*> render_objects;
    std::vector<RenderObject*> mouse_callback_objects;

    float* screen_x;
    float prev_time;
    Window(){
        //initialize glfw
        glfwInit();
        //tell glfw what version of opengl we are using
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //tell glfw we are using core profile
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
        window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Street Chase", NULL, NULL);
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
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        //tell glfw to call framebuffer_size_callback on window resize
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        screen_x = new float(1.0f);
        prev_time = glfwGetTime();
        //debugging: wireframe mode
        #ifdef WIREFRAME_MODE
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        #endif

    }

    void addRenderObject(RenderObject* object){
        render_objects.push_back(object);
        object->addScreenX(this->screen_x);
    }

    void addPlayer(PlayerClass* player){
        this->player = player;
    }

    void play(){
        while(!glfwWindowShouldClose(window)){
            show_fps();
            clear();
            processInput();
            player->update();
            for(auto object : render_objects){
                object->draw();
            }
            swap();
        }
    }
    void addMouseCallbackObject(RenderObject* object){
        mouse_callback_objects.push_back(object);
    }
    private:
    void clear(){
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //state setting function
        glClear(GL_COLOR_BUFFER_BIT); //state using function
    }
    void swap(){
        // glfwSwapBuffers(window);
        glFlush();
        glfwPollEvents();
    }

    void processInput(){
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
            for(int i = 0; i < mouse_callback_objects.size(); ++i){
                auto object = mouse_callback_objects[i];             
                if(object->x + object->width/2 > pos_x && object->x - object->width/2 < pos_x && object->y + object->height/2 > pos_y && object->y - object->height/2 < pos_y)
                    object->mouseClick();
            }
        }
    }
    bool shouldClose(){
        return glfwWindowShouldClose(window);
    }
    void show_fps(){
        #ifdef SHOW_FPS
        float current_time = glfwGetTime();
        std::cout << "FPS: " << 1.0f/(current_time - prev_time) << std::endl;
        prev_time = current_time;
        #endif
    }
    void show_screen_x(){
        #ifdef SHOW_SCREEN_X
        std::cout << "Screen X: " << *screen_x << std::endl;
        #endif
    }
    public:
    ~Window(){
        glfwTerminate();
        delete this->screen_x;
    }   
};