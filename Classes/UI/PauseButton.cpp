//
//  PauseButton.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/11/21.
//

#include "PauseButton.h"

USING_NS_CC;

using namespace ui;

bool PauseButton::init()
{
    if (!Node::init())
    {
        return false;
    }
    _pause = Button::create("pause", "pause-press", "pause", cocos2d::ui::Button::TextureResType::PLIST);
    _pause->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pause->setPosition(Vec2(_pause->getContentSize().width / 2, _pause->getContentSize().height / 2));
    
    _resume =Button::create("play", "play-press", "rewind", cocos2d::ui::Button::TextureResType::PLIST);
    _resume->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _resume->setPosition(Vec2(_pause->getContentSize().width / 2, _pause->getContentSize().height / 2));
    
    _pause->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                _pause->setVisible(false);
                _resume->setVisible(true);
                Director::getInstance()->pause();
                break;
            default:
                break;
        }
    });
    
    _resume->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                _pause->setVisible(true);
                _resume->setVisible(false);
                Director::getInstance()->resume();
                break;
            default:
                break;
        }
    });
    
    this->addChild(_pause);
    this->addChild(_resume);
    
    _pause->setVisible(true);
    _resume->setVisible(false);
    
    setContentSize(_pause->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}
