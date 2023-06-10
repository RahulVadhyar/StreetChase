class HealthBarClass{
    public:
        PlayerClass *player;
        float width = 0.1;
        float height = 0.01;
        Shader shader = Shader(VS_HEALTHBAR_SHADER_DIR, FS_HEALTHBAR_SHADER_DIR);
        unsigned int VBO, VAO, EBO;
        bool shouldRender = true;
        glm::mat4(*Transform_func)(float, float) = Transform::Default;
        float offset_y = 0.13;
    HealthBarClass(PlayerClass *input_player){
        player = input_player;
        float vertices[] = {
            //verticies         texture coords
            width/2,  height/2, 
            width/2, -height/2, 
            -width/2, -height/2, 
            -width/2,  height/2,
                };
        generateVertices(vertices, sizeof(vertices));
    }

    void generateVertices(float vertices[], int vertices_size){
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

    void draw(){
        if(shouldRender){
            shader.use();
            shader.setFloat("health_percent", 0.6);

            glm::mat4 trans = Transform_func(player->x - *player->screen_x, player->y + offset_y);
            
            shader.use();
            unsigned int transformLoc = glGetUniformLocation(shader.shader_id, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));      
            shader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    }
};