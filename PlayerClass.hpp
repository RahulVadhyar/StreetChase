struct InputStatus{
    bool up;
    bool down;
    bool left;
    bool right;
    bool jump;
};


class PlayerClass: public PhysicsObject{
    public:
    InputStatus input_status;
    PlayerClass(float width, float height, Shader *o_shader, std::string texture_dir) : 
    PhysicsObject::PhysicsObject(width, height, o_shader, texture_dir){
    input_status = {false, false, false, false, false};
    default_y = -0.5f + (height/2);
    y = default_y;
    }

    void update(){
        
        velocity_x = 0;
        velocity_y = 0;
        if(input_status.up) velocity_y = 0.01;
        if(input_status.down) velocity_y = -0.01;
        if(input_status.left) velocity_x = -0.01;
        if(input_status.right) velocity_x = 0.01;
        if(input_status.jump && velocity_y == 0) velocity_y = 0.1;

        #ifdef INPUT_STATUS
            printKeystrokes();
        #endif
        #ifdef PRINT_PLAYER_STATUS
        std::cout << "Status player.update():" << std::endl;
        printPlayerStatus();
        #endif
    }

    void printKeystrokes(){
        std::cout << "DEBUG: Input Status" << std::endl;
        std::cout << "Up(W): " << input_status.up << " Down(S): " << input_status.down << std::endl;
        std::cout << "Left(A): " << input_status.left << " Right(D): " << input_status.right << std::endl;
        std::cout << "Jump(Space Bar): " << input_status.jump << std::endl; 
        std::cout << std::endl;
    }
};