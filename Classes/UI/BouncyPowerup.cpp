//
//  BouncyPowerup.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/6/21.
//

#include "BouncyPowerup.h"

USING_NS_CC;

bool BouncyPowerup::init()
{
    if (!PopupNode::init())
    {
        return false;
    }
    
    _onCancelCallback = nullptr;
    auto dpad = new DPad();
    dpad->setPosition(Vec2(_contentSize.width / 2 - dpad->getContentSize().width / 2, _contentSize.height / 2 - 64));
    dpad->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dpad->setCallback(CC_CALLBACK_1(BouncyPowerup::onDpad, this));
    addChild(dpad);
    
    auto label = Label::createWithTTF("Select new direction", "Stick-Regular.ttf", 32);
    label->setPosition(Vec2(_contentSize.width/2, _contentSize.height - 32));
    addChild(label);
    
    label = Label::createWithTTF("Just Crash", "Stick-Regular.ttf", 32);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto item = MenuItemLabel::create(label);
    item->setTag(0x21);
    
    auto menu = Menu::create(item, nullptr );
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Vec2(_contentSize.width/2 , 24));
    menu->setTag(0x21);
    addChild(menu);

    return true;
}

void BouncyPowerup::onDpad(int dir)
{
    if (_onSelectionCallback != nullptr) {
        _onSelectionCallback(dir);
    }
}

void BouncyPowerup::setOnCancel(const cocos2d::ccMenuCallback &callback)
{
    auto item = (MenuItem*)getChildByTag(0x21)->getChildByTag(0x21);
    item->setCallback(callback);
}
