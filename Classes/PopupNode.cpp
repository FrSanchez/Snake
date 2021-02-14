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
    if ( !Node::init() )
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    BlockPassingTouch(); //CreateInvisibleBackgroundButton();
    SetupUI();
    //////////////////////////////////////////////////////////////////////////
    return true;
}

void PopupNode::SetupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //////////////////////////////////////////////////////////////////////////
    auto cover = Sprite::create("background.png");
    cover->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(cover);
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
