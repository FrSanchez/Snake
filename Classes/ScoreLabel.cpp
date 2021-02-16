//
//  ScoreLabel.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/8/21.
//

#include "ScoreLabel.h"

USING_NS_CC;

Label* ScoreLabel::createHS()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = ScoreLabel::_create(Vec2(64, size.height - 64), Vec2::ANCHOR_MIDDLE_LEFT, TextHAlignment::LEFT, "SCORE: 0");
    label->setTag(LABEL_HIGH_SCORE);
    return label;
}

Label* ScoreLabel::createPct()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = ScoreLabel::_create(Vec2(64, size.height - 128), Vec2::ANCHOR_MIDDLE_LEFT, TextHAlignment::LEFT, "FOOD: 0%");
    label->setTag(LABEL_PERCENTAGE);
    return label;
}

Label* ScoreLabel::_create(Vec2 pos, Vec2 anchorPoint, TextHAlignment alignment, const std::string& text)
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithTTF(text, "fonts/Arcade.ttf", 64, Size::ZERO, alignment);
    label->setAnchorPoint(anchorPoint);
    label->setPosition(pos);
    label->setTextColor(Color4B::WHITE);
    label->enableOutline(Color4B::BLACK, 1);
    return label;
}
