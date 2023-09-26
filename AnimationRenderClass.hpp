class AnimationRenderObject{
    public:
        unsigned int VBO, VAO, EBO; //for opengl vertices

        //textueres
        unsigned int texture = 0;

        //position and size
        glm::mat4 (*Transform_func)(float, float, float) = nullptr;
        Shader* shader; //shader to use
        float x = 0, y = 0, width, height;
        float rotation = 0; //rotation in radians
        float* screen_x = nullptr;
        int flip = 0;
        int current_texture = 0;
        int num_rows;
        int num_cols;
        //some flags
        bool shouldRender = true;
        bool isClicked = false;
        

        //Constructors
        //if x, y are not provided, else use the other one
        AnimationRenderObject(float input_width, float input_height, std::string texture_dir, int input_num_rows, int input_num_cols)
        {   
            width = input_width;
            height = input_height;
            num_rows = input_num_rows;
            num_cols = input_num_cols;
            float vertices[] = {
                //verticies         texture coords
                width/2,  height/2, 0.0f,  1.0f, 1.0f, 
                width/2, -height/2, 0.0f,  1.0f, 0.0f, 
                -width/2, -height/2, 0.0f,  0.0f, 0.0f, 
                -width/2,  height/2, 0.0f,  0.0f, 1.0f 
                    };
            shader = new Shader(VS_ANIMATION_SHADER_DIR, FS_ANIMATION_SHADER_DIR);
            renderInitDebug("Shader initialized");
            generateVertices(vertices, sizeof(vertices));
            renderInitDebug("Vertices generated");
            attachTexture(texture_dir);
            renderInitDebug("Texture attached");
            Transform_func = &Transform::Default;
            renderInitDebug("Transform function set"); 
        };

    private:
        //generates the vertices and binds them to the VAO(sends to GPU)
        void generateVertices(float vertices[], int vertices_size){
            renderInitDebug("Generating vertices");
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
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
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
            shader->addTextureUniform(1);
            shader->setInt("textures", 0);
            shader->setFloat("num_rows", num_rows);
            shader->setFloat("num_cols", num_cols);
        }

    public:
        //draws the object when called
        void draw(){
            if(shouldRender){
                if(screen_x == nullptr){
                    std::cout << "Screen_x not set" << std::endl;
                    exit(-1);
                }

                glm::mat4 trans = Transform_func(x - *screen_x, y, rotation);
                shader->use();
                unsigned int transformLoc = glGetUniformLocation(shader->shader_id, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));  
                renderDrawDebug("Set transform uniform");     

                shader->setInt("current_texture", current_texture);
                //draw triangle
                shader->use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);
                renderDrawDebug("Bound textures");
                shader->setInt("flip", flip);
                renderDrawDebug("Set flip to values: " + std::to_string(flip));

                shader->use();
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
                renderDrawDebug("Drew object");
            }
        }
    
    public:
        void addScreenX(float* input_screen_x){
            screen_x = input_screen_x;
        }

    private:
        void renderDrawDebug(std::string message){
            #ifdef ANIMATION_RENDER_DRAW_DEBUG
                std::cout<<"[ANIMATION_RENDER_DRAW_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
    
    private:
        void renderInitDebug(std::string message){
            #ifdef ANIMATION_RENDER_INIT_DEBUG
                std::cout<<"[ANIMATION_RENDER_INIT_DEBUG]::[" << this <<"] " << message << std::endl;
            #endif
        }
};