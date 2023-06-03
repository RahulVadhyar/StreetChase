struct Collision {
    bool down;
    bool left;
    bool right;
    bool up;
};
class PhysicsObject: public RenderObject {
    public:
        //Physics variables
        float velocity_x, velocity_y;
        float default_x, default_y;
        float acceleration_x, acceleration_y;
        float last_time;
        bool is_touching_ground = false;
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


        }
        Collision getCollision(RenderObject other){
            Collision collision = {false, false, false, false};
            auto distance_x = (other.width/2 + width/2) - std::abs(other.x - x);
            auto distance_y = (other.height/2 + height/2) - std::abs(other.y - y);
            auto side_x = other.x - x;
            auto side_y = other.y - y;

            auto down = distance_y >= 0 && side_y <= 0;
            auto up = distance_y >= 0 && side_y >= 0;
            auto left = distance_x > 0 && side_x <= 0;
            auto right = distance_x >= 0 && side_x >= 0;

            auto mag = distance_y - distance_x;
            is_touching_ground = false;
            if((down||up) && left && mag >= 0){
                //go left
                x = other.x + other.width/2 + width/2;
                if(velocity_x < 0) velocity_x = 0;
                collision.left = true;
            } else if((left||right) && down && mag <= 0){
                //go up
                y = other.y + other.height/2 + height/2;
                if(velocity_y < 0) velocity_y = 0;
                collision.down = true;
                is_touching_ground = true;
            } else if((down||up) && right && mag >= 0){
                //go right
                x = other.x - other.width/2 - width/2;
                if(velocity_x > 0) velocity_x = 0;
                collision.right = true;
            } else if((left||right) && up && mag <= 0){
                //go down
                y = other.y - other.height/2 - height/2;
                if(velocity_y > 0) velocity_y = 0;
                collision.up = true;
            } 
            printPlayerStatus("player.getCollision()");
            printPlayerCollisionStatus(collision);
            return collision;
        }


        //updates the coordinates of the object based on the physics variables
        void update_coords(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            auto delay = 1000*(current_time - last_time) / (double) CLOCKS_PER_SEC;

            acceleration_y = -0.001;

            //update the coordinates and velocity based on delay
            velocity_x += acceleration_x*delay;
            velocity_y += acceleration_y*delay;

            if(velocity_x > 0.3) velocity_x = 0.3;
            if(velocity_y > 0.3) velocity_y = 0.3;
            if(velocity_x < -0.3) velocity_x = -0.3;
            if(velocity_y < -0.3) velocity_y = -0.3;

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
            printPlayerStatus("player.update_coords()");
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

        void printPlayerCollisionStatus(Collision collision){{
        #ifdef PRINT_COLLISION_STATUS
            std::cout << "DEBUG: Current Player Collision Status" << std::endl;
            std::cout << "Collision Down status: " << collision.down << std::endl;
            std::cout << "Collision Up status: " << collision.up << std::endl;
            std::cout << "Collision Right status: " << collision.right << std::endl;
            std::cout << "Collision Left status: " << collision.left  << std::endl << std::endl;
        #endif
        }}
};
