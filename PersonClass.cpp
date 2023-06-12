class PersonClass: public PhysicsObject, public Health{
    public:
    BaseWeaponClass* weapon = nullptr;
    std::vector<BaseBulletClass*> bullets;
    int current_weapon = 0;

    //Constructor, sends the attributes to the parent class and initalizes the input status and default y position
    PersonClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) : 
    PhysicsObject::PhysicsObject(input_x, input_y, width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){}

    void addWeapon(BaseWeaponClass* input_weapon){
        weapon = input_weapon;
    }
     
    void updateBullets(){
        if(bullets.size() > 0){
            for(auto &bullet: bullets){
                if(bullet != nullptr){
                    bullet->update();
                    bullet->updateCollisions();
                    if(bullet->isTimeOver()){
                        delete bullet;
                        bullet = nullptr;
                    } else if(bullet->collision_status.down || bullet->collision_status.up || bullet->collision_status.left || bullet->collision_status.right){
                        for(auto object: bullet->collided_objects){
                            if(object != nullptr && dynamic_cast<PersonClass*>(object)){
                                PersonClass* enemy = dynamic_cast<PersonClass*>(object);
                                enemy->takeDamage(bullet->damage);
                                std::cout << "Enemy hit!" << std::endl;
                            }
                        }
                        delete bullet;
                        bullet = nullptr;
                        std::cout << "Ammo left" << weapon->current_ammo << std::endl;                        
                    }
                }
            }
        }
    }

    void fireWeapon(){
        if(weapon == nullptr){
            std::cout << "No weapon" << std::endl;
            exit(-1);
        };
        if(weapon->current_ammo > 0){
                BaseBulletClass* bullet = weapon->fire(x, y, direction);
                bullets.push_back(bullet);
                bullet->screen_x = screen_x;
                if(collision_objects.empty()){
                    std::cout << "Collision objects is empty" << std::endl;
                    exit(-1);
                }
                for(auto object: collision_objects){
                    if(object != nullptr){
                        if(dynamic_cast<PersonClass*>(object)){
                            
                        bullet->addCollisionObject(object); 
                        }
                   }    
                }
        }
    }
};