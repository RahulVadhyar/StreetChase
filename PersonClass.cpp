class PersonClass: public PhysicsObject, public Health{
    public:
        BaseWeaponClass* weapon = nullptr;
        std::vector<BaseBulletClass*> bullets;
        int current_weapon = 0;

        //Constructor, sends the attributes to the parent class and initalizes the input status and default y position
        PersonClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) : 
        PhysicsObject::PhysicsObject(input_x, input_y, width, height, o_shader, texture_dir), Health::Health(1.0, 0.01){}

    public:
        void addWeapon(BaseWeaponClass* input_weapon){
            weapon = input_weapon;
            personAddWeaponDebug("Weapon " + std::to_string((long)weapon) + " added");
        }
    
    protected:
        void updateBullets(){
            if(bullets.size() > 0){
                for(auto &bullet: bullets){
                    if(bullet != nullptr){
                        bullet->update();
                        personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " updated");
                        bullet->updateCollisions();
                        personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collisions updated");
                        if(bullet->isTimeOver()){
                            personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " deleting");
                            delete bullet;
                            bullet = nullptr;
                        } else if(bullet->collision_status.down || bullet->collision_status.up || bullet->collision_status.left || bullet->collision_status.right){
                            personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collided");
                            for(auto object: bullet->collided_objects){
                                if(object != nullptr && dynamic_cast<PersonClass*>(object)){
                                    PersonClass* enemy = dynamic_cast<PersonClass*>(object);
                                    enemy->takeDamage(bullet->damage);
                                    personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collided with " + std::to_string((long)enemy) + " and dealt " + std::to_string(bullet->damage) + " damage");
                                }
                            }
                            delete bullet;
                            bullet = nullptr;
                            personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " deleted");                      
                        }
                    }
                }
            }
        }
    
    public:
        void fireWeapon(){
            if(weapon == nullptr){
                std::cout << "No weapon" << std::endl;
                exit(-1);
            };
            if(weapon->current_ammo > 0){
                    BaseBulletClass* bullet = weapon->fire(x, y, direction);
                    personFireWeaponDebug("Bullet " + std::to_string((long)bullet) + " fired");
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
                    personFireWeaponDebug("Bullet " + std::to_string((long)bullet) + " collision objects added");
            }
        }
    
    private:
        void personUpdateBulletDebug(std::string message){
            #ifdef PERSON_UPDATE_BULLET_DEBUG
            std::cout << "[PERSON_UPDATE_BULLET_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }

    private:
        void personFireWeaponDebug(std::string message){
            #ifdef PERSON_FIRE_WEAPON_DEBUG
            std::cout << "[PERSON_FIRE_WEAPON_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
    
    private:
        void personAddWeaponDebug(std::string message){
            #ifdef PERSON_ADD_WEAPON_DEBUG
            std::cout << "[PERSON_ADD_WEAPON_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
};