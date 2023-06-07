class SimpleBullet: public BaseBulletClass{
    public:
        SimpleBullet(float x, float y, Shader *shader):
        BaseBulletClass::BaseBulletClass(x, y, 0.03, 0.03, shader, BULLET_TEX_DIR){
            damage = 0.5;
            max_speed = 0.1;
            initial_velocity = 0.0015;
        }

};