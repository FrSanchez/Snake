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
    
    int oX = w / 2;
    int oY = l * 2/3;

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
    return body.size();
}

void Snake::setLayer(TMXLayer* value)
{
    layer = value;
}

bool Snake::advance()
{
    Point head = body.at(0) + direction;
    auto gid = layer->getTileGIDAt(Vec2(head.x, head.y));
    if (gid != 5) {
        log("head: %f %f, gid %d", head.x, head.y, gid);
        // only tile 5 is a valid empty spot
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

void Snake::turnLeft()
{
    if (direction.x == 0) {
        direction.x = -direction.y;
        direction.y = 0;
    } else {
        direction.y = direction.x;
        direction.x = 0;
    }
}

void Snake::turnRight()
{
    if (direction.x == 0) {
        direction.x = direction.y;
        direction.y = 0;
    } else {
        direction.y = - direction.x;
        direction.x = 0;
    }
}
