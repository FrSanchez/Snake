//
//  Store.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "Store.h"
#include "SelfSavingClass.h"

Store::Store()
{
    _items.clear();
    _items.push_back(StoreItem(0, 25, "bouncy", "Bouncy PowerUp"));
    _items.push_back(StoreItem(1, 75, "extrabouncy", "Bouncy PowerUp 4pk"));
}

PurchaseResult Store::purchase(int itemId)
{
    PurchaseResult result;
    result.success = false;
    SelfSavingClass<Bank> bank;
    auto item = _items.at(itemId);
    if (bank.getData()->getFood() >= item.price)
    {
        if (!bank.getData()->alterfood(-item.price)) {
            CCLOG("Not enough funds!");
            return result;
        }
        switch(item.id) {
            case 0:
                result.success = bank.getData()->alterbouncyPowerup(1);
                if (result.success) {
                    result.paid = item.price;
                    result.purchased = 1;
                }
                break;
            case 1:
                result.success = bank.getData()->alterbouncyPowerup(4);
                if (result.success) {
                    result.paid = item.price;
                    result.purchased = 4;
                }
        }
        bank.save();
        return result;
    }
    CCLOG("Not enough funds!");
    return result;
}
