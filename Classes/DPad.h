//
//  DPad.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/8/21.
//

#ifndef DPad_hpp
#define DPad_hpp

#include "cocos2d.h"

typedef std::function<void(int direction)> padCallback;

class CC_DLL DPad : public cocos2d::Node
{
public:
    DPad() ;
    void dpadCallback(cocos2d::Ref* sender);
    void setCallback(const padCallback& callback);
    void setPosition(const cocos2d::Vec2 &position);
    
protected:
    padCallback _callback = nullptr;
};

#endif /* DPad_hpp */
