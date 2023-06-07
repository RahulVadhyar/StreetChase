class BaseWeaponClass{
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
    BaseWeaponClass(Shader* input_bullet_shader, std::string input_bullet_texture_dir, int input_current_ammo){
        bullet_shader = input_bullet_shader;
        bullet_texture_dir = input_bullet_texture_dir;
        current_ammo = input_current_ammo;
    }
    virtual BaseBulletClass* fire(float x, float y){
        if(current_ammo > 0){
            current_ammo -= 1;
            BaseBulletClass* bullet = new BaseBulletClass(x, y, bullet_shader, bullet_texture_dir);
            bullet->fire();
            return bullet;
        }
        else{
            return nullptr;
        }
    }
    void reload(){
        if(mags > 0){
            mags -= 1;
            current_ammo = max_ammo;
        }
    };
    void take_damage(float damage){
        weapon_health -= damage;
    }
};