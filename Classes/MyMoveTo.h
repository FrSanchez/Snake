//
//  MyMoveTo.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#ifndef MyMoveTo_hpp
#define MyMoveTo_hpp

#include "cocos2d.h"

class MyMoveTo : public cocos2d::MoveTo
{
public:
    cocos2d::Vec2 getEndPosition() { return cocos2d::Vec2(_endPosition.x, _endPosition.y); }
};

#endif /* MyMoveTo_hpp */
