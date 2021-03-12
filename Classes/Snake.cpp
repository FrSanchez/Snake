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

bool Snake::canAdvance()
{
    Point head = body.at(0) + direction;
    auto gid = layer->getTileGIDAt(head);
    if (gid != SPACE_BLOCK) {
        return false;
    }
    for(auto &link : body) {
        if (link == head) {
            return false;
        }
    }
    return true;
}

bool Snake::advance()
{
    Point head = body.at(0) + direction;
    if (!canAdvance()) {
//    auto gid = layer->getTileGIDAt(headPos);
//    if (gid != SPACE_BLOCK) {
        direction.x = 0;
        direction.y = 0;
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

bool Snake::isOccupying(cocos2d::Vec2 position)
{
    for(int i = 0 ; i < body.size(); i++) {
        if (position == body.at(i)) {
            return true;
        }
    }
    return false;
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
//    if (direction.x == 0)
    {
        direction.y = 0;
        direction.x = 1;
    }
}

void Snake::turnLeft()
{
//    if (direction.x == 0)
    {
        direction.y = 0;
        direction.x = -1;
    }
}

void Snake::turnDown()
{
//    if (direction.y == 0)
    {
        direction.x = 0;
        direction.y = 1;
    }
}

void Snake::turnUp()
{
//    if (direction.y == 0)
    {
        direction.x = 0;
        direction.y = -1;
    }
}
