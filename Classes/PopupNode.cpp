//
//  PopupNode.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/13/21.
//

#include "PopupNode.h"

USING_NS_CC;

bool PopupNode::init()
{
    if ( !Sprite::initWithSpriteFrameName(getBackgroundFrameName()) )
    {
        return false;
    }
    BlockPassingTouch();
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    setPosition(Vec2(size.width/2, size.height/2) + origin);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

std::string PopupNode::getBackgroundFrameName()
{
    return "popup";
}

void PopupNode::BlockPassingTouch()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event){
        if(event->getCurrentTarget()->isVisible() == false )
            return false;
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PopupNode::ClosePopup()
{
    this->removeFromParentAndCleanup(false);
}
