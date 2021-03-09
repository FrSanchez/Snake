//
//  Store.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "Store.h"

Store::Store()
{
    _items.clear();
    _items.push_back(StoreItem(0, 25, "bouncy", "Bouncy PowerUp"));
    _items.push_back(StoreItem(1, 75, "extrabouncy", "Bouncy PowerUp 4pk"));
}

bool Store::purchase(int itemId)
{
    Bank bank;
    auto item = _items.at(itemId);
    if (bank.getFood() >= item.price)
    {
        if (!bank.alterfood(item.price)) {
            CCLOG("Not enough funds!");
            return false;
        }
        switch(item.id) {
            case 0:
                return bank.alterbouncyPowerup(1);
            case 1:
                return bank.alterbouncyPowerup(4);
        }
        return true;
    }
    CCLOG("Not enough funds!");
    return false;
}
