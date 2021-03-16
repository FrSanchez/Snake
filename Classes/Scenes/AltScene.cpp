//
//  AltScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#include "AltScene.h"
#include "Head.h"
#include "SceneMenu.h"

USING_NS_CC;

bool AltScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();
    
    _head = Head::create();
    _head->setPosition(size.width /2 , size.height/2);
    _head->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(_head);
    scheduleUpdate();
    for(int i =0; i < 4; i++) { _head->addBodyPart(); }

    enableSwipe();
    
    auto normal = Sprite::createWithSpriteFrameName("fruit");
    auto select = Sprite::createWithSpriteFrameName("fruit");
    auto eatItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  {
        _head->addBodyPart();
    });
   
    normal = Sprite::createWithSpriteFrameName("exit");
    select = Sprite::createWithSpriteFrameName("exit_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *s) {
        auto scene = SceneMenu::create();
        auto transition = TransitionSlideInR::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    auto menu = Menu::create(eatItem, closeItem, nullptr);
    
    menu->setPosition(Vec2(size.width / 2, 32));
    menu->alignItemsHorizontally();
    this->addChild(menu, 1);

    return true;
}

void AltScene::update(float dt)
{
    _head->update(dt);
}


void AltScene::enableSwipe()
{
    auto swipe = SwipeGestureRecognizer::create();
    swipe->onSwipe = CC_CALLBACK_1(AltScene::onSwipe, this);
    addChild(swipe);
}


void AltScene::onSwipe(SwipeGestureRecognizer* recognizer)
{
    auto stato = recognizer->getStatus();
    
    if (stato == GestureStatus::RECOGNIZED)
    {
        auto direction = recognizer->getSwipeDirection();
        std::string text;
        float angle = 0;
        switch (direction)
        {
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_UP:
                _head->setDir(Vec2(0,1));
                angle = 90;
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_RIGHT:
                _head->setDir(Vec2(1,0));
                angle = 0;
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_DOWN:
                _head->setDir(Vec2(0,-1));
                angle = 270;
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_LEFT:
                _head->setDir(Vec2(-1,0));
                angle = 180;
                break;
            default:
                break;
        }
        float rv = fmod(angle + 180, 360);
        if (_head->getAngle() == angle) {
            _head->setSpeed(_head->getSpeed() * 2);
        } else {
            if (_head->getAngle() == rv) {
                _head->setSpeed(_head->getSpeed() / 2);
            }
            else {
            _head->setAngle(angle);
            }
        }
    }
}
