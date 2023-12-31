#include "animObject.hpp"

AnimObject::AnimObject(int shapeNo, int animObjNo, Shape* shape){
    this->shapeNo = shapeNo;
    this->animObjNo = animObjNo;
    this->shape = shape;
}

void AnimObject::addChild(AnimObject* child){
    children.push_back(child);
    child->setParent(this);
}

void AnimObject::removeChild(AnimObject* child){
    for(int i = 0; i < children.size(); i++){
        if(children[i] == child){
            children.erase(children.begin() + i);
            break;
        }
    }
    child->setParent(nullptr);
}   

int AnimObject::addAnimation(Animation animation){
    int animNo = nextFreeAnimNo;
    nextFreeAnimNo++;
    animations[animNo] = animation;
    return animNo;
}

void AnimObject::removeAnimation(int animNo){
    if(animations.count(animNo) > 0){
        animations.erase(animNo);
    }
}



