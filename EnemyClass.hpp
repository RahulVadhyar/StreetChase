class EnemyClass : public PhysicsObject, public Health{
    public:
    BaseWeaponClass* weapon = nullptr;
    std::vector<BaseBulletClass*> bullets;
    int current_weapon = 0;

    EnemyClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) :
    PhysicsObject::PhysicsObject(input_x, input_y, width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){}

    void addWeapon(BaseWeaponClass* input_weapon){
        weapon = input_weapon;
    }

    void fireWeapon(){
        if(weapon == nullptr){
            std::cout << "No weapon" << std::endl;
            exit(-1);
        };
        if(weapon->current_ammo > 0){
            auto bullet = weapon->fire(x, y, direction);
            bullets.push_back(bullet);
            bullet->screen_x = screen_x;
            bullet->velocity_x = bullet->initial_velocity*direction;
            if(collision_objects.empty()){
                std::cout << "Collision objects is empty" << std::endl;
                exit(-1);
            }
            if(!collision_objects.empty()){
                for(auto object: collision_objects)
                    if(object != nullptr)
                        bullet->addCollisionObject(object);
            }            
        }
    }

    void update() override{
        PhysicsObject::update();
        updateCollisions();
        //update the weapons
        if(weapon == nullptr){
            std::cout << "No weapon" << std::endl;
            exit(-1);
        };
        weapon->update(x, y);
        if(bullets.size() > 0){
            for(auto &bullet: bullets){
                if(bullet != nullptr){
                    bullet->update();
                    if(bullet->isTimeOver() || bullet->collision_status.down || bullet->collision_status.up || bullet->collision_status.left || bullet->collision_status.right){
                        delete bullet;
                        bullet = nullptr;
                        std::cout << "Ammo left" << weapon->current_ammo << std::endl;

                    }
                }
            }
        }
    }
};