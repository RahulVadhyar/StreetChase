/*
parts of body
-head
-right upper arm
-left upper arm
-right lower arm
-left lower arm
-chest
-right thigh
-left thigh
-right leg
-left leg
-right foot
-left foot

states
-idle
-walking right
-walking left
-jumping
-falling/lying down

THIS WILL NEED TO BE FRAME RATE INDEPENDENT AT ANY COST(NEED TO IMPLEMENT MULTITHREADING)
-Will likely need to refactor the render pipeline in order for this to work
-Will need to implement frame rate independent physics
-Will need to implement frame rate independent animations

-Will need a way to store the animations
*/
class AnimationClass{
    public:
        int state = 0;
};