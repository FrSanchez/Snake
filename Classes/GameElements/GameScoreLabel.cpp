//
//  ScoreLabel.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/19/21.
//

#include "GameScoreLabel.h"

USING_NS_CC;

bool GameScoreLabel::init()
{
    if (!initWithTTF("SCORE: ", "Arcade.ttf", 64, Size::ZERO, TextHAlignment::LEFT))
    {
        return false;
    }
    auto size = Director::getInstance()->getVisibleSize();
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    setPosition(Vec2(0, size.height - getContentSize().height));
    // Analogous colors - Ground: #c0f228
    setTextColor ( Color4B(0xF2, 0xBF, 0x28, 0xFF) );
    setValue(0);
    setTag(SCORE_TAG);
    return true;
}

void GameScoreLabel::setValue(int newValue)
{
    _value = newValue;
    auto score_str = StringUtils::format("%d", _value );
    if (getChildrenCount() == 0) {
        auto text = Label::createWithTTF(score_str, "Arcade.ttf", 64, Size::ZERO, TextHAlignment::LEFT);
        text->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        text->setPosition(Vec2(getContentSize().width, 0));
        text->setTextColor ( Color4B(0xF2, 0xBF, 0x28, 0xFF) );
        addChild(text);
    } else {
        auto text = (Label*)getChildren().front();
        text->setString(score_str);
    }
}
