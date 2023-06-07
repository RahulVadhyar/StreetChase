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
        float delay;
        bool gravity;
        float direction;
        //collision variables
        std::vector<RenderObject*> collision_objects;
        Collision collision_status;
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
            collision_status = {false, false, false, false};
            gravity = true;
            float direction = 1.0f;

        }
        PhysicsObject(float x, float y, float width, float height, Shader *shader, std::string texture_dir) : 
        RenderObject::RenderObject(width, height, shader, texture_dir)
        {
            default_x = x;
            default_y = y;
            x = default_x;
            y = default_y;
            velocity_x = 0;
            velocity_y = 0;
            acceleration_x = 0;
            acceleration_y = 0;
            collision_status = {false, false, false, false};
            gravity = true;

        }
    private: 
        //gets the collision between the object and another object
        void getCollision(RenderObject other){
            auto distance_x = (other.width/2 + width/2) - std::abs(other.x - x);
            auto distance_y = (other.height/2 + height/2) - std::abs(other.y - y);
            auto side_x = other.x - x;
            auto side_y = other.y - y;

            auto down = distance_y >= 0 && side_y <= 0;
            auto up = distance_y >= 0 && side_y >= 0;
            auto left = distance_x > 0 && side_x <= 0;
            auto right = distance_x >= 0 && side_x >= 0;

            auto mag = distance_y - distance_x;

            //when there is a collision
            if((down||up) && left && mag >= 0){
                //go left
                x = other.x + other.width/2 + width/2;
                if(velocity_x < 0) velocity_x = 0;
                collision_status.left = true;
            } else if((left||right) && down && mag <= 0){
                //go up
                y = other.y + other.height/2 + height/2;
                if(velocity_y < 0) velocity_y = 0;
                collision_status.down = true;
            } else if((down||up) && right && mag >= 0){
                //go right
                x = other.x - other.width/2 - width/2;
                if(velocity_x > 0) velocity_x = 0;
                collision_status.right = true;
            } else if((left||right) && up && mag <= 0){
                //go down
                y = other.y - other.height/2 - height/2;
                if(velocity_y > 0) velocity_y = 0;
                collision_status.up = true;
            } 
        }
    public:
        //updates the collision status of the object
        void updateCollisions(){
            collision_status = {false, false, false, false};
            if(!collision_objects.empty())
                for(RenderObject *i : collision_objects)
                    getCollision(*i);
        }
        //adds an collision object to the list of collision objects
        void addCollisionObject(RenderObject *object){
            collision_objects.push_back(object);
        }

        //updates the coordinates of the object based on the physics variables
        void update(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            delay = 200*(current_time - last_time) / (double) CLOCKS_PER_SEC;

            //gravity
            if(gravity)
                acceleration_y = -0.0005;

            //update the coordinates and velocity based on delay
            velocity_x += acceleration_x*delay;
            velocity_y += acceleration_y*delay;

            //speed limits
            if(velocity_x > 0.3) velocity_x = 0.3;
            if(velocity_y > 0.3) velocity_y = 0.3;
            if(velocity_x < -0.3) velocity_x = -0.3;
            if(velocity_y < -0.3) velocity_y = -0.3;
    
            y += velocity_y*delay;
            x += velocity_x*delay;


            //if the object is out of bounds, then set it to the edge of the screen
            if(y > 1.0f - height/2){
                y = 1.0f - height/2;
            }
            if(y < -1.0f + height/2){
                y = -1.0f + height/2;
            }

            //update the last time
            last_time = current_time;
            if(velocity_x > 0)
                direction = 1.0f;
            else if(velocity_x < 0)
                direction = -1.0f;
            
        }
  
        
};
