//for resizing the viewport in the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

class Window{
    public:
    GLFWwindow* window;
    PlayerClass *player;
    std::vector<RenderObject*> render_objects;
    float screen_x;
    Window(){
        //initialize glfw
        glfwInit();
        //tell glfw what version of opengl we are using
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //tell glfw we are using core profile
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

        screen_x = 1.0f;

        //debugging: wireframe mode
        #ifdef WIREFRAME_MODE
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        #endif

    }
    void addRenderObject(RenderObject* object){
        render_objects.push_back(object);
        object->addScreenX(&this->screen_x);
    }

    void addPlayer(PlayerClass* player){
        this->player = player;
    }

    void play(){
        while(!glfwWindowShouldClose(window)){
            processInput();
            player->update();
            clear();
            for(auto object : render_objects){
                object->draw();
            }
            swap();
        }
    }
    private:
    void clear(){
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //state setting function
        glClear(GL_COLOR_BUFFER_BIT); //state using function
    }
    void swap(){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    void processInput(){
        InputStatus inputs = {false, false, false, false, false};
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            inputs.up = true;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            inputs.down = true;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            inputs.left = true;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            inputs.right = true;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            inputs.jump = true;
        }
        player->input_status = inputs;
    }
    bool shouldClose(){
        return glfwWindowShouldClose(window);
    }
    public:
    ~Window(){
        glfwTerminate();
    }
};