//
//  LevelOpened.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/15/21.
//

#include "LevelOpened.h"

USING_NS_CC;

LevelOpened::LevelOpened()
{
    auto size = Director::getInstance()->getVisibleSize();
    _pos0 = Vec2(size.width/2, size.height/2);
    _posEnd = Vec2(size.width/2, size.height / 2 + 240);
    setVisible(false);
    setTag(LEVEL_OPENED_TAG);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    setTextColor(Color4B::RED);
    enableOutline(Color4B::YELLOW, 4);
    enableShadow(Color4B::GREEN);
}

void LevelOpened::display()
{
    setVisible(true);
    setPosition(_pos0);
    auto action = Spawn::createWithTwoActions(Blink::create(1, 4), MoveTo::create(1, _posEnd));
    auto seq = Sequence::create(action, Hide::create(), nullptr);
    runAction(seq);
}

Label* LevelOpened::create(int level)
{
    auto score_str = StringUtils::format("LEVEL %d OPENED", level );
    auto ret = new (std::nothrow) LevelOpened();

    if (ret && ret->initWithTTF(score_str, "Arcade.ttf", 64))
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

