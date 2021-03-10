//
//  Bank.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef Bank_hpp
#define Bank_hpp

#include "SelfSavingClass.h"
#include "cocos2d.h"

#define CREATE_ALTER(__VAR__) \
bool alter##__VAR__(int qty) \
{ \
    int value = _##__VAR__ + qty;  \
    if (value < 0) \
    { \
        return false; \
    } \
    _##__VAR__ = value;\
    return true; \
}

class Bank
{
private:
    int _food;
    int _stars;
    int _bouncyPowerup;
       
public:
    int getFood() { return _food; }
    int getBouncyPW() { return _bouncyPowerup; }
    int getStars() { return _stars; }
    void reset();

    CREATE_ALTER(food);
    CREATE_ALTER(bouncyPowerup);
    CREATE_ALTER(stars);
};

#endif /* Bank_hpp */
