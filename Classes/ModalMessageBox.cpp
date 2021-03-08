//
//  ModalMessageBox.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/13/21.
//

#include "ModalMessageBox.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool ModalMessageBox::init()
{
    if ( !PopupNode::init() )
    {
        return false;
    }
    Size visibleSize = getContentSize();

    menu = Menu::create( );
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 - 80));
    
    TTFConfig ttfConfig("Stick-Regular.ttf", 30);
    auto label = Label::createWithTTF(ttfConfig, "", TextHAlignment::CENTER, getContentSize().width * 0.8);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::GRAY);
    label->enableOutline(Color4B::BLACK, 1);
    label->setTag(0x11);
    label->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 50));

    addChild(menu);
    addChild(label);
    return true;
}

void ModalMessageBox::setString(const std::string& text)  {
    Label* label = (Label*)getChildByTag(0x11);

    label->setString(text);
}

void ModalMessageBox::addButton(const std::string& text, float fontSize, const ccMenuCallback& callback) {
    
    auto label = Label::createWithTTF(text, "Stick-Regular.ttf", fontSize);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::WHITE);
    label->enableOutline(Color4B::GRAY, 1);
    
    auto item = MenuItemLabel::create(label, callback);
    item->setTag((int)menu->getChildrenCount());
    menu->addChild(item);
    menu->alignItemsHorizontallyWithPadding(20);
}
