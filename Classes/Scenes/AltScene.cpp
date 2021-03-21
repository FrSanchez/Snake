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
#include "UI/LostModalBox.h"
#include "UI/PauseButton.h"

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
    
    addChild(Arena::create());
    
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
        
    auto pause = PauseButton::create();
    pause->setPosition(Vec2(size.width - pause->getContentSize().width, pause->getContentSize().height / 2));
    addChild(pause);
    
    auto homeButton = Button::create("home-sm", "home-sm-pressed", "home-sm", cocos2d::ui::AbstractCheckButton::TextureResType::PLIST);
    homeButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                closeScene(sender);
                break;
            default:
                break;
        }
    });
    homeButton->setPosition(Vec2(size.width - pause->getContentSize().width - homeButton->getContentSize().width, pause->getContentSize().height / 2));
    addChild(homeButton);

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
     
    _head = SnakeHead::create();
    _head->setPosition(size.width /2 , size.height/2 - 64);
    _head->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _head->setTag(0xf00);
    _head->setTLim(30 / 100.0f);
    addChild(_head);
    for(int i =0; i < 4; i++) { _head->addBodyPart(); }

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

    
    scheduleOnce((SEL_SCHEDULE)&AltScene::addFruit, CCRANDOM_0_1() * 5 + duration);
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
    int points = fruit->getOpacity() * 100 / 255;

    if (fruit)
    {
        fruit->removeFromParentAndCleanup(true);
    }
    _head->setSpeed(_head->getSpeed() + 20);
    _head->runAction(GrowByAction::create(0.2, 3, CC_CALLBACK_0(SnakeHead::getSize, _head), CC_CALLBACK_1(SnakeHead::grow, _head)));
    _head->setTLim(_head->getTLim() * 0.9);
    log("Current tLim: %f", _head->getTLim());
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d("impactMetal_light_004.mp3", false, 1.0f);
    }
    auto scorelbl = dynamic_cast<GameScoreLabel*>(getChildByTag(SCORE_TAG));
    if (scorelbl) {
        scorelbl->runAction(GrowByAction::create(0.2, points, CC_CALLBACK_0(GameScoreLabel::getValue, scorelbl), CC_CALLBACK_1(GameScoreLabel::setValue, scorelbl)));
    }
    _foodEaten ++;
}

void AltScene::isDead()
{
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

    log("your snake is dead");
}

void AltScene::crash()
{
    unscheduleAllCallbacks();
    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d("impactPlate_medium_001.mp3", false, 1.0f);
    }
    _head->die(CC_CALLBACK_0(AltScene::isDead, this));
    
    auto lost = LostModalBox::create();
    lost->setOnHome([&, lost](Ref* s){
        lost->ClosePopup();
        closeScene();
    });
    lost->setOnRepeat([&, lost](Ref *s){
        lost->ClosePopup();
        auto scene = AltScene::create();
        auto fade = TransitionFade::create(1, scene);
        Director::getInstance()->replaceScene(fade);
    });
    addChild(lost);

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
