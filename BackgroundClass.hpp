class BackgroundObject: public RenderObject{
    public:

        float scale = 1;

        BackgroundObject(float input_x, float input_y, float input_width, float input_height, float input_scale, Shader *input_shader, std::string base_texture_dir):
        RenderObject(input_x, input_y, input_width, input_height, input_shader, base_texture_dir){
            this->scale = input_scale;
        }

    public:
        void draw(){
            //same as render object, just scaling screen_X
            if(shouldRender){
                if(screen_x == nullptr){
                    std::cout << "Screen_x not set" << std::endl;
                    exit(-1);
                }
                if(textures.empty()){
                    std::cout << "No textures attached" << std::endl;
                    exit(-1);
                }
                float x_pos = x - (*screen_x)*scale;
                glm::mat4 trans = Transform_func(x_pos, y, rotation);
                shader->use();
                unsigned int transformLoc = glGetUniformLocation(shader->shader_id, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));  
                BackgroundDebug("Set scaled transform uniform, the scale is: " + std::to_string(scale));     
                justDraw();
            }
        }
        
    private:
        void BackgroundDebug(std::string message){
            #ifdef BACKGROUND_DEBUG
            std::cout << "[BACKGROUND_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }

};