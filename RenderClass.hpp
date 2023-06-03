class RenderObject{
    public:
        unsigned int texture1, texture2;
        unsigned int VBO, VAO, EBO;
        glm::mat4 (*Transform_func)(float, float);
        Shader* shader;
        float x, y, width, height;
        RenderObject(float input_width, float input_height, Shader *input_shader, std::string texture_dir)
        {   
            width = input_width;
            height = input_height;
             float vertices[] = {
                //verticies         texture coords
                width/2,  height/2, 0.0f,  1.0f, 1.0f, 
                width/2, -height/2, 0.0f,  1.0f, 0.0f, 
                -width/2, -height/2, 0.0f,  0.0f, 0.0f, 
                -width/2,  height/2, 0.0f,  0.0f, 1.0f 
                    };
            shader = input_shader;
            generateVertices(vertices, sizeof(vertices));
            shader->use();
            attachTexture(texture_dir);
            Transform_func = &Transform::Default; 
            x = 0; y= 0;
        };

        private:
        void generateVertices(float vertices[], int vertices_size){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

            unsigned int indices[] = {
                    0, 1, 3,
                    1, 2, 3
                            };

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0); 
        }
        private:
        void attachTexture(std::string image_dir){
            //texture1
            glGenTextures(1, &texture1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            //texture wrapping and filtering options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //load the texture
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *data = stbi_load(image_dir.c_str(), &width, &height, &nrChannels, 0);
            if(data){
                if(nrChannels == 3)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                else if(nrChannels == 4)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                else{
                    std::cout << "Unsupported number of channels" << std::endl;
                    exit(-1);
                }
            glGenerateMipmap(GL_TEXTURE_2D);   
            } else {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
        }
        public:
        void draw(){
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