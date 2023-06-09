class RenderObject{
    public:
        //opengl variables
        std::vector<unsigned int> textures;
        unsigned int VBO, VAO, EBO;
        glm::mat4 (*Transform_func)(float, float) = nullptr;
        Shader* shader;
        float x = 0, y = 0, width, height;
        float prev_x = 0, prev_y = 0;
        float* screen_x = nullptr;
        bool shouldRender = true;
        float mixValue[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float TexScale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        //Constructors
        //if x, y are not provided, else use the other one
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
            attachTexture(texture_dir);
            Transform_func = &Transform::Default; 
        };

        RenderObject(float input_x, float input_y, float input_width, float input_height, Shader *input_shader, std::string texture_dir)
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
            attachTexture(texture_dir);
            Transform_func = &Transform::Default; 
            x = input_x; y= input_y;
        };


        private:
        //generates the vertices and binds them to the VAO(sends to GPU)
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
        public:
        //attaches the texture to the object
        void attachTexture(std::string image_dir){
            //texture1
            shader->use();
            textures.push_back(0);
            glGenTextures(1, &textures.back());
            glBindTexture(GL_TEXTURE_2D, textures.back());
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
                    std::cout << "Unsupported number of channels: " << nrChannels << "for "<< image_dir << std::endl;
                    exit(-1);
                }
            glGenerateMipmap(GL_TEXTURE_2D);   
            } else {
                std::cout << "Failed to load texture" << std::endl;
                exit(-1);
            }
            stbi_image_free(data);
            shader->addTextureUniform(textures.size());
        }
        void removeLastTexture(){
            if(!textures.empty()){
            glDeleteTextures(1, &textures.back());
            textures.pop_back();
            } else {
                std::cout << "No textures to remove" << std::endl;
                exit(-1);
            }
        }
        public:
        //draws the object when called
        virtual void draw(){
            if(shouldRender){
                if(screen_x == nullptr){
                    std::cout << "Screen_x not set" << std::endl;
                    exit(-1);
                }
                if(textures.empty()){
                    std::cout << "No textures attached" << std::endl;
                    exit(-1);
                }
                Transform_func(x - *screen_x, y);
                glm::mat4 trans = Transform_func(x - *screen_x, y);
                shader->use();
                unsigned int transformLoc = glGetUniformLocation(shader->shader_id, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));       
                justDraw();
            }
        }

        public:
        void justDraw(){
            shader->setFloat("mixValue2", mixValue[0]);
            shader->setFloat("mixValue3", mixValue[1]);
            shader->setFloat("mixValue4", mixValue[2]);
            shader->setFloat("mixValue5", mixValue[3]);

            shader->setFloat("TexScale2", TexScale[0]);
            shader->setFloat("TexScale3", TexScale[1]);
            shader->setFloat("TexScale4", TexScale[2]);
            shader->setFloat("TexScale5", TexScale[3]);
            //draw triangle
            for(int i = 0; i < static_cast<int>(textures.size()); i++){
                shader->use();
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i]);
            }

            shader->use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
            
        }
        virtual void mouseClick(){
            std::cout << "Clicked" << std::endl;
        }
        void addScreenX(float* input_screen_x){
            screen_x = input_screen_x;
        }

};