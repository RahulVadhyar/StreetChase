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
        AnimationRenderObject* animation_object = nullptr;
        int current_animation_state = 0;
        int num_textures;
        bool shouldRender = true;

        AnimationClass(float input_x, float input_y, float input_width, float input_height){ 
            x = input_x;
            y = input_y;
            width = input_width;
            height = input_height;

            animation_init_debug("Initializing animation states");
            const std::string tex_dir = "Tools/texture_atlas.png";
            animation_init_debug("Animation states added");
            animation_object = new AnimationRenderObject(width, height, tex_dir, 4, 22);
            num_textures = 85;
            animation_init_debug("Animation object created");
        }

    public:
        void draw(){
            if(shouldRender){
                update();
                animation_object->draw();
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
                    if(current_animation_state > num_textures){
                        current_animation_state = 0;
                    }
                    break;
                
            }
            animation_object->flip = 0;
            animation_object->x = x;
            animation_object->y = y;
            animation_object->current_texture = current_animation_state;
            animation_draw_debug("Updating animation. Current state: " + std::to_string((long)current_animation_state) + " X:" +std::to_string(x) + " Y:" + std::to_string(y));
        }

    public:
        virtual ~AnimationClass(){
            delete animation_object;
        }
    
    public:
        void addScreenX(float* input_screen_x){
            screen_x = input_screen_x;
            animation_object->addScreenX(screen_x);
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