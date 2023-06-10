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
class PlayerClass: public PersonClass{
    public:
    //input status struct
    InputStatus input_status = {false, false, false, false, false, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, false, false};
    bool fired = false;
    HealthBarClass health_bar = NULL;

    //Constructor, sends the attributes to the parent class and initalizes the input status and default y position
    PlayerClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) : 
    PersonClass(input_x, input_y, width, height, o_shader, texture_dir){
        PersonClass* player_ptr = dynamic_cast<PersonClass*>(this);
        health_bar = HealthBarClass(player_ptr);
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
        if(screen_x == nullptr){
            std::cout << "Screen x is null" << std::endl;
            exit(-1);
        }
        if(std::abs(x - *screen_x) > 0.3)
            *screen_x +=  (x - *screen_x) * delay * 0.01 * std::exp(std::abs(x - *screen_x));
        updateCollisions();
        //update the weapons
        weapon->update(x, y);
        regen();
        
        updateBullets();
        health_bar.draw();

        if(input_status.left_click){
            if(!fired){
                fireWeapon();  
                fired = true;
            }
        } else if(fired){
            fired = false;
        }
        printKeystrokes();
        printPlayerStatus("Update");
        printPlayerCollisionStatus();
        // std::cout << fired << std::endl;
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