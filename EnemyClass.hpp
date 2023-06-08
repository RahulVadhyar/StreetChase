class EnemyClass : public PhysicsObject, public Health{
    public:
    BaseWeaponClass* weapon;
    std::vector<BaseBulletClass*> bullets;
    int current_weapon = 0;

    EnemyClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) :
    PhysicsObject::PhysicsObject(width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){
        x = input_x;

    }

    void addWeapon(BaseWeaponClass* input_weapon){
        weapon = input_weapon;
    }

    void fireWeapon(){
        if(weapon->current_ammo > 0){
            auto bullet = weapon->fire(x, y);
            bullets.push_back(bullet);
            bullet->screen_x = screen_x;
            bullet->velocity_x = bullet->initial_velocity*direction;
            bullet->addCollisionObject(this);             
        }
    }

    void update() override{
        PhysicsObject::update();
        updateCollisions();
        //update the weapons
        weapon->update(x, y);
        for(auto &bullet: bullets){
            if(bullet != nullptr){
                bullet->update();
                if(bullet->isTimeOver() || bullet->collision_status.down || bullet->collision_status.up || bullet->collision_status.left || bullet->collision_status.right){
                    delete bullet;
                    bullet = nullptr;
                    std::cout << "taking damage" << std::endl;
                    this->takeDamage(0.1);
                    std::cout << "Ammo left" << weapon->current_ammo << std::endl;

                }
            }
        }
    }
};