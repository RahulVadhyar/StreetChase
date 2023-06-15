class BaseBulletClass: public PhysicsObject{
    public:
        float damage = 0;
        float max_speed = 0;
        float current_speed = 0;
        float height = 0.1;
        float width = 0.1;
        float initial_velocity = 0;
        float start_time = 0;
        float max_time = 5;
    
        BaseBulletClass(float x, float y, float input_width, float input_height, Shader *shader, std::string texture_dir):
        PhysicsObject::PhysicsObject(x, y, input_width, input_height, shader, texture_dir){
            width = input_width;
            height = input_height;
            gravity = false;
            snap_collisions = false;
            bulletDebug("Bullet initialized");
        }

    public:
        virtual ~BaseBulletClass(){
            delete shader;
            bulletDebug("Bullet deleted along with shader: " + std::to_string((long)shader));
        };

    public:
        void fire(float direction){
            start_time = std::clock();
            velocity_x = initial_velocity*direction;
            bulletDebug("Bullet fired with velocity_x: " + std::to_string(velocity_x));
        }

    public:
        bool isTimeOver(){
            auto time_elapsed = (std::clock() - start_time) / (double) CLOCKS_PER_SEC;
            return time_elapsed > max_time;
        }
        
    public:
        void update(){
            PhysicsObject::update();
            bulletDebug("Bullet physics updated");
            updateCollisions();
            bulletDebug("Bullet collisions updated");
            draw();
            bulletDebug("Bullet drawn");
        }

    private:
        void bulletDebug(std::string message){
            #ifdef BULLET_DEBUG
            std::cout << "[BULLET_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
};