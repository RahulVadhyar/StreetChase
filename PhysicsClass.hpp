class PhysicsObject: public RenderObject {
    public:
        float velocity_x, velocity_y;
        float default_x, default_y;
        float acceleration_x, acceleration_y;
        bool gravity;
        float last_time;
        bool is_grounded;
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

        void collision(RenderObject other){
            if(isColliding(other)){
            if(std::abs(other.x - x) >= std::abs(other.y - y)){
                if(other.x > x){ x = other.x - other.width/2 - width/2;}
                else{ x = other.x + other.width/2 + width/2;}
                acceleration_x = 0;
                velocity_x = 0;
            } else {
                if(other.y > y){ y = other.y - other.height/2 - height/2;}
                else{ y = other.y + other.height/2 + height/2;}
                acceleration_y = 0;
                velocity_y = 0;
            }
            } 
            #ifdef PRINT_PLAYER_STATUS
            std::cout << "Status player.collision():" << std::endl;
            printPlayerStatus();
            #endif
        }
        bool isColliding(RenderObject other){
            if(x + width/2 > other.x - other.width/2 - 0.01 && x - width/2 < other.x + other.width/2 + 0.01){
                if(y + height/2 > other.y - other.height/2 -0.01 && y - height/2 < other.y + other.height/2 + 0.01){
                    is_grounded = true;
                    return true;
                }
            }
            is_grounded = false;
            return false;
        }



        void update_coords(){
            auto current_time = std::clock();
            auto delay = (current_time - last_time) / (double) CLOCKS_PER_SEC;

            if(gravity && !is_grounded)
                acceleration_y = -0.04f;
            else{
                acceleration_y = 0;
            }

            velocity_x += acceleration_x*delay;
            
            velocity_y += acceleration_y*delay;
            y += velocity_y*delay;
            x += velocity_x*delay;
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
            last_time = current_time;
        }
        #ifdef PRINT_PLAYER_STATUS
        std::cout << "Status player.update_coords():" << std::endl;
        printPlayerStatus();
        #endif
        }

        void printPlayerStatus(){
            std::cout << "DEBUG: Current Player Status" << std::endl;
            std::cout << "x: " << x << " y: " << y << std::endl;
            std::cout << "velocity_x: " << velocity_x << " velocity_y: " << velocity_y << std::endl;
            std::cout << "acceleration_x: " << acceleration_x << " acceleration_y: " << acceleration_y << std::endl;
            std::cout << "Collision status:  " << is_grounded << std::endl << std::endl;

        }
};
