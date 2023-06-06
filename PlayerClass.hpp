//creating a struct for the input status, helps to keep things organized
struct InputStatus{
    bool up;
    bool down;
    bool left;
    bool right;
    bool jump;
    double mouse_x;
    double mouse_y;
    bool left_click;
    bool right_click;
};

//main player class
class PlayerClass: public PhysicsObject, public Health{
    public:
    //input status struct
    InputStatus input_status;
    int num_weapons;
    std::list<BaseWeaponClass*> weapons;

    //Constructor, sends the attributes to the parent class and initalizes the input status and default y position
    PlayerClass(float width, float height, Shader *o_shader, std::string texture_dir) : 
    PhysicsObject::PhysicsObject(width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){
    input_status = {false, false, false, false, false, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, false, false};
    num_weapons = 0;
    }

    void addWeapon(BaseWeaponClass* weapon){
        num_weapons += 1;
        weapons.push_back(weapon);
    }

    void removeWeapon(int index){
        num_weapons -= 1;
        weapons.remove(weapons.at(index));
    }

    //Updates the player's velocity based on inputs
    void update(){
        velocity_x = 0;
        if(input_status.left) velocity_x = -0.005;
        if(input_status.right) velocity_x = 0.005;
        if(input_status.down) velocity_y = -0.005;
        if(input_status.up) velocity_y = 0.01;
        if(input_status.jump && collision_status.down) velocity_y = 0.025;
        PhysicsObject::update();
        //move the screen if the player is too far to the right or left
        if(std::abs(x - *screen_x) > 0.3)
            *screen_x +=  (x - *screen_x) * delay * 0.01 * std::exp(std::abs(x - *screen_x));
        updateCollisions();
        Health::regen();
        printKeystrokes();
        printPlayerStatus("Update");
        printPlayerCollisionStatus();
    }

    //debugging: prints the inputs
    void printKeystrokes(){
        #ifdef INPUT_STATUS
        std::cout << "DEBUG: Input Status" << std::endl;
        std::cout << "Up(W): " << input_status.up << " Down(S): " << input_status.down << std::endl;
        std::cout << "Left(A): " << input_status.left << " Right(D): " << input_status.right << std::endl;
        std::cout << "Jump(Space Bar): " << input_status.jump << std::endl; 
        std::cout << "Mouse X: " << input_status.mouse_x << " Mouse Y: " << input_status.mouse_y << std::endl;
        std::cout << "Left Click: " << input_status.left_click << " Right Click: " << input_status.right_click << std::endl;
        std::cout << std::endl;
        #endif
    }
    //if needed then it prints out the current status of player
    //mainly for debugging purposes
    void printPlayerStatus(std::string function_name){
    #ifdef PRINT_PLAYER_STATUS
        std::cout << "DEBUG: Current Player Status" << std::endl;
        std::cout << "Function name: " << function_name << std::endl;
        std::cout << "x: " << x << " y: " << y << std::endl;
        std::cout << "velocity_x: " << velocity_x << " velocity_y: " << velocity_y << std::endl;
        std::cout << "acceleration_x: " << acceleration_x << " acceleration_y: " << acceleration_y << std::endl << std::endl;
        
    #endif
    }

    void printPlayerCollisionStatus(){
    #ifdef PRINT_COLLISION_STATUS
        std::cout << "DEBUG: Current Player Collision Status" << std::endl;
        std::cout << "Collision Down status: " << collision_status.down << std::endl;
        std::cout << "Collision Up status: " << collision_status.up << std::endl;
        std::cout << "Collision Right status: " << collision_status.right << std::endl;
        std::cout << "Collision Left status: " << collision_status.left  << std::endl << std::endl;
    #endif
    }
};