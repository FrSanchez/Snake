//
//  Fruit.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#ifndef Fruit_hpp
#define Fruit_hpp

#include <cocos2d.h>

class Fruit : public cocos2d::Sprite
{
    
public:
    /**
            How many seconds this will last
     */
    void setDuration(float duration);
    void removeFruit(float dt = 0);
    bool init();
    CREATE_FUNC(Fruit);
    
    void setParent(cocos2d::Node * parent);
};

#endif /* Fruit_hpp */
