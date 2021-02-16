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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    menu = Menu::create( );
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - 80));
    
    auto label = Label::createWithTTF("", "fonts/arial.ttf", 30);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::GRAY);
    label->enableOutline(Color4B::BLACK, 1);
    label->setTag(0x11);
    label->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + 50));
    auto cover = Sprite::create("background.png");
    label->setDimensions(cover->getContentSize().width - 200, 0);

    addChild(menu, 1);
    addChild(label, 1);
    return true;
}

void ModalMessageBox::setString(const std::string& text)  {
    Label* label = (Label*)getChildByTag(0x11);
    label->setString(text);
}

void ModalMessageBox::addButton(const std::string& text, float fontSize, const ccMenuCallback& callback) {
    
    auto label = Label::createWithTTF(text, "fonts/arial.ttf", fontSize);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::GRAY);
    label->enableOutline(Color4B::BLACK, 1);
    
    auto item = MenuItemLabel::create(label, callback);
    item->setTag((int)menu->getChildrenCount());
    menu->addChild(item);
    menu->alignItemsHorizontallyWithPadding(20);
}
