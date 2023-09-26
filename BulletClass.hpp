//This is the base class from which bullets of all types inherit from
class BaseBulletClass: public PhysicsObject, public RenderObject{
    public:
        //Bullet properties
        float damage = 0;
        float height = 0.1;
        float width = 0.1;
        float initial_velocity = 0;
        float start_time = 0;
        float max_time = 5; //max time before bullet is destroyed
        Shader bullet_shader = Shader(shader_dir::vs::shader, shader_dir::fs::shader);
    
        BaseBulletClass(float x, float y, float input_width, float input_height, std::string texture_dir):
        PhysicsObject::PhysicsObject(x, y, input_width, input_height), RenderObject::RenderObject(x, y, input_width, input_height, &bullet_shader, texture_dir){
            bulletDebug("Initializing bullet");
            width = input_width;
            height = input_height;
            PhysicsObject::gravity = false; //turn off gravity for bullets
            PhysicsObject::snap_collisions = true; //turn off snapping for bullets
        }

    public:
        void fire(float direction){
            //when bullet is fired, start the clock so that we know when to destroy the bullet and give it initial velcoity
            start_time = std::clock();
            PhysicsObject::velocity_x = initial_velocity*PhysicsObject::direction;
            bulletDebug("Bullet fired with velocity_x: " + std::to_string(PhysicsObject::velocity_x));
        }

    public:
        bool isTimeOver(){
            //check if the bullet has been alive for more than max_time
            auto time_elapsed = (std::clock() - start_time) / (double) CLOCKS_PER_SEC;
            return time_elapsed > max_time;
        }
        
    public:
        void update(){
            //update the bullet physics, collisions and draw the bullet
            PhysicsObject::update();
            bulletDebug("Bullet physics updated");
            updateCollisions();
            bulletDebug("Bullet collisions updated");
            RenderObject::x = PhysicsObject::x;
            RenderObject::y = PhysicsObject::y;
            draw();
            bulletDebug("Bullet drawn");
            bulletDebug("posX: " + std::to_string(PhysicsObject::x) + " posY: " + std::to_string(PhysicsObject::y));
        }

    private:
        void bulletDebug(std::string message){
            #ifdef BULLET_DEBUG
            std::cout << "[BULLET_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
};