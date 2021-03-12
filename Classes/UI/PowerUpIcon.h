//
//  PowerUpIcon.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#ifndef PowerUpIcon_hpp
#define PowerUpIcon_hpp

#include "cocos2d.h"

#define HELMET_TAG 0xB1

class PowerUpIcon : public cocos2d::Sprite
{
private:
    int _count = 0;
public:
    bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
    void setCount(int count);
    int getCount() { return _count; }
    static PowerUpIcon* createWithImage(const std::string &name);
};

#endif /* PowerUpIcon_hpp */
