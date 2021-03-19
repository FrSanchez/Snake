//
//  AltScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#include "AltScene.h"
#include "SceneMenu.h"
#include "audio/include/AudioEngine.h"
#include "GameElements/TimerSprite.h"

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
    
    _foodAdded = 0;
    _foodEaten = 0;
    MenuItemFont::setFontSize(64);
    auto item = MenuItemFont::create("Toggle debug", [&](Ref* pSender) {
        _debugDraw = !_debugDraw;
        _physicsWorld->setDebugDrawMask(_debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
    });
    auto menuDebug = Menu::create(item, nullptr);
    this->addChild(menuDebug);
    menuDebug->setPosition(Vec2(size.width - item->getContentSize().width / 2 - 10, size.height - item->getContentSize().height / 2 - 10));
    
    _head = SnakeHead::create();
    _head->setPosition(size.width /2 , size.height/2);
    _head->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _head->setTag(0xf00);
    addChild(_head);
    for(int i =0; i < 4; i++) { _head->addBodyPart(); }
    
    auto normal = Sprite::createWithSpriteFrameName("exit");
    auto select = Sprite::createWithSpriteFrameName("exit_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, CC_CALLBACK_1(AltScene::closeScene, this));
    
    auto menu = Menu::create(closeItem, nullptr);
    
    menu->setPosition(Vec2(size.width / 2, 32));
    menu->alignItemsHorizontally();
    this->addChild(menu, 1);

    
//    auto slider = ui::Slider::create();
//    slider->setTag(2);
//    slider->setTouchEnabled(true);
//    slider->loadBarTexture("sliderTrack.png");
//    slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
//    slider->loadProgressBarTexture("sliderProgress.png");
//    slider->setPosition(Vec2(size.width / 2, size.height * 0.1f + slider->getContentSize().height * 2.0f));
//    slider->setPercent(30);
//    slider->addEventListener(CC_CALLBACK_2(AltScene::sliderEvent, this));
//    addChild(slider, 999);
//
//    sliderEvent(slider, Slider::EventType::ON_PERCENTAGE_CHANGED);
    _head->setTLim(30 / 100.0f);
    
    //adds contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(AltScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    scheduleOnce((SEL_SCHEDULE)&AltScene::addFruit, 3);
    
    addChild(GameScoreLabel::create());

    auto timer = TimerSprite::create();
    timer->setPosition(size.width - 64, size.height - 64);
    timer->setTag(TIMER_TAG);
//    timer->setOnTimerEndCallback(CC_CALLBACK_0(SceneGame::onTimerEnd, this));
    addChild(timer);
    
    enableSwipe();
    

    
    return true;
}

void AltScene::addFruit(float dt)
{
    float duration = random(5, 10);
    auto fruit = Fruit::create();
    fruit->setDuration(duration);
    addChild(fruit);
    
    _foodAdded++;
    
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->activate(duration);

    
    scheduleOnce((SEL_SCHEDULE)&AltScene::addFruit, CCRANDOM_0_1() * 5 + 5);
}

cocos2d::Node* AltScene::findNode(cocos2d::Node* a, cocos2d::Node* b, int tag)
{
    if (a->getTag() == tag) {
        return a;
    }
    if (b->getTag() == tag) {
        return b;
    }
    return nullptr;
}

bool AltScene::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    log("Contact %d %d", nodeA->getTag(), nodeB->getTag());
    auto snake = dynamic_cast<SnakeHead*>(findNode(nodeA, nodeB, 0xf00));
    auto fruit = dynamic_cast<Fruit*>(findNode(nodeA, nodeB, 0xf02));
    if (snake) {
        if (fruit) {
            eat(fruit);
        } else {
            crash();
        }
    }
    return true;
}

void AltScene::eat(Fruit *fruit)
{
    log("eat");
    if (fruit)
    {
        fruit->removeFromParentAndCleanup(true);
    }
    _head->setSpeed(_head->getSpeed() + 20);
    _head->runAction(GrowByAction::create(0.2, 5, CC_CALLBACK_0(SnakeHead::getSize, _head), CC_CALLBACK_1(SnakeHead::grow, _head)));
    _head->setTLim(_head->getTLim() * 0.9);
    log("Current tLim: %f", _head->getTLim());
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d("impactMetal_light_004.ogg", false, 1.0f);
    }
    auto scorelbl = dynamic_cast<GameScoreLabel*>(getChildByTag(SCORE_TAG));
    if (scorelbl) {
        scorelbl->runAction(GrowByAction::create(0.2, 10, CC_CALLBACK_0(GameScoreLabel::getValue, scorelbl), CC_CALLBACK_1(GameScoreLabel::setValue, scorelbl)));
    }
    _foodEaten ++;
}

void AltScene::isDead()
{
    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d("impactPlate_medium_001.ogg", false, 1.0f);
    }
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

    log("your snake is dead");
}

void AltScene::crash()
{
    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d("impactPlate_medium_001.ogg", false, 1.0f);
    }
    _head->die(CC_CALLBACK_0(AltScene::isDead, this));
}

void AltScene::sliderEvent(Ref *pSender, ui::Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        auto slider =(Slider*)getChildByTag(2);
        _head->setTLim(slider->getPercent() / 100.0f);
        log("minDist: %f tlim %f", _head->getMindDistance(), _head->getTLim());
    }
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
//            _head->setSpeed(_head->getSpeed() + 50);
        } else {
            if (_head->getAngle() == rv) {
//                _head->setSpeed(_head->getSpeed() - 50);
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

void AltScene::closeScene(Ref* s)
{
    auto scene = SceneMenu::create();
    if (scene != nullptr) {
        auto transition = TransitionSlideInR::create(1, scene);
        Director::getInstance()->replaceScene(transition);
        Config::getInstance()->save();
    }
}
