class PhysicsObject: public RenderObject {
    public:
        //Physics variables
        float velocity_x, velocity_y;
        float default_x, default_y;
        float acceleration_x, acceleration_y;
        bool gravity;
        float last_time;
        bool is_grounded;

        //Constructor, sends the attributes to the parent class and initalizes the physics variables with default values
        PhysicsObject(float width, float height, Shader *shader, std::string texture_dir) : 
        RenderObject::RenderObject(width, height, shader, texture_dir)
        {
            default_x = 0;
            default_y = 0;
            x = default_x;
            y = default_y;
            velocity_x = 0;
            velocity_y = 0;
            acceleration_x = 0;
            acceleration_y = 0;
            gravity = true;
            is_grounded = true;
        }

        //Handles collision with other objects
        void collision(RenderObject other){
            //Check if the object is colliding with another object, if it is, 
            //move the object to the edge of the other object on the nearest side
            //and set the respective accceleration and velocity to 0
            if(isColliding(other)){
                if(std::abs(other.x - x) >= std::abs(other.y - y)){
                    if(other.x > x){
                         x = other.x - other.width/2 - width/2;
                    } else { 
                        x = other.x + other.width/2 + width/2;
                    }
                    
                    acceleration_x = 0;
                    velocity_x = 0;
                } else {
                    if(other.y > y){ y = other.y - other.height/2 - height/2;}
                    else{ y = other.y + other.height/2 + height/2;}
                    acceleration_y = 0;
                    velocity_y = 0;
                }
            } 
            //debugging
            #ifdef PRINT_PLAYER_STATUS
            std::cout << "Status player.collision():" << std::endl;
            printPlayerStatus();
            #endif
        }

        //Checks if the object is colliding with another object
        bool isColliding(RenderObject other){
            //Check if the object is colliding with another object with a small gap as buffer, if it is,
            //return true, else return false and set the is_grounded variable respectively
            if(x + width/2 > other.x - other.width/2 - 0.01 && x - width/2 < other.x + other.width/2 + 0.01){
                if(y + height/2 > other.y - other.height/2 -0.01 && y - height/2 < other.y + other.height/2 + 0.01){
                    is_grounded = true;
                    return true;
                }
            }
            is_grounded = false;
            return false;
        }

        //updates the coordinates of the object based on the physics variables
        void update_coords(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            auto delay = (current_time - last_time) / (double) CLOCKS_PER_SEC;
            
            //if gravity is enabled and the object is not grounded, then apply gravity
            if(gravity && !is_grounded)
                acceleration_y = -0.04f;
            else{
                acceleration_y = 0;
            }

            //update the coordinates and velocity based on delay
            velocity_x += acceleration_x*delay;
            velocity_y += acceleration_y*delay;
            y += velocity_y*delay;
            x += velocity_x*delay;

            //if the object is out of bounds, then set it to the edge of the screen
            if(x > 1.0f - width/2){
                x = 1.0f - width/2;
            }
            if(x < -1.0f + width/2){
                x = -1.0f + width/2;
            }
            if(y > 1.0f - height/2){
                y = 1.0f - height/2;
            }
            if(y < -1.0f + height/2){
                y = -1.0f + height/2;
            }

            //update the last time
            last_time = current_time;

            //debugging
            #ifdef PRINT_PLAYER_STATUS
            std::cout << "Status player.update_coords():" << std::endl;
            printPlayerStatus();
            #endif
        }

        //if needed then it prints out the current status of player
        //mainly for debugging purposes
        void printPlayerStatus(){
            std::cout << "DEBUG: Current Player Status" << std::endl;
            std::cout << "x: " << x << " y: " << y << std::endl;
            std::cout << "velocity_x: " << velocity_x << " velocity_y: " << velocity_y << std::endl;
            std::cout << "acceleration_x: " << acceleration_x << " acceleration_y: " << acceleration_y << std::endl;
            std::cout << "Collision status:  " << is_grounded << std::endl << std::endl;
        }
};
