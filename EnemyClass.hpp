class EnemyClass : public PersonClass{
    public:
        HealthBarClass health_bar = NULL;
        EnemyClass(float input_x, float input_y, float width, float height) :
        PersonClass(input_x, input_y, width, height){
            //we need player_ptr so that we can deal damage to the player
            PersonClass* player_ptr = dynamic_cast<PersonClass*>(this);
            health_bar = HealthBarClass(player_ptr);
        }
    
    public:
        void update() override{
            //update the physics, collisions, weapons and bullets and draw healthbar
            PhysicsObject::update();
            enemyDebug("physics updated");
            updateCollisions();
            enemyDebug("collisions updated");
            AnimationClass::x = PhysicsObject::x;
            AnimationClass::y = PhysicsObject::y;
            //update the weapons
            if(weapon != nullptr) weapon->update(AnimationClass::x, AnimationClass::y);
            // regen();
            updateBullets();
            enemyDebug("bullets updated");
            enemyDebug("health bar drawn");
        }

    private:
        void enemyDebug(std::string message){
            #ifdef ENEMY_DEBUG
            std::cout << "[ENEMY_DEBUG]::[" << this << "] " << message << std::endl;
            #endif
        }
};