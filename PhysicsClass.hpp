class PhysicsObject: public RenderObject {
    public:
        //Physics variables
        float velocity_x, velocity_y;
        float default_x, default_y;
        float acceleration_x, acceleration_y;
        float last_time;
        bool is_grounded;
        bool wall_colliding;

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
            is_grounded = true;
            wall_colliding = false;
        }

        void collisionY(RenderObject other){
            is_grounded = false;
            if(other.y + other.height/2 > y - height/2){
                y = other.y + other.height/2 + height/2;
                is_grounded = true;
            } 
            //debugging
            #ifdef PRINT_PLAYER_STATUS
            std::cout << "Status player.collisionY():" << std::endl;
            printPlayerStatus();
            #endif
        }

        void collisionX(RenderObject other){
            wall_colliding = false;
            if(other.x - other.width/2 < x + width/2){
                x = other.x - other.width/2 -width/2;
                wall_colliding = true;
            }

            //debugging
            #ifdef PRINT_PLAYER_STATUS
            std::cout << "Status player.collisionX():" << std::endl;
            printPlayerStatus();
            #endif
        }

        // void collisionXandY(RenderObject other){

        // }

        //updates the coordinates of the object based on the physics variables
        void update_coords(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            auto delay = 1000*(current_time - last_time) / (double) CLOCKS_PER_SEC;

            acceleration_y = -0.001;

            //update the coordinates and velocity based on delay
            velocity_x += acceleration_x*delay;
            
            if(velocity_y < 0 && is_grounded)
                velocity_y = 0;
            velocity_y += acceleration_y*delay;
            y += velocity_y*delay;
            x += velocity_x*delay;
            if(velocity_x > 0.03) velocity_x = 0.03;
            if(velocity_y > 0.03) velocity_y = 0.03;
            if(velocity_x < -0.03) velocity_x = -0.03;
            if(velocity_y < -0.03) velocity_y = -0.03;

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
            std::cout << "Collision status: " << is_grounded << std::endl;
            std::cout << "Wall Collision status: " << wall_colliding << std::endl << std::endl;
        }
};
