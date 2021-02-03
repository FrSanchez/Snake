//
//  SceneGame.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneGame.h"
#include "SceneMenu.h"

USING_NS_CC;

Scene* SceneGame::createScene()
{
    return SceneGame::create();
}


// on "init" you need to initialize your instance
bool SceneGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    srand(time(NULL));
#endif

    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("grass.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    float offsetX = bg->getPosition().x - bg->getContentSize().width / 2 + 10;
    float offsetY = bg->getPosition().y - bg->getContentSize().height / 2 + 10;
    corner = Vec2(offsetX, offsetY);
    
    
    apple = Sprite::create("circles.png", Rect(0,21,20,20));
    if (apple == nullptr)
    {
        log("Error loading circles.png");
        return false;
    }
    apple->setPosition(-20, -20);
    this->addChild(apple, 0);

    initBody();
    
    // creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(SceneGame::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(SceneGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
    lastFood = arc4random() % 10 + 1;

    return true;
}


// Implementation of the keyboard event callback function prototype
void SceneGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SceneGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
        snake.turnLeft();
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
        snake.turnRight();
    }
}

void SceneGame::eat()
{
    apple->setPosition(-20, -20);
    lastFood = arc4random() % 10 + 1;
    food = Vec2::ZERO;
    snake.grow();
    auto sprite = Sprite::create("circles.png", Rect(0,0,20,20));
    auto i = snake.getLength() - 1;
    sprite->setPosition((snake.getPosAt(i) * 20.0f) + corner);
    body.push_back(sprite);
    this->addChild(sprite, 0);
    snakeSpeed *= 0.95f;
}

void SceneGame::collide()
{
    auto total = body.size();
       for(auto i=1; i < total; i++)
    {
        auto fadeOut = FadeOut::create(1.0f);
        body.at(i)->runAction(fadeOut);
    }
    if (apple != nullptr && food != Vec2::ZERO) {
        auto fadeOut = FadeOut::create(1.0f);
        apple->runAction(fadeOut);
    }
    auto end = CallFunc::create([=]() {
        menuCloseCallback(this);
    });
    auto fadeOut = FadeOut::create(1.0f);
    auto seq = Sequence::create(fadeOut, end, nullptr);
    body.at(0)->runAction(seq);
                  
}

void SceneGame::updateTimer(float dt)
{
    if (lastFood)
        lastFood--;
    if (snake.getPosAt(0) == food) {
        eat();
    }
    if (!snake.advance()) {
        collide();
        return;
    }
    for(auto i = 0; i < snake.getLength(); i++) {
        auto moveTo = MoveTo::create(snakeSpeed, corner + (snake.getPosAt(i) * 20.0f));
        body.at(i)->runAction(moveTo);
    }
    if (lastFood <= 0 && food == Vec2::ZERO) {
        addFood();
    }
    auto pos = snake.getPosAt(0);
    log("lastFood %d speed: %.3f", lastFood, snakeSpeed);
    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
}

void SceneGame::addFood()
{
    food = Vec2(arc4random() % 20 + 1, arc4random() % 20 + 1);
    auto fadeIn = FadeIn::create(0.5f);
    apple->setPosition((food * 20.0f) + corner);
    apple->runAction(fadeIn);
}

void SceneGame::initBody()
{
    headSprite = Sprite::create("circles.png", Rect(0,0,20,20));
    auto total = snake.getLength();
       for(auto i=0; i < total; i++)
       {
           auto sprite = Sprite::create("circles.png", Rect(0,0,20,20));
           sprite->setPosition(snake.getPosAt(i) * 20.0f);
           body.push_back(sprite);
           this->addChild(sprite, 0);
       }
}

void SceneGame::menuCloseCallback(Ref* pSender)
{
    auto scene = SceneMenu::createScene();
    auto fade = TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(fade);
}
