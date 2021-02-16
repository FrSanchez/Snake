//
//  Timer.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/15/21.
//

#include "TimerSprite.h"


USING_NS_CC;

TimerSprite::TimerSprite() : _callback(nullptr)
{
    
}

bool TimerSprite::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto image = Sprite::createWithSpriteFrameName("clock");
    image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    image->setVisible(false);

    auto timer = ProgressTimer::create(image);
    timer->setType(ProgressTimer::Type::RADIAL);
    addChild(timer);
    return true;
}

void TimerSprite::activate(float duration)
{
    auto callback = CallFunc::create([=](){
        if (_callback) {
            _callback();
        }
        stop();
    });
    auto timer = static_cast<ProgressTimer*>( getChildren().at(0) );
    auto to = Sequence::create(ProgressTo::create(duration, 100), ProgressTo::create(0, 0), callback, nullptr);
    timer->setVisible(true);
    timer->runAction(to);
}

void TimerSprite::stop()
{
    auto timer = static_cast<ProgressTimer*>( getChildren().at(0) );
    timer->stopAllActions();
    timer->setVisible(false);
}
