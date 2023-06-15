class BaseWeaponClass:public RenderObject{
    public:
        float firing_speed = 0;
        int max_ammo = 0;
        int current_ammo = 0;
        float weapon_health = 1.0f;
        float mags = 0;
        float offset_x = 0;
        float offset_y = 0;
        float bullet_offset_x = 0;
        float bullet_offset_y = 0;
        BaseWeaponClass(Shader* input_gun_shader, std::string input_gun_texture_dir , int input_current_ammo) :
        RenderObject::RenderObject(0.1, 0.1, input_gun_shader, input_gun_texture_dir){
            current_ammo = input_current_ammo;
            offset_y = -0.05;
        }
    
    public:
        virtual BaseBulletClass* fire(float x, float y, float direction) = 0;
    
    public:
        void reload(){
            if(mags > 0){
                mags -= 1;
                current_ammo = max_ammo;
            }
        };
    
    public:
        void update(float x, float y){
            this->x = x + offset_x;
            this->y = y + offset_y;
        }

    public:
        void take_damage(float damage){
            weapon_health -= damage;
        }
};