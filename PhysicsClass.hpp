struct Collision {
    bool down;
    bool left;
    bool right;
    bool up;
};
class PhysicsObject: public RenderObject {
    public:
        //Physics variables
        float velocity_x = 0, velocity_y = 0;
        float acceleration_x = 0, acceleration_y = 0;
        float last_time = 0;
        float delay;
        bool gravity = true;
        float direction = 1.0f;
        bool snap_collisions = true;
        //collision variables
        std::vector<RenderObject*> collision_objects;
        std::vector<RenderObject*> collided_objects;
        Collision collision_status = {false, false, false, false};
        //Constructor, sends the attributes to the parent class and initalizes the physics variables with default values
        PhysicsObject(float input_x, float input_y, float width, float height, Shader *shader, std::string texture_dir) : 
        RenderObject::RenderObject(input_x, input_y, width, height, shader, texture_dir){}
        PhysicsObject(bool input_snap_collisions, float input_x, float input_y, float width, float height, Shader *shader, std::string texture_dir) : 
        RenderObject::RenderObject(input_x, input_y, width, height, shader, texture_dir){
            snap_collisions = input_snap_collisions;
        }

    private: 
        //gets the collision between the object and another object
        bool getCollision(RenderObject* other){
            bool isCollided = false;
            auto distance_x = (other->width + width + std::abs(x - prev_x))/2 - std::abs(other->x - x);
            auto distance_y = (other->height + height + std::abs(y - prev_y))/2 - std::abs(other->y - y);
            auto side_x = other->x - x;
            auto side_y = other->y - y;

            auto down = distance_y >= 0 && side_y <= 0;
            auto up = distance_y >= 0 && side_y >= 0;
            auto left = distance_x > 0 && side_x <= 0;
            auto right = distance_x >= 0 && side_x >= 0;

            auto mag = distance_y - distance_x;

            //when there is a collision
            if((down||up) && left && mag >= 0){
                //go left
                if(snap_collisions)
                    x = other->x + other->width/2 + width/2;
                if(velocity_x < 0) velocity_x = 0;
                collision_status.left = true;
                isCollided = true;
            } else if((left||right) && down && mag <= 0){
                //go up
                if(snap_collisions)
                    y = other->y + other->height/2 + height/2;
                if(velocity_y < 0) velocity_y = 0;
                collision_status.down = true;
                isCollided = true;
            } else if((down||up) && right && mag >= 0){
                //go right
                if(snap_collisions)
                    x = other->x - other->width/2 - width/2;
                if(velocity_x > 0) velocity_x = 0;
                collision_status.right = true;
                isCollided = true;
            } else if((left||right) && up && mag <= 0){
                //go down
                if(snap_collisions)
                    y = other->y - other->height/2 - height/2;
                if(velocity_y > 0) velocity_y = 0;
                collision_status.up = true;
                isCollided = true;
            } 
            return isCollided;
        }
    public:
        //updates the collision status of the object
        void updateCollisions(){
            collided_objects.clear();
            collision_status = {false, false, false, false};
            if(!collision_objects.empty()){
                for(RenderObject* i : collision_objects){
                    if(i != nullptr){
                        if(getCollision(i)){
                            collided_objects.push_back(i);
                        }
                    }
                }
            }
        }
        //adds an collision object to the list of collision objects
        void addCollisionObject(RenderObject* object){
            collision_objects.push_back(object);
        }
        void removeCollisionObject(RenderObject* object){
            for(int i = 0; i < static_cast<int>(collision_objects.size()); ++i){
                if(collision_objects[i] == object){
                    collision_objects[i] = nullptr;
                    break;
                }
            }
        }

        //updates the coordinates of the object based on the physics variables
        virtual void update(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            delay = 1000*(current_time - last_time) / (double) CLOCKS_PER_SEC;
            delay = 1/delay;

            //update the last time
            last_time = current_time;

            //gravity
            if(gravity)
                acceleration_y = -0.0004;

            //update the coordinates and velocity based on delay
            velocity_x += acceleration_x*delay;
            velocity_y += acceleration_y*delay;

            //speed limits
            if(velocity_x > 0.3) velocity_x = 0.3;
            if(velocity_y > 0.3) velocity_y = 0.3;
            if(velocity_x < -0.3) velocity_x = -0.3;
            if(velocity_y < -0.3) velocity_y = -0.3;

            prev_x = x;
            prev_y = y;

            y += velocity_y*delay;
            x += velocity_x*delay;

            //if the object is out of bounds, then set it to the edge of the screen
            if(y > 1.0f - height/2){
                y = 1.0f - height/2;
            }
            if(y < -1.0f + height/2){
                y = -1.0f + height/2;
            }

            if(velocity_x > 0)
                direction = 1.0f;
            else if(velocity_x < 0)
                direction = -1.0f;
            
        }
  
        
};
