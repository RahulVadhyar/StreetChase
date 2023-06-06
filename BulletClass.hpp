class BaseBulletClass: public PhysicsObject{
    public:
        float damage;
        float max_speed;
        float current_speed;
        float height;
        float width;
        float initial_velocity;
    
    BaseBulletClass(float x, float y, Shader *shader, std::string texture_dir):
    PhysicsObject::PhysicsObject(x, y, width, height, shader, texture_dir){
        gravity = false;
    }

    void fire(){
        velocity_x = initial_velocity;
    }

};