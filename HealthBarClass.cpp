class HealthBarClass{
    public:
        PlayerClass *player;
        float width = 0.1;
        float height = 0.01;
    HealthBarClass(PlayerClass *input_player){
        player = input_player;
        float vertices[] = {
            //verticies         texture coords
            width/2,  height/2, 0.0f,  1.0f, 1.0f, 
            width/2, -height/2, 0.0f,  1.0f, 0.0f, 
            -width/2, -height/2, 0.0f,  0.0f, 0.0f, 
            -width/2,  height/2, 0.0f,  0.0f, 1.0f 
                };
    }

};