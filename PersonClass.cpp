//Common things with player and enemy(weapons and bullets)
class PersonClass: public PhysicsObject, public Health, public AnimationClass{
    public:
        //weapons and bullets
        BaseWeaponClass* weapon = nullptr;
        std::vector<BaseBulletClass*> bullets;
        int current_weapon = 0;

        PersonClass(float input_x, float input_y, float width, float height) : 
        PhysicsObject::PhysicsObject(input_x, input_y, width, height),
        Health::Health(1.0, 0.01),
        AnimationClass(input_x, input_y, width, height){}

    public:
        void addWeapon(BaseWeaponClass* input_weapon){
            weapon = input_weapon;
            personAddWeaponDebug("Weapon " + std::to_string((long)weapon) + " added");
        }
    
    protected:
        void updateBullets(){
            if(bullets.size() > 0){
                //for each bullet update physics, collisions, and check if the bullet is out of time or has collided with an enemy and perform appropriate actions
                for(auto &bullet: bullets){
                    if(bullet != nullptr){
                        //physics and collisons
                        bullet->update();
                        personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " updated");
                        bullet->updateCollisions();
                        personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collisions updated");

                        //is out of time delete bullet
                        if(bullet->isTimeOver()){
                            personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " deleting");
                            delete bullet;
                            bullet = nullptr;

                        //has collided with an enemy
                        } else if(bullet->collision_status.down || bullet->collision_status.up || bullet->collision_status.left || bullet->collision_status.right){
                            personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collided");
                            for(auto object: bullet->animation_collided){
                                if(object != nullptr && dynamic_cast<PersonClass*>(object)){
                                    PersonClass* enemy = dynamic_cast<PersonClass*>(object);
                                    enemy->takeDamage(bullet->damage);
                                    personUpdateBulletDebug("Bullet " + std::to_string((long)bullet) + " collided with " + std::to_string((long)enemy) + " and dealt " + std::to_string(bullet->damage) + " damage");
                                }
                            }
                            //delete the bullet
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
                    //fire the weapon and add the bullet to the bullets vector
                    BaseBulletClass* bullet = weapon->fire(AnimationClass::x, AnimationClass::y, direction);
                    personFireWeaponDebug("Bullet " + std::to_string((long)bullet) + " fired");
                    bullets.push_back(bullet);
                    bullet->screen_x = screen_x;
                    if(render_object_collision.empty() && animation_collision.empty()){
                        std::cout << "Collision objects is empty" << std::endl;
                        exit(-1);
                    }
                    //add the collision objects to the bullet
                    for(auto object: render_object_collision){
                        if(object != nullptr){
                            if(dynamic_cast<PersonClass*>(object)){
                            bullet->addCollisionObject(object); 
                            }
                        }    
                    }
                    for(auto object: animation_collision){
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