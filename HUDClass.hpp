class HUDObject: public RenderObject{
    public:
    HUDObject(float input_x, float input_y, float input_width, float input_height, Shader *input_shader, std::string texture_dir):
    RenderObject(input_x, input_y, input_width, input_height, input_shader, texture_dir){
    }

    void draw(){
        if(!shouldRender)
            return;
        glm::mat4 trans = Transform_func(x, y);
        shader->use();
        unsigned int transformLoc = glGetUniformLocation(shader->shader_id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));       
        
        //draw triangle
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
            }
};