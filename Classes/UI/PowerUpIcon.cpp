//
//  PowerUpIcon.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#include "PowerUpIcon.h"
#include "Bank.h"

USING_NS_CC;

PowerUpIcon* PowerUpIcon::createWithImage(const std::string &name)
{
    PowerUpIcon *pRet = new(std::nothrow) PowerUpIcon();
    if (pRet && pRet->initWithSpriteFrameName(name))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool PowerUpIcon::initWithSpriteFrame(SpriteFrame *spriteFrame)
{
//    static Sprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    if (!Sprite::initWithSpriteFrame(spriteFrame)) {
        return false;
    }
    SelfSavingClass<Bank> bank;
    _count = bank.getData()->getBouncyPW();
    std::string str = StringUtils::format("%d", _count);
    auto  label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 40 );
    label->setPosition(Vec2(getContentSize().width/ 2, 0));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setAlignment(TextHAlignment::CENTER);
    label->setTextColor(Color4B::RED);
    label->setTag(0x11);
    addChild(label);
    
    return true;
}

void PowerUpIcon::setCount(int count)
{
    int delta = count - _count;
    if (delta > 1) {
        scheduleOnce([&,count](float dt) {
            setCount(count);
        }, 0.3, "setCount");
    }
    if (delta < 1) { return; }
    this->_count++;
    auto label = (Label*)getChildByTag(0x11);
    if (label != nullptr)
    {
        std::string str = StringUtils::format("%d", _count);
        label->setString(str);
        stopAllActions();
        auto rot1 = RotateTo::create(0.1, 5.0);
        auto rot2 = RotateTo::create(0.1, -5.0);
        auto rot3 = RotateTo::create(0.1, 0.0);
        auto act = Sequence::create(rot1,rot2,rot1,rot2,rot3,NULL);
        runAction(act);
    }
}
