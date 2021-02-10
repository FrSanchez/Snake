//
//  SceneGame.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneGame.h"
#include "SceneMenu.h"
#include "DPad.h"
#include "audio/include/AudioEngine.h"
#include "Gestures/GestureRecognizerUtils.h"

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
    
    _map = TMXTiledMap::create("level2.tmx");
    addChild(_map, 0, 1);
    _map->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _map->setPosition(Vec2(0.5 * size.width, size.height - 96));
    layer = _map->getLayer("layer0");
    auto s = layer->getLayerSize();
    tileSize = _map->getTileSize();
    
    SpriteBatchNode* child = nullptr;
    auto& children = _map->getChildren();
    
    for(const auto &node : children) {
        child = static_cast<SpriteBatchNode*>(node);
        child->getTexture()->setAntiAliasTexParameters();
    }
    
    layer->setOpacity(128);
    
    snake = Snake(s.width, s.height);
    snake.setLayer(layer);

    float offsetX = _map->getPosition().x - _map->getContentSize().width / 2;
    float offsetY = _map->getPosition().y - _map->getContentSize().height ;
    corner = Vec2(offsetX, offsetY);
    
    
    apple = Sprite::createWithSpriteFrameName("fruit.png");
    apple->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    apple->setVisible(false);
    _map->addChild(apple, 10);
    
    auto pos = Label::createWithTTF("0, 0", "fonts/arial.ttf", 32);
    pos->setPosition(Vec2(0, 0));
    pos->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(pos, 10, 0x10);

    scoreLabel = Label::createWithTTF("SCORE: 0", "fonts/Arcade.ttf", 64, Size::ZERO, TextHAlignment::CENTER);
    scoreLabel->setPosition(Vec2(size.width / 2, size.height - scoreLabel->getContentSize().height));
    scoreLabel->setTextColor( Color4B::RED );
    scoreLabel->enableOutline(Color4B::YELLOW,1);
    scoreLabel->enableShadow(Color4B::GREEN);
    addChild(scoreLabel, 10);
        
    initBody();
    
    // creating a keyboard event listener
    auto kbdLstnr = EventListenerKeyboard::create();
    kbdLstnr->onKeyPressed = CC_CALLBACK_2(SceneGame::onKeyPressed, this);
    kbdLstnr->onKeyReleased = CC_CALLBACK_2(SceneGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(kbdLstnr, this);
            
    enableSwipe();

    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
    lastFood = arc4random() % 10 + 5;

    _audioId = AudioEngine::play2d("Arcade-Puzzler.mp3", true, 0.7f);
    
    if(_audioId != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }
    
    auto dpad = new DPad();
    dpad->setPosition(Vec2(240, 32));
    dpad->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dpad->setCallback(CC_CALLBACK_1(SceneGame::onDpad, this));
    addChild(dpad);

    return true;
}

void SceneGame::onDpad(int dir)
{
    if (dir == 1) {
        snake.turnUp();
    }
    if (dir == 2) {
        snake.turnDown();
    }
    if (dir == 4) {
        snake.turnLeft();
    }
    if (dir == 8) {
        snake.turnRight();
    }
}

void SceneGame::enableSwipe()
{
    auto swipe = SwipeGestureRecognizer::create();
    swipe->onSwipe = CC_CALLBACK_1(SceneGame::onSwipe, this);
    addChild(swipe);
}


void SceneGame::onSwipe(SwipeGestureRecognizer* recognizer)
{
    auto stato = recognizer->getStatus();
    
    if (stato == GestureStatus::RECOGNIZED)
    {
        auto direction = recognizer->getSwipeDirection();
        std::string text;
        switch (direction)
        {
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_UP:
                snake.turnUp();
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_RIGHT:
                snake.turnRight();
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_DOWN:
                snake.turnDown();
                break;
            case SwipeGestureRecognizer::SwipeStatus::SWIPE_LEFT:
                snake.turnLeft();
                break;
            default:
                break;
        }
    }
}

void SceneGame::labelCallback(Label* label)
{
    label->setVisible(false);
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
    auto action = Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.5f, 0.5f),
                                              MoveTo::create(0.5f, scoreLabel->getPosition()));
    auto seq = Sequence::create(action,
                                CallFunc::create([&]() {
        auto score_str = StringUtils::format("SCORE: %d", score);
        scoreLabel->setString(score_str);
        apple->setVisible(false);
    }), nullptr);
    apple->runAction(seq);
    lastFood = arc4random() % 10 + 1;
    food = Vec2::ZERO;
    snake.grow();
    auto sprite = Sprite::createWithSpriteFrameName("body.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto i = snake.getLength() - 1;
    sprite->setPosition((snake.getPosAt(i) * tileSize.width) );
    body.push_back(sprite);
    _map->addChild(sprite, 0);
    snakeSpeed *= 0.95f;
    score++;
    AudioEngine::play2d("munch.wav", false, 1.0f);
}

void SceneGame::collide()
{
    auto total = body.size();
       for(auto i=1; i < total; i++)
    {
        body.at(i)->runAction(Blink::create(2, 10));
    }
    if (_audioId != AudioEngine::INVALID_AUDIO_ID) {
        AudioEngine::stop(_audioId);
        _audioId = AudioEngine::INVALID_AUDIO_ID;
    }
    AudioEngine::play2d("crash.wav", false, 1.0f);

    if (apple != nullptr && food != Vec2::ZERO) {
        auto fadeOut = FadeOut::create(1.0f);
        apple->runAction(fadeOut);
    }
    auto end = CallFunc::create([=]() {
        closeScene(this);
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
    Vec2 headPos = snake.getPosAt(0);
    headPos.y = _map->getMapSize().height - headPos.y;
    auto gid = _map->getLayer("layer0")->getTileGIDAt(headPos);
    for(auto i = 0; i < snake.getLength(); i++) {
        auto moveTo = MoveTo::create(snakeSpeed, Vec2(0, -tileSize.height) + (snake.getPosAt(i) * tileSize.width));
        body.at(i)->runAction(moveTo);
    }
    if (lastFood <= 0 && food == Vec2::ZERO) {
        addFood();
    }
    auto label = getChildByTag<Label*>(0x10);
    if (label != nullptr) {
        auto posStr = StringUtils::format("%.1f,%.1f", headPos.x, headPos.y);
        label->setString(posStr);
    }
    auto pos = snake.getPosAt(0);
    auto moveBy = calcViewPointCenter();
    _map->runAction(MoveTo::create(snakeSpeed, moveBy));

    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
}

Vec2 SceneGame::calcViewPointCenter()
{
    auto winSize = Director::getInstance()->getWinSize();
    auto headPos = Vec2(snake.getPosAt(0).x * tileSize.width , snake.getPosAt(0).y * tileSize.height);
    auto x = winSize.width / 2;
    auto y = winSize.height - 96;;

    auto dx = _map->getContentSize().width / 2 - headPos.x;
    
    return Vec2(x + dx, y);
}


void SceneGame::addFood()
{
    uint32_t gid;
    do {
        food = Vec2(arc4random() % snake.getWidth() , arc4random() % snake.getHeight() );
        gid = layer->getTileGIDAt(food);
    } while(gid != 4 && food != snake.getPosAt(0));
    
    CCLOG("Food: %.1f %.1f", food.x, food.y);
    
    auto fadeIn = FadeIn::create(0.5f);
    apple->stopAllActions();
    apple->setPosition(Vec2(food.x * tileSize.width, (food.y - 1) * tileSize.height) );
    apple->setScale(0.1f);
    apple->setVisible(true);
    apple->runAction(
                     Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1, 1), fadeIn)
                     );
}

void SceneGame::initBody()
{
    auto total = snake.getLength();
       for(auto i=0; i < total; i++)
       {
           Sprite* sprite;
           if (i == 0) {
               sprite = Sprite::createWithSpriteFrameName("head.png");
           } else {
               sprite = Sprite::createWithSpriteFrameName("body.png");
           }
           sprite->setPosition(snake.getPosAt(i) * tileSize.width);
           sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
           body.push_back(sprite);
           _map->addChild(sprite, 0);
       }
}

void SceneGame::closeScene(Ref* pSender)
{
    SceneMenu* scene = static_cast<SceneMenu*> (SceneMenu::createScene());
    scene->setScore(score);
    auto fade = TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(fade);
}
