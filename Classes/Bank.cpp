//
//  Bank.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "Bank.h"

USING_NS_CC;

Bank::Bank() : _bouncyPowerup(0),
_food(0),
_dirty(true)
{
    load();
}

Bank::~Bank()
{
    if (_dirty)
    {
        save();
    }
}

void Bank::save()
{
    _dirty = false;
    Data data;
    data.copy((unsigned char*) this, sizeof(this));
    UserDefault::getInstance()->setDataForKey("bank", data);
}

void Bank::load()
{
    Data data = UserDefault::getInstance()->getDataForKey("bank");
    if (!data.isNull() && data.getSize() > 0 ) {
        memcpy(this, data.getBytes(), data.getSize());
    }
    _dirty = false;
}
