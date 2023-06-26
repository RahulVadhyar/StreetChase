class HealthBarClass{
    public:
        PersonClass *person;
        float width = 0.1;
        float height = 0.01;
        Shader shader = Shader(VS_HEALTHBAR_SHADER_DIR, FS_HEALTHBAR_SHADER_DIR); //custom shaders for the health bar
        unsigned int VBO, VAO, EBO;
        bool shouldRender = true;
        glm::mat4(*Transform_func)(float, float, float) = Transform::Default;
        float offset_y = 0.13;

        HealthBarClass(PersonClass *input_player){
            person = input_player;
            float vertices[] = {
                //verticies 
                width/2,  height/2, 
                width/2, -height/2, 
                -width/2, -height/2, 
                -width/2,  height/2,
                    };
            generateVertices(vertices, sizeof(vertices));
            debug("Generated vertices");
        }
    private:
        void generateVertices(float vertices[], int vertices_size){
            //generate the vertices for the health bar(it is just a rectangle)
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

            unsigned int indices[] = {
                    0, 1, 3,
                    1, 2, 3};

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0); 
        }

    public:
        void draw(){
            //draw the health bar
            if(shouldRender){
                //set the health percent uniform
                shader.use();
                shader.setFloat("health_percent", person->current_health);
                debug("Set health_percent uniform");

                //set the transform uniform
                glm::mat4 trans = Transform_func(person->AnimationClass::x - *person->screen_x, person->AnimationClass::y + offset_y, 0);
                shader.use();
                unsigned int transformLoc = glGetUniformLocation(shader.shader_id, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));      
                debug("Set transform uniform");

                //draw the health bar
                shader.use();
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                debug("Drew healthbar");
            }
        }

    private:
        void debug(std::string message){
            #ifdef HEALTHBAR_DEBUG
                std::cout<<"[HEALTHBAR_DEBUG]::[" << person <<"] " << message << std::endl;
            #endif
        }
};