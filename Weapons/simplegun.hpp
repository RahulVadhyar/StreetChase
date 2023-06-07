class SimpleGun: public BaseWeaponClass{
    public:
        SimpleGun(Shader* input_gun_shader, Shader* input_bullet_shader):
        BaseWeaponClass::BaseWeaponClass(input_gun_shader, GUN_TEX_DIR ,input_bullet_shader, BULLET_TEX_DIR, 10){
            firing_rate = 0.5;
            firing_speed = 0.01;
            max_ammo = 10;
            reload_time = 1.0;
            weapon_health = 1.0;
            mags = 10;
            bullet_offset_x = 0.02;
            bullet_offset_y = 0.03;
        }
        BaseBulletClass* fire(float input_x, float input_y){
            if(current_ammo > 0){
                current_ammo -= 1;
                auto s = new Shader(VS_SHADER_DIR, FS_SHADER_DIR);
                #ifdef GCC
                std::cout << input_x << std::endl; //for GCC only
                #endif
                BaseBulletClass* bullet = new SimpleBullet(input_x, input_y, s);
                bullet->x = input_x + offset_x + bullet_offset_x;
                bullet->y = input_y + offset_y + bullet_offset_y;
                bullet->fire();
                return bullet;
                
            }
            else{
                return nullptr;
            }
        }
};