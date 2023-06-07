class SimpleBullet: public BaseBulletClass{
    public:
        SimpleBullet(float x, float y, Shader *shader):
        BaseBulletClass::BaseBulletClass(x, y, shader, BULLET_TEX_DIR){
            damage = 0.5;
            max_speed = 0.1;
            current_speed = 0;
            height = 0.1;
            width = 0.1;
            initial_velocity = 0.1;
        }
};