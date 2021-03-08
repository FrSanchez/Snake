//
//  TrophyModalBox.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/6/21.
//

#include "TrophyModalBox.h"

USING_NS_CC;

bool TrophyModalBox::init()
{
    if (!PopupNode::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rays = Sprite::createWithSpriteFrameName("rays");
    rays->setPosition(Vec2(_contentSize.width / 2, _contentSize.height - 32));
    rays->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rays->setLocalZOrder(-1);
    addChild(rays);
    
    auto rot1 = RotateBy::create(30, 360*2.0f);
    auto rot2 = rot1->reverse();
    auto seq3_2 = Sequence::create( rot1, rot2, nullptr);
    rays->runAction( RepeatForever::create(seq3_2));
    
    menu = Menu::create( );
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Vec2(_contentSize.width/4 , _contentSize.height/3 ));
    addChild(menu);
    
    auto trophy = Sprite::createWithSpriteFrameName("trophy");
    trophy->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2 - 50));
    trophy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(trophy);
    
    TTFConfig ttfConfig("Marker Felt.ttf", 50);
    auto label = Label::createWithTTF(ttfConfig, "", TextHAlignment::CENTER, getContentSize().width * 0.8);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::RED);
    label->enableOutline(Color4B::BLACK, 1);
    label->setTag(0x11);
    label->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2 + 10));
    addChild(label);
    
    for (int i = 0; i < 3; i++) {
        auto star = Sprite::createWithSpriteFrameName("star_p");
        star->setPosition(Vec2(_contentSize.width / 2 + offX[i], _contentSize.height / 2 + offY[i]));
        addChild(star);
    }
    return true;
}

void TrophyModalBox::addStar(int starNum)
{
    if (starNum >= 0 & starNum < 3) {
        scheduleOnce([&, starNum](float dt) {
                auto star = Sprite::createWithSpriteFrameName("star");
                star->setPosition(Vec2(_contentSize.width / 2 + offX[starNum], _contentSize.height / 2 + offY[starNum]));
                star->setScale(5);
                star->runAction(ScaleTo::create(0.5, 1));
                addChild(star);
        }, starNum * 0.5, StringUtils::format("star%d", starNum).c_str());
    }
}

void TrophyModalBox::setLevel(int level)
{
    auto label = static_cast<Label*>(getChildByTag(0x11));
    label->setString(StringUtils::format("%d", level));
    Score score = Score();
    auto stars = score.getStars(level);
    if (stars & 0x1) {
        addStar(0);
    }
    if (stars & 0x2) {
        addStar(1);
    }
    if (stars & 0x4) {
        addStar(2);
    }
}

void TrophyModalBox::addButton(const std::string& text, float fontSize, const ccMenuCallback& callback) {
    
    auto label = Label::createWithTTF(text, "Stick-Regular.ttf", fontSize);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::GRAY);
    label->enableOutline(Color4B::BLACK, 1);
    
    auto item = MenuItemLabel::create(label, callback);
    item->setTag((int)menu->getChildrenCount());
    menu->addChild(item);
    menu->alignItemsHorizontallyWithPadding(20);
}
