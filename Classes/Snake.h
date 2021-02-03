//
//  Snake.hpp
//  Snake
//
//  Created by Francisco Sanchez on 2/1/21.
//

#ifndef Snake_hpp
#define Snake_hpp

#include "cocos2d.h"

class Snake
{
public:
    Snake(int, int);
    
    bool advance();
    void turnLeft();
    void turnRight();
    int getLength();
    cocos2d::Vec2 getPosAt(int);
    void grow();
    int getWidth() { return width; }
    int getHeight() { return height; }
    void setLayer(cocos2d::TMXLayer*);
    
private:
    std::vector<cocos2d::Point> body;
    cocos2d::Vec2 direction;
    int width;
    int height;
    cocos2d::TMXLayer* layer;
};

#endif /* Snake_hpp */
