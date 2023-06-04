class LevelEditor: public PlayerClass{
    public: 
    LevelEditor(float width, float height, Shader *o_shader, std::string texture_dir):
    PlayerClass::PlayerClass(width, height, o_shader,texture_dir){};

    void update(){
        velocity_x = 0;
        if(input_status.left) velocity_x = -0.01;
        if(input_status.right) velocity_x = 0.01;
        PhysicsObject::update();
        //move the screen if the player is too far to the right or left
        *screen_x = x;
        printPlayerStatus("Update");
    }
};