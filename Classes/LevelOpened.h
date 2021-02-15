//
//  LevelOpened.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/15/21.
//

#ifndef LevelOpened_hpp
#define LevelOpened_hpp

#include "cocos2d.h"

#define LEVEL_OPENED_TAG 0x21

class CC_DLL LevelOpened : public cocos2d::Label
{
private:
    cocos2d::Vec2 _pos0;
    cocos2d::Vec2 _posEnd;
public:
    LevelOpened();
    static Label* create(int level);
    void display();
};

#endif /* LevelOpened_hpp */
