class PersonClass: public PhysicsObject, public Health{
    public:
    BaseWeaponClass* weapon = nullptr;
    std::vector<BaseBulletClass*> bullets;
    int current_weapon = 0;
    bool fired = false;

    //Constructor, sends the attributes to the parent class and initalizes the input status and default y position
    PersonClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) : 
    PhysicsObject::PhysicsObject(input_x, input_y, width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){}

    void addWeapon(BaseWeaponClass* input_weapon){
        weapon = input_weapon;
    }
     
    void fireWeapon(){
        if(weapon->current_ammo > 0){
            if(!fired){
                BaseBulletClass* bullet = weapon->fire(x, y, direction);
                bullets.push_back(bullet);
                bullet->screen_x = screen_x;
                if(collision_objects.empty()){
                    std::cout << "Collision objects is empty" << std::endl;
                    exit(-1);
                }
                for(auto object: collision_objects)
                    if(object != nullptr)
                        bullet->addCollisionObject(object);    
                bullet->addCollisionObject(this);         
            }
            fired = true;
        } else {
            fired = false;
        }
    }
};