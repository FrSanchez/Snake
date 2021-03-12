//
//  Snake.hpp
//  Snake
//
//  Created by Francisco Sanchez on 2/1/21.
//

#ifndef Snake_hpp
#define Snake_hpp

#include "cocos2d.h"

#define SPACE_BLOCK 2

class Snake
{
private:
    std::vector<cocos2d::Point> body;
    cocos2d::Vec2 direction;
    int width;
    int height;
    cocos2d::TMXLayer* layer;

public:
    Snake(int, int);
    
    bool advance();
    bool canAdvance();
    void turnLeft();
    void turnRight();
    void turnUp();
    void turnDown();
    int getLength();
    cocos2d::Vec2 getPosAt(int);
    void grow();
    cocos2d::Vec2 getDirection() { return direction; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    void setLayer(cocos2d::TMXLayer*);
    bool isOccupying(cocos2d::Vec2 position);
};

#endif /* Snake_hpp */
