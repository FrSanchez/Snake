//
//  Bank.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef Bank_hpp
#define Bank_hpp

#include "BaseAutoSave.h"
#include "cocos2d.h"

#define CREATE_ALTER(__VAR__) \
bool alter##__VAR__(int qty) \
{ \
    int value = _##__VAR__ + qty;  \
    if (value < 0) \
    { \
        return false; \
    } \
    setDirty(); \
    _##__VAR__ = value;\
    save(); \
    return true; \
}

class Bank : public BaseAutoSave
{
private:
    int _food=0;
    int _stars=0;
    int _bouncyPowerup=0;
    Bank();

public:
    int getFood() { return _food; }
    int getBouncyPW() { return _bouncyPowerup; }
    int getStars() { return _stars; }
    
    static Bank* getInstance();

    CREATE_ALTER(food);
    CREATE_ALTER(bouncyPowerup);
    CREATE_ALTER(stars);
};

#endif /* Bank_hpp */
