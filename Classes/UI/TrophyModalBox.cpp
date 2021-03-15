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
    rays->setTag(0xf00);
    rays->setVisible(false);
    addChild(rays);
    
    auto rot1 = RotateBy::create(30, 360*2.0f);
    auto rot2 = rot1->reverse();
    auto seq3_2 = Sequence::create( rot1, rot2, nullptr);
    rays->runAction( RepeatForever::create(seq3_2));
    
    menu = Menu::create( );
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Vec2(_contentSize.width/2 , _contentSize.height/2 ));
    addChild(menu);
    
    auto trophy = Sprite::createWithSpriteFrameName("trophy");
    trophy->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2 + 32));
    trophy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(trophy);
    
    // level label on trophy
//    TTFConfig ttfConfig("Marker Felt.ttf", 50);
//    auto label = Label::createWithTTF(ttfConfig, "", TextHAlignment::CENTER, getContentSize().width * 0.8);
//    label->setAnchorPoint(Vec2(0.5, 0.5));
//    label->setTextColor(Color4B::RED);
//    label->enableOutline(Color4B::BLACK, 1);
//    label->setTag(0x11);
//    label->setPosition(Vec2(_contentSize.width/2, _contentSize.height/2 + 30));
//    addChild(label);
    
    setScale(0.5);
    runAction(ScaleTo::create(0.2, 1));
    return true;
}

void TrophyModalBox::addStar(int starNum)
{
    if (starNum >= 0 & starNum < 3) {
        auto rays = (Sprite*)getChildByTag(0xf00);
        rays->setVisible(true);
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
    _level = level;
//    auto label = static_cast<Label*>(getChildByTag(0x11));
//    label->setString(StringUtils::format("%d", _level));
}

void TrophyModalBox::addButton(const std::string& normal, const std::string &pressed, float fontSize, const ccMenuCallback& callback) {
    auto b = Sprite::createWithSpriteFrameName(normal);
    auto bp = Sprite::createWithSpriteFrameName(pressed);
    auto item = MenuItemSprite::create(b, bp, callback);
    if (fontSize < 10) {
        item->setVisible(false);
    }
    menu->addChild(item);
    menu->alignItemsHorizontally();
}

void TrophyModalBox::addFood(int qty)
{
    if (qty > 0) {
        Bank::getInstance()->alterfood(qty);
        auto label = Label::createWithTTF(StringUtils::format("Food earned: %d", qty).c_str(), "Stick-Regular.ttf", 32);
        label->setTextColor(Color4B::WHITE);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(_contentSize.width/2, 36));
        addChild(label);
    }
}

void TrophyModalBox::showPct(float dt)
{
    Node::update(dt);
    auto pct = (Label*)getChildByTag(0xa01)->getChildren().front();
    float top = (float)_eaten / (float)_target;
    if (pct != nullptr && _pctValue < top) {
        _pctValue += dt / 2;
        _pctValue = clampf(_pctValue, 0.25, top);
        auto str = StringUtils::format("%.0f%%", _pctValue * 100);
        pct->setString(str.c_str());
    }
}

void TrophyModalBox::start()
{
    auto label = Label::createWithTTF("ACCURACY: ", "Stick-Regular.ttf", 32);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setTag(0xa01);
    addChild(label);

    _pctValue = MIN(0.25, (float)_eaten / _target);
    auto str = StringUtils::format("%.0f%%", _pctValue * 100);

    auto pct = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 32);
    pct->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    pct->setPosition(Vec2(label->getContentSize().width, 0));
    label->setPosition(Vec2(_contentSize.width / 2 - pct->getContentSize().width / 2, _contentSize.height / 2 - 120));
    label->addChild(pct);
    auto stars = _score.getStars(_level);
    addStar(0);
    schedule((SEL_SCHEDULE)&TrophyModalBox::showPct);

    if (stars & 0x2) {
        addStar(1);
    }
    if (stars & 0x4) {
        addStar(2);
    }
}
