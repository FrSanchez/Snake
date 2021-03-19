//
//  SnakeHead.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#ifndef SnakeHead_hpp
#define SnakeHead_hpp

#include "cocos2d.h"

class SnakeHead : public cocos2d::Sprite
{
private:
    cocos2d::Vec2 _dir;
    float _speed;
    float _angle;
    float _minDistance;
    float _tLim;
    cocos2d::Vector<cocos2d::Sprite*> bodyParts;
public:
    SnakeHead();
    ~SnakeHead();
    bool init();
    float getSpeed() { return _speed; }
    cocos2d::Vec2 getDir() { return _dir; }
    void setSpeed(float value) { _speed = value; }
    void setDir(const cocos2d::Vec2 value) { _dir = value; }
    void update(float dt);
    void addBodyPart();
    float getAngle() { return _angle; }
    void setAngle(float value) ;
    float getMindDistance() { return _minDistance; }
    void setMinDistance(float value) { _minDistance = value; }
    float getTLim() { return _tLim; }
    void setTLim(float value) { _tLim = value; }
    void die(const std::function<void()>& callback);
    CREATE_FUNC(SnakeHead);
    int getSize() { return (int) bodyParts.size(); }
    void grow(int);
};

#endif /* SnakeHead_hpp */
