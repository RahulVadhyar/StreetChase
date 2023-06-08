class BaseBulletClass: public PhysicsObject{
    public:
        float damage;
        float max_speed;
        float current_speed;
        float height;
        float width;
        float initial_velocity;
        float start_time;
    
    BaseBulletClass(float x, float y, float input_width, float input_height, Shader *shader, std::string texture_dir):
    PhysicsObject::PhysicsObject(x, y, input_width, input_height, shader, texture_dir){
        width = input_width;
        height = input_height;
        gravity = false;
        
    }
    virtual ~BaseBulletClass(){
        delete shader;
    };
    void fire(){
        start_time = std::clock();
        velocity_x = initial_velocity;
    }

    bool isTimeOver(){
        auto time_elapsed = (std::clock() - start_time) / (double) CLOCKS_PER_SEC;
        return time_elapsed > 5;
    }
    void update(){
        PhysicsObject::update();
        updateCollisions();
        draw();
    }
};