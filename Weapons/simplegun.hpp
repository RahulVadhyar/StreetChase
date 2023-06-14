class SimpleGun: public BaseWeaponClass{
    public:
        SimpleGun(Shader* input_gun_shader):
        BaseWeaponClass::BaseWeaponClass(input_gun_shader, GUN_TEX_DIR, 10){
            max_ammo = 10;
            weapon_health = 1.0;
            mags = 10;
            bullet_offset_x = -0.02;
            bullet_offset_y = 0.03;
        }
        BaseBulletClass* fire(float input_x, float input_y, float direction){
            if(current_ammo > 0){
                current_ammo -= 1;
                auto bullet_shader = new Shader(VS_SHADER_DIR, FS_SHADER_DIR);
                BaseBulletClass* bullet = new SimpleBullet(input_x, input_y, bullet_shader);
                bullet->fire(direction);
                return bullet;
            }
            else{
                return nullptr;
            }
        }
};