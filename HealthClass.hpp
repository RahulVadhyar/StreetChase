//Handles health
class Health{
    public:
        float max_health = 1.0f;
        float current_health = 1.0f;
        float regen_rate = 4;
        bool is_dead = false;
        float last_time = 0;
    
        Health(float input_max_health, float input_regen_rate){
            max_health = input_max_health;
            current_health = input_max_health;
            regen_rate = input_regen_rate;
            is_dead = false;
            float last_time = std::clock();
            healthDebug("Health initalized");
        }

    public:
        void takeDamage(float damage){
            //take damage and check if the player is dead
            this->current_health -= damage;
            if(current_health <= 0){
                is_dead = true;
            }
            healthDebug("Damage " + std::to_string(damage) + " taken");
        }

    public:
        void regen(){
            //regenerate health based on time passed
            if(std::clock() - last_time >= regen_rate){
                current_health += regen_rate;
                if(current_health > max_health){
                    current_health = max_health;
                }
                last_time = std::clock();
            }
        }
        
    private:
    void healthDebug(std::string message){
        #ifdef HEALTH_DEBUG
        std::cout << "[HEALTH_DEBUG]::[" << this << "] " << message << std::endl;
        #endif
    }
};