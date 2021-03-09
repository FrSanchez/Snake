//
//  Bank.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef Bank_hpp
#define Bank_hpp

#include "cocos2d.h"

#define ALTER(__variable__) bool alter__variable__(int qty) { \
int newValue = __variable__ + qty; \
if (newValue < 0) { return false;} \
__variable__ = newValue; \
return true; \
    }

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

class Bank {
private:
    int _bouncyPowerup;
    int _food;
    int _stars;
    bool _dirty;
    
    void load();
    
public:
    Bank();
    ~Bank();
    int getFood() { return _food; }
    int getBouncyPW() { return _bouncyPowerup; }
    int getStars() { return _stars; }

    CREATE_ALTER(food);
    CREATE_ALTER(bouncyPowerup);
    CREATE_ALTER(stars);
    void save();
};

#endif /* Bank_hpp */
