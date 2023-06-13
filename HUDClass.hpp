class HUDObject: public RenderObject{
    public:
    HUDObject(float input_x, float input_y, float input_width, float input_height, Shader *input_shader, std::string base_texture_dir):
    RenderObject(input_x, input_y, input_width, input_height, input_shader, base_texture_dir){
    }

    void draw(){
        if(shouldRender){
            if(textures.empty()){
                std::cout << "No textures attached" << std::endl;
                exit(-1);
            }
            Transform_func(x, y);
            glm::mat4 trans = Transform_func(x , y);
            shader->use();
            unsigned int transformLoc = glGetUniformLocation(shader->shader_id, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));       
            justDraw();
        }
        
    }

};