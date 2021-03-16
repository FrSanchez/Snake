//
//  Head.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#ifndef Head_hpp
#define Head_hpp

#include "cocos2d.h"

class Head : public cocos2d::Sprite
{
private:
    cocos2d::Vec2 _dir = cocos2d::Vec2(0,1);
    float _speed = 50;
    float _angle = 90;
    float _minDistance = 0.1;
    cocos2d::Vector<cocos2d::Sprite*> bodyParts;
public:
    bool init();
    float getSpeed() { return _speed; }
    cocos2d::Vec2 getDir() { return _dir; }
    void setSpeed(float value) { _speed = value; }
    void setDir(const cocos2d::Vec2 value) { _dir = value; }
    void update(float dt);
    void addBodyPart();
    float getAngle() { return _angle; }
    void setAngle(float value) ;
    CREATE_FUNC(Head);
};

#endif /* Head_hpp */
