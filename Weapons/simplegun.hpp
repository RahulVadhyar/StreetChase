class SimpleGun: public BaseWeaponClass{
    public:
        SimpleGun(Shader* input_bullet_shader):BaseWeaponClass::BaseWeaponClass(input_bullet_shader, BULLET_TEX_DIR, 10){
            firing_rate = 0.5;
            firing_speed = 0.01;
            max_ammo = 10;
            reload_time = 1.0;
            weapon_health = 1.0;
            mags = 10;
        }
        BaseBulletClass* fire(float x, float y){
            if(current_ammo > 0){
                current_ammo -= 1;
                BaseBulletClass* bullet = new SimpleBullet(x, y, bullet_shader);
                bullet->fire();
                return bullet;
            }
            else{
                return nullptr;
            }
        }
};