//
//  Snake.cpp
//  Snake
//
//  Created by Francisco Sanchez on 2/1/21.
//

#include "Snake.h"

USING_NS_CC;

Snake::Snake(int w, int l)
{
    width = w;
    height = l;
    
    int oX = 1;
    int oY = l / 2;
    
    body.clear();
    body.push_back(Vec2(oX, oY++));
    body.push_back(Vec2(oX, oY++));
    body.push_back(Vec2(oX, oY++));
    body.push_back(Vec2(oX, oY++));
    
    direction = Vec2(0, -1);
}

void Snake::grow()
{
    Vec2 last = body.back();
    body.push_back(last);
}

int Snake::getLength()
{
    return (int) body.size();
}

void Snake::setLayer(TMXLayer* value)
{
    layer = value;
}

bool Snake::advance()
{
    Point head = body.at(0) + direction;
    Vec2 headPos = head; //Vec2(head.x, height - head.y);
    auto gid = layer->getTileGIDAt(headPos);
    if (gid != SPACE_BLOCK) {
        CCLOG("(CRASH) Headpos: %.1f %.1f gid: %d", headPos.x, headPos.y, gid);
        return false;
    }
    Point temp = head;
    Point old;
    auto total = body.size();
       for(auto i=0; i < total; i++)
       {
           old = body.at(i);
           body.at(i) = temp;
           temp = old;
           if (temp == head) {
               return false;
           }
       }
    return true;
}

Vec2 Snake::getPosAt(int at)
{
    return body.at(at);
}

//void Snake::turnLeft()
//{
//    if (direction.x == 0) {
//        direction.x = -direction.y;
//        direction.y = 0;
//    } else {
//        direction.y = direction.x;
//        direction.x = 0;
//    }
//}
//
//void Snake::turnRight()
//{
//    if (direction.x == 0) {
//        direction.x = direction.y;
//        direction.y = 0;
//    } else {
//        direction.y = - direction.x;
//        direction.x = 0;
//    }
//}


void Snake::turnRight()
{
    if (direction.x == 0) {
        direction.y = 0;
        direction.x = 1;
    }
}

void Snake::turnLeft()
{
    if (direction.x == 0) {
        direction.y = 0;
        direction.x = -1;
    }
}

void Snake::turnDown()
{
    if (direction.y == 0) {
        direction.x = 0;
        direction.y = 1;
    }
}

void Snake::turnUp()
{
    if (direction.y == 0) {
        direction.x = 0;
        direction.y = -1;
    }
}
