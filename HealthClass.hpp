class Health{
    public:
        float max_health;
        float current_health;
        float regen_rate;
        bool is_dead;
    
    Health(float input_max_health, float input_regen_rate){
        max_health = input_max_health;
        current_health = input_max_health;
        regen_rate = input_regen_rate;
        is_dead = false;
    }

    void takeDamage(float damage){
        std::cout << "taking damage2" << std::endl;
        this->current_health -= damage;
        std::cout << current_health << " " << damage <<std::endl;
        if(current_health <= 0){
            is_dead = true;
        }
    }
    void regen(){
        current_health += regen_rate;
        if(current_health > max_health){
            current_health = max_health;
        }
    }
};