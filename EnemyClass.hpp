class EnemyClass : public PersonClass{
    public:
    HealthBarClass health_bar = NULL;
    EnemyClass(float input_x, float input_y, float width, float height, Shader *o_shader, std::string texture_dir) :
    PersonClass(input_x, input_y, width, height, o_shader, texture_dir){
        PersonClass* player_ptr = dynamic_cast<PersonClass*>(this);
        health_bar = HealthBarClass(player_ptr);
    }
    
    void update() override{
        PhysicsObject::update();
        updateCollisions();
        //update the weapons
        if(weapon != nullptr) weapon->update(x, y);
        // regen();
        updateBullets();
        health_bar.draw();
    }
};