//
//  IconButton.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/12/21.
//

#include "IconButton.h"

USING_NS_CC;

using namespace ui;

#define ICON_TAG 0xf00
#define LABEL_TAG 0xf01

IconButton* IconButton::createWithIconAndText(const std::string &image, const std::string &text, const ccMenuCallback& callback)
{
    IconButton *button = new (std::nothrow) IconButton();
    if (button && button->init(image, text, callback))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool IconButton::init(const std::string &image, const std::string &text, const ccMenuCallback& callback)
{
    auto b = Sprite::createWithSpriteFrameName("button");
    auto bp = Sprite::createWithSpriteFrameName("button-pressed");
    if(!MenuItemSprite::initWithNormalSprite(b, bp, nullptr, callback))
    {
        return false;
    }
    setIcon(image);
    setText(text);
    return true;
}

void IconButton::setIcon(const std::string &iconName)
{
    auto icon =(Sprite*) getChildByTag(ICON_TAG);
    if (icon != nullptr) {
        icon->setSpriteFrame(iconName);
    } else {
        icon = Sprite::createWithSpriteFrameName(iconName);
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        icon->setPosition(Vec2(70, getContentSize().height/2));
        icon->setTag(ICON_TAG);
        addChild(icon);
    }
}

void IconButton::setText(const std::string &text)
{
    auto label = (Label*)getChildByTag(LABEL_TAG);
    if (label == nullptr) {
        auto sz = getContentSize().height * 0.375;
        TTFConfig ttfConfig("Stick-Regular.ttf", sz);
        label = Label::createWithTTF(ttfConfig, text, TextHAlignment::CENTER, getContentSize().width * 0.8);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(getContentSize().width / 2 + 25, getContentSize().height / 2));
        label->setTag(LABEL_TAG);
        addChild(label);
    } else {
        label->setString(text);
    }
}
