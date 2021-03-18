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
using namespace ui;
using namespace extension;

bool AltScene::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }
    auto size = Director::getInstance()->getVisibleSize();
    
    MenuItemFont::setFontSize(64);
    auto item = MenuItemFont::create("Toggle debug", [&](Ref* pSender) {
        _debugDraw = !_debugDraw;
        _physicsWorld->setDebugDrawMask(_debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
    });
    auto menuDebug = Menu::create(item, nullptr);
    this->addChild(menuDebug);
    menuDebug->setPosition(Vec2(size.width - item->getContentSize().width / 2 - 10, size.height - item->getContentSize().height / 2 - 10));
    
    _head = Head::create();
    _head->setPosition(size.width /2 , size.height/2);
    _head->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _head->setTag(0xf00);
    addChild(_head);
    _head->scheduleUpdate();
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
        _head->unscheduleUpdate();
        auto scene = SceneMenu::create();
        auto transition = TransitionSlideInT::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    auto menu = Menu::create(eatItem, closeItem, nullptr);
    
    menu->setPosition(Vec2(size.width / 2, 32));
    menu->alignItemsHorizontally();
    this->addChild(menu, 1);
    
    
    auto slider = ui::Slider::create();
    slider->setTag(1);
    slider->setTouchEnabled(true);
    slider->loadBarTexture("sliderTrack.png");
    slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
    slider->loadProgressBarTexture("sliderProgress.png");
    slider->setPosition(Vec2(size.width / 2, size.height * 0.15f + slider->getContentSize().height * 2.0f));
    slider->setPercent(52);
    slider->addEventListener(CC_CALLBACK_2(AltScene::sliderEvent, this));
    addChild(slider, 999);
    
    slider = ui::Slider::create();
    slider->setTag(2);
    slider->setTouchEnabled(true);
    slider->loadBarTexture("sliderTrack.png");
    slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
    slider->loadProgressBarTexture("sliderProgress.png");
    slider->setPosition(Vec2(size.width / 2, size.height * 0.1f + slider->getContentSize().height * 2.0f));
    slider->setPercent(30);
    slider->addEventListener(CC_CALLBACK_2(AltScene::sliderEvent, this));
    addChild(slider, 999);

    sliderEvent(slider, Slider::EventType::ON_PERCENTAGE_CHANGED);
    
    //adds contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(AltScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


    return true;
}

bool AltScene::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    log("Contact %d %d", nodeA->getTag(), nodeB->getTag());
    if (nodeA->getTag() == 0xf00 || nodeB->getTag() == 0xf00) {
        log("CRASH!");
        _head->unscheduleUpdate();
    }
    return true;
}

void AltScene::sliderEvent(Ref *pSender, ui::Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider*  slider = (Slider*)getChildByTag(1);
        _head->setMinDistance(slider->getPercent() * .2);
        slider =(Slider*)getChildByTag(2);
        _head->setTLim(slider->getPercent() / 100.0f);
        log("minDist: %f tlim %f", _head->getMindDistance(), _head->getTLim());
    }
}

void AltScene::update(float dt)
{
    
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
            _head->setSpeed(_head->getSpeed() + 50);
        } else {
            if (_head->getAngle() == rv) {
                _head->setSpeed(_head->getSpeed() - 50);
            }
            else {
            _head->setAngle(angle);
            }
        }
    }
}

void AltScene::rewardPlayer(const char *placementId)
{
    log("BaseScene: uninmplemented rewardPlayer for %s", placementId);
}
