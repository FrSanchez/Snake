//
//  WalletSprite.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/9/21.
//

#include "WalletSprite.h"
#include "Bank.h"

USING_NS_CC;

bool WalletSprite::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    initWithSpriteFrameName("labelbg");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto lblSize = getContentSize();

    auto fruit = Sprite::createWithSpriteFrameName("fruit-store");
    fruit->setScale(0.8);
    fruit->setPosition(Vec2(78, lblSize.height/ 2));
    fruit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(fruit);
    
    auto star = Sprite::createWithSpriteFrameName("star");
    star->setScale(0.6);
    star->setPosition(Vec2(lblSize.width * 5/8, lblSize.height/2));
    star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(star);
    
    _food = Bank::getInstance()->getFood();
    _stars = Bank::getInstance()->getStars();
    auto str = StringUtils::format(": %d", _food);
    
    auto label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 50, Size::ZERO, TextHAlignment::LEFT);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Vec2(fruit->getContentSize().width + fruit->getContentSize().width / 2, lblSize.height / 2 + 10));
    label->setTextColor(Color4B::ORANGE);
    label->enableShadow();
    label->setTag(0x80);
    addChild(label);
    
    str = StringUtils::format(": %d", _stars);
    label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 50, Size::ZERO, TextHAlignment::LEFT);
    label->setPosition(lblSize.width * 5/8 + star->getContentSize().width / 3 , lblSize.height/2 + 10);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setTextColor(Color4B::ORANGE);
    label->enableShadow();
    label->setTag(0x81);
    addChild(label);

    return true;
}

void WalletSprite::deductFood(int qty, std::function<void()> onDone)
{
    deductHelper(qty, _food, 0x80, "food", onDone);
    _food -= qty;
}

void WalletSprite::deductStars(int qty, std::function<void()> onDone)
{
    deductHelper(qty, _stars, 0x81, "stars", onDone);
    _stars -= qty;
}

void WalletSprite::deductHelper(int qty, int startValue, int tagId, const std::string &key, std::function<void()> onDone)
{
    auto label = (Label*)getChildByTag(tagId);
    if (onDone != nullptr) {
        scheduleOnce([&, onDone](float dt){ onDone(); }, 0.01 * qty, "callback");
    }
    if (label == nullptr) {
        return ;
    }
    int *d = new int(1);
    d[0] = startValue;
    label->setUserData(d);
    schedule([&, label](float dt) {
        int *d = (int *)label->getUserData();
        auto str = StringUtils::format(": %d", *d);
        label->setString(str.c_str());
        *d = *d - 1;
    }, 0.01, qty, 0, key);
}
