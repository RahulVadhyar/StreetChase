#pragma once
#include "shape.hpp"

class AnimObject{
public:
    AnimObject(int shapeNo, int animObjNo, Shape* shape);
    int animObjNo = -1;
    void addChild(AnimObject* child);
    void removeChild(AnimObject* child);
    int addAnimation(Animation animation);
    void removeAnimation(int animNo);
    void setParent(AnimObject* parent);
private:    
    int shapeNo = -1;
    Shape* shape = nullptr;
    AnimObject* parent = nullptr;
    std::vector<AnimObject*> children;

    int nextFreeAnimNo = 0;
    std::map<int, Animation> animations;
    
};