//
//  LostModalBox.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/13/21.
//

#include "LostModalBox.h"

USING_NS_CC;

bool LostModalBox::init()
{
    if (!PopupNode::init()) {
        return false;
    }
    Size size = Director::getInstance()->getVisibleSize();
    
    auto label = Label::createWithTTF("SORRY!", "Stick-Regular.ttf", 48);
    label->setPosition(Vec2(_contentSize.width/2, _contentSize.height * 2 / 3));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(label);
    
    auto normal = Sprite::createWithSpriteFrameName("home-sm");
    auto pressed = Sprite::createWithSpriteFrameName("home-sm-pressed");
    _home = MenuItemSprite::create(normal, pressed);
    
    normal = Sprite::createWithSpriteFrameName("repeat-normal");
    pressed = Sprite::createWithSpriteFrameName("repeat-bt-pressed");
    _repeat = MenuItemSprite::create(normal, pressed);
    
    auto menu = Menu::create(_home, _repeat, nullptr);
    addChild(menu);
    menu->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 3));
    menu->alignItemsHorizontally();
    
    return true;
}

void LostModalBox::setOnHome(const ccMenuCallback& callback)
{
    _home->setCallback(callback);
}

void LostModalBox::setOnRepeat(const ccMenuCallback& callback)
{
    _repeat->setCallback(callback);
}


