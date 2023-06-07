class BaseWeaponClass:public RenderObject{
    public:
        float firing_rate;
        float firing_speed;
        int max_ammo;
        int current_ammo;
        float reload_time;
        float weapon_health;
        float mags;
        Shader* bullet_shader;
        std::string bullet_texture_dir;
        float offset_x;
        float offset_y;
        float bullet_offset_x;
        float bullet_offset_y;
    BaseWeaponClass(Shader* input_gun_shader, std::string input_gun_texture_dir ,Shader* input_bullet_shader, std::string input_bullet_texture_dir, int input_current_ammo) :
    RenderObject::RenderObject(0.1, 0.1, input_gun_shader, input_gun_texture_dir){
        bullet_shader = input_bullet_shader;
        bullet_texture_dir = input_bullet_texture_dir;
        current_ammo = input_current_ammo;
        offset_y = -0.05;
    }
    virtual BaseBulletClass* fire(float x, float y) = 0;
    void reload(){
        if(mags > 0){
            mags -= 1;
            current_ammo = max_ammo;
        }
    };
    void update(float x, float y){
        this->x = x + offset_x;
        this->y = y + offset_y;
    }

    void take_damage(float damage){
        weapon_health -= damage;
    }
};