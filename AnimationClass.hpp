/*
states
-idle
-walking right
-walking left
-jumping
-falling/lying down

THIS WILL NEED TO BE FRAME RATE INDEPENDENT AT ANY COST(NEED TO IMPLEMENT MULTITHREADING)
-Will likely need to refactor the render pipeline in order for this to work
-Will need to implement frame rate independent physics
-Will need to implement frame rate independent animations

-Will need a way to store the animations
*/
class AnimationClass{

    public:
        int state = 0; //0 is idle, 1 is running right, 2 is running left, 3 is jumping
        float x = 1, y = 0;
        float width = 0, height = 0;
        Shader commonshader = Shader(VS_SHADER_DIR, FS_SHADER_DIR);
        float* screen_x = nullptr;
        std::vector<RenderObject*> animation_states;
        int current_animation_state = 0;
        bool shouldRender = true;

        AnimationClass(float input_x, float input_y, float input_width, float input_height){ 
            x = input_x;
            y = input_y;
            width = input_width;
            height = input_height;
            animation_init_debug("Variables initalized, adding animation states");
            for(int i = 0; i < 10; i++){
                animation_init_debug("Adding animation state characterwalk_0000" + std::to_string(i) + ".png");
                addAnimationState("test/characterwalk_0000" + std::to_string(i) + ".png");
            }
            for(int i = 10; i < 100; i++){
                animation_init_debug("Adding animation state characterwalk_000" + std::to_string(i) + ".png");
                addAnimationState("test/characterwalk_000" + std::to_string(i) + ".png");
            }
            for(int i = 100; i < 232; i++){
                animation_init_debug("Adding animation state characterwalk_00" + std::to_string(i) + ".png");
                addAnimationState("test/characterwalk_00" + std::to_string(i) + ".png");
            }


            animation_init_debug("Animation states added");
        }

    public:
        void addAnimationState(std::string image_dir){
            RenderObject* temp = new RenderObject(x, y, width, height, &commonshader, image_dir);
            temp->screen_x = screen_x;
            animation_states.push_back(temp);
        }

    public:
        void draw(){
            if(shouldRender){
                update();
                animation_states[current_animation_state]->draw();
                animation_draw_debug("Drawing animation" + std::to_string((long)current_animation_state));
            }
        }

    private:
        void update(){
            switch(state){
                case 0:
                    current_animation_state = 0;
                    break;
                case 1:
                    current_animation_state += 1;
                    if(current_animation_state > 231){
                        current_animation_state = 0;
                    }
                    break;
                
            }
            animation_states[current_animation_state]->x = x;
            animation_states[current_animation_state]->y = y;
        }

    public:
        virtual ~AnimationClass(){
            for(auto &animation_state: animation_states){
                delete animation_state;
            }
        }
    
    public:
        void addScreenX(float* input_screen_x){
            screen_x = input_screen_x;
            for(auto &animation_state: animation_states){
                animation_state->screen_x = screen_x;
            }
        }

    private:
        void animation_init_debug(std::string message){
            #ifdef ANIMATION_INIT_DEBUG
            std::cout << "[ANIMATION_INIT_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
    
    private:
        void animation_draw_debug(std::string message){
            #ifdef ANIMATION_DRAW_DEBUG
            std::cout << "[ANIMATION_DRAW_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
};