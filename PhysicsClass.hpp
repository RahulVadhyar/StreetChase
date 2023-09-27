struct Collision {
    bool down;
    bool left;
    bool right;
    bool up;
};
class PhysicsObject{
    public:
        //Physics variables
        float velocity_x = 0, velocity_y = 0;
        float old_velocity_x = 0, old_velocity_y = 0;
        float acceleration_x = 0, acceleration_y = 0;
        float old_acceleration_x = 0, old_acceleration_y = 0;
        float last_time = 0;
        float delay;
        bool gravity = true;
        float direction = 1.0f;
        float prev_x = 0, prev_y = 0;
        float x = 0, y = 0, width, height;
        //collision variables
        bool snap_collisions = true;
        std::vector<RenderObject*> render_object_collision;
        std::vector<RenderObject*> render_object_collided;
        std::vector<AnimationClass*> animation_collision;
        std::vector<AnimationClass*> animation_collided;
        Collision collision_status = {false, false, false, false};

        //Constructor, sends the attributes to the parent class and initalizes the physics variables with default values
        PhysicsObject(float input_x, float input_y, float input_width, float input_height){
            x = input_x;
            y = input_y;
            width = input_width;
            height = input_height;

        }
        PhysicsObject(bool input_snap_collisions, float input_x, float input_y, float input_width, float input_height){
            snap_collisions = input_snap_collisions;
            x = input_x;
            y = input_y;
            width = input_width;
            height = input_height;
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
            physicsCollisionDebug("Down: " + std::to_string(down));
            physicsCollisionDebug("Up: " + std::to_string(up));
            physicsCollisionDebug("Left: " + std::to_string(left));
            physicsCollisionDebug("Right: " + std::to_string(right));
            return isCollided;
        }

    private: 
        //gets the collision between the object and another object
        bool getCollision(AnimationClass* other){
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
            physicsCollisionDebug("Down: " + std::to_string(down));
            physicsCollisionDebug("Up: " + std::to_string(up));
            physicsCollisionDebug("Left: " + std::to_string(left));
            physicsCollisionDebug("Right: " + std::to_string(right));
            return isCollided;
        }

    public:
        //updates the collision status of the object based on the collision objects set for the object
        void updateCollisions(){
            render_object_collided.clear();
            animation_collided.clear();
            collision_status = {false, false, false, false};
            if(!render_object_collision.empty()){
                for(RenderObject* i : render_object_collision){
                    if(i != nullptr){
                        if(getCollision(i)){
                            render_object_collided.push_back(i);
                        }
                    }
                }
            }
            if(!animation_collision.empty()){
                for(AnimationClass* i : animation_collision){
                    if(i != nullptr){
                        if(getCollision(i)){
                            animation_collided.push_back(i);
                        }
                    }
                }
            }
        }

    public:
        //adds an collision object to the list of collision objects
        void addCollisionObject(RenderObject* object){
            render_object_collision.push_back(object);
            physicsCollisionDebug("Added collision object: " + std::to_string((long)object));
        }
    
    public:
        //adds an collision object to the list of collision objects
        void addCollisionObject(AnimationClass* object){
            animation_collision.push_back(object);
            physicsCollisionDebug("Added collision object: " + std::to_string((long)object));
        }   

    public:
        //removes a collision object from the list of collision objects
        void removeCollisionObject(RenderObject* object){
            for(int i = 0; i < static_cast<int>(render_object_collision.size()); ++i){
                if(render_object_collision[i] == object){
                    physicsCollisionDebug("Removed collision object: " + std::to_string((long)object));
                    render_object_collision[i] = nullptr;
                    break;
                }
            }
        }
    
    public:
        //removes a collision object from the list of collision objects
        void removeCollisionObject(AnimationClass* object){
            for(int i = 0; i < static_cast<int>(animation_collision.size()); ++i){
                if(animation_collision[i] == object){
                    physicsCollisionDebug("Removed collision object: " + std::to_string((long)object));
                    animation_collision[i] = nullptr;
                    break;
                }
            }
        }

    public:
        //updates the coordinates of the object based on the physics variables
        virtual void update(){
            //use the time to calculate the delay between frames and use it to calculate the new coordinates
            auto current_time = std::clock();
            delay = 75*(current_time - last_time) / (double) CLOCKS_PER_SEC;
            physicsUpdateDebug("Delay: " + std::to_string(delay));
            //update the last time
            last_time = current_time;

            //gravity
            if(gravity)
                acceleration_y = -0.005;

            //update the coordinates and velocity based on delay
            velocity_x += (acceleration_x + old_acceleration_x)*delay/2;
            velocity_y += (acceleration_y + old_acceleration_y)*delay/2;
            physicsUpdateDebug("Velocity_X: " + std::to_string(velocity_x) + " Velocity_Y: " + std::to_string(velocity_y));

            old_acceleration_x = acceleration_x;
            old_acceleration_y = acceleration_y;


            //speed limits
            if(velocity_x > 0.3) velocity_x = 0.3;
            if(velocity_y > 0.3) velocity_y = 0.3;
            if(velocity_x < -0.3) velocity_x = -0.3;
            if(velocity_y < -0.3) velocity_y = -0.3;

            prev_x = x;
            prev_y = y;

            y += (velocity_y + old_velocity_y)*delay/2;
            x += (velocity_x + old_velocity_x)*delay/2;

            old_velocity_x = velocity_x;
            old_velocity_y = velocity_y;
            physicsUpdateDebug("X: " + std::to_string(x) + " Y: " + std::to_string(y));

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
    
    private:
        void physicsUpdateDebug(std::string message){
            #ifdef PHYSICS_UPDATE_DEBUG
            std::cout << "[PHYSICS_UPDATE_DEBUG]::[OBJECT:" << this << "]: " << message << std::endl;
            #endif
        }

    private:
        void physicsCollisionDebug(std::string message){
            #ifdef PHYSICS_COLLISION_DEBUG
            std::cout << "[PHYSICS_COLLISION_DEBUG]::[OBJECT:" << this << "]: " << message << std::endl;
            #endif
        }
};
