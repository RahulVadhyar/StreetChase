class SimpleBullet: public BaseBulletClass{
    public:
        SimpleBullet(float x, float y):
        BaseBulletClass::BaseBulletClass(x, y, 0.03, 0.03, BULLET_TEX_DIR){
            damage = 0.3;
            initial_velocity = 0.05;
        }

};