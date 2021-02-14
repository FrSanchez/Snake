//
//  Chooser.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/11/21.
//

#include "Chooser.h"

USING_NS_CC;

bool Chooser::init()
{
    auto numLabel = Label::createWithTTF("1", "fonts/Arcade.ttf", 96);
    numLabel->setTextColor(Color4B::BLUE);
    numLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    auto downNormal = Sprite::createWithSpriteFrameName("LeftNormal");
    auto downSelected = Sprite::createWithSpriteFrameName("LeftSelected");
    auto downItem = MenuItemSprite::create(downNormal, downSelected, CC_CALLBACK_1(Chooser::upDownCallback, this));
    
    auto upNormal = Sprite::createWithSpriteFrameName("RightNormal");
    auto upSelected = Sprite::createWithSpriteFrameName("RightSelected");
    auto upItem = MenuItemSprite::create(upNormal, upSelected, CC_CALLBACK_1(Chooser::upDownCallback, this));
    
    auto menuChoose = Menu::create(downItem, upItem, nullptr);
    menuChoose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    downItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    upItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    upItem->setTag(1);
    downItem->setTag(-1);
    addChild(menuChoose);
    addChild(numLabel);
    upItem->setUserData(numLabel);
    downItem->setUserData(numLabel);
    
    upItem->setPosition(Vec2(100,0));
    downItem->setPosition(Vec2(-100,0));
    menuChoose->setPosition(Vec2::ZERO);
    numLabel->setPosition(Vec2::ZERO);

    return true;
}

void Chooser::upDownCallback(cocos2d::Ref* pSender)
{
    MenuItem *item = static_cast<MenuItem*>(pSender);
    _value += item->getTag();
    if (_value < _minValue)
    {
        _value = _maxValue;
    }
    if (_value > _maxValue)
    {
        _value = _minValue;
    }
    Label* numLabel =  static_cast<Label*>(item->getUserData());
    numLabel->setString(StringUtils::format("%d", _value));
    if (_callback != nullptr) {
        _callback(_value);
    }
}

void Chooser::setMaxValue(int value)
{
    _maxValue = value;
}

Chooser::~Chooser()
{
    
}
