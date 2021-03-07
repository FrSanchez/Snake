//
//  SceneGame.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneGame.h"
#include "SceneMenu.h"
#include "DPad.h"
#include "LevelOpened.h"
#include "TimerSprite.h"
#include "audio/include/AudioEngine.h"
#include "Gestures/GestureRecognizerUtils.h"
#include "TrophyModalBox.h"

USING_NS_CC;

Scene* SceneGame::createWithFile(int level, std::string levelFile)
{
    SceneGame *pRet = new(std::nothrow) SceneGame();
    if (pRet && pRet->init(level, levelFile))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool SceneGame::init(int level, std::string levelFile)
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    _active = true;
    _level = level;
    _score.setLevel(_level);
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    srand(time(NULL));
#endif
        
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _map = TMXTiledMap::create(levelFile);
    addChild(_map, 0, 1);
    _map->setPosition(Vec2(size.width / 2, size.height / 2 ));
    _map->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer = _map->getLayer("layer0");
    auto s = layer->getLayerSize();
    tileSize = _map->getTileSize();
    
    SpriteBatchNode* child = nullptr;
    auto& children = _map->getChildren();
    
    for(const auto &node : children) {
        child = static_cast<SpriteBatchNode*>(node);
        child->getTexture()->setAntiAliasTexParameters();
    }
    
    snake = Snake(s.width, s.height);
    snake.setLayer(layer);

    apple = Sprite::createWithSpriteFrameName("fruit");
    apple->setVisible(false);
    apple->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _map->addChild(apple, 10);
    
    auto levelstr = StringUtils::format("Level %d", level);
    auto label = Label::createWithTTF(levelstr.c_str(), "fonts/Arcade.ttf", 64);
    label->setPosition(Vec2(size.width/2, size.height - 32));
    addChild(label);
    
    auto pos = Label::createWithTTF("0, 0", "fonts/arial.ttf", 32);
    pos->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pos->setPosition(Vec2(0, 0));
    addChild(pos, 10, 0x10);

    scoreLabel = Label::createWithTTF("SCORE: 0", "fonts/Arcade.ttf", 64, Size::ZERO, TextHAlignment::CENTER);
    scoreLabel->setPosition(Vec2(size.width / 2, size.height - scoreLabel->getContentSize().height));
    scoreLabel->setTextColor( Color4B::RED );
    scoreLabel->enableOutline(Color4B::YELLOW,3);
    scoreLabel->enableShadow(Color4B::GREEN);
    addChild(scoreLabel, 10);
    
    addChild(LevelOpened::create(level + 1));
        
    initBody();
       
    enableSwipe();

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::scoreByLiving), 0.100f);

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
    lastFood = arc4random() % 5 + 1;
    scheduleOnce(CC_SCHEDULE_SELECTOR(SceneGame::addFood), lastFood);

    _audioId = AudioEngine::play2d("Arcade-Puzzler.mp3", true, 0.7f);
    
    if(_audioId != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }
    
    auto dpad = new DPad();
    dpad->setPosition(Vec2(240, 32));
    dpad->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dpad->setCallback(CC_CALLBACK_1(SceneGame::onDpad, this));
    addChild(dpad);
    
    auto _locator = Sprite::createWithSpriteFrameName("UpSelected");
    _locator->setVisible(false);
    _locator->setPosition(size.width / 2, size.height / 2);
    _locator->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _map->addChild(_locator, 1, 0x20);

    snakeSpeed = 0.35;
    
    auto timer = TimerSprite::create();
    timer->setPosition(size.width - 64, size.height - 64);
    timer->setTag(TIMER_TAG);
    timer->setOnTimerEndCallback(CC_CALLBACK_0(SceneGame::onTimerEnd, this));
    addChild(timer);
        
    return true;
}

void SceneGame::onTimerEnd()
{
    food = Vec2::ZERO;
    scheduleOnce(CC_SCHEDULE_SELECTOR(SceneGame::addFood), lastFood);
    apple->setVisible(false);
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

void SceneGame::eat()
{
    auto action = Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.5f, 0.5f),
                                              MoveTo::create(0.5f, scoreLabel->getPosition()));
    auto seq = Sequence::create(action,
                                CallFunc::create([&]() {
        auto score = _score.getScore(_level);
        auto score_str = StringUtils::format("SCORE: %d", score );
        scoreLabel->setString(score_str);
        apple->setVisible(false);
    }), nullptr);
    apple->runAction(seq);
    lastFood = arc4random() % 10 + 1;
    food = Vec2::ZERO;
    snake.grow();
    auto sprite = Sprite::createWithSpriteFrameName("body");
    sprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    auto i = snake.getLength() - 1;
    sprite->setPosition(Vec2(snake.getPosAt(i).x, snake.getHeight() - snake.getPosAt(i).y) * tileSize.width);
    body.push_back(sprite);
    _map->addChild(sprite, 0);
    snakeSpeed *= 0.95f;
    _score.addScore(_level, 10);
    AudioEngine::play2d("munch.wav", false, 1.0f);
    scheduleOnce(CC_SCHEDULE_SELECTOR(SceneGame::addFood), lastFood);
    checkForOpenLevel();
    
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

}

void SceneGame::checkForOpenLevel()
{
    _foodEaten++;
    if (_foodEaten == getFoodTarget()) {
        int nextLevel = _level + 1;
        if (nextLevel >= _score.getMaxLevel()) {
            auto label = static_cast<LevelOpened*>(getChildByTag(LEVEL_OPENED_TAG));
            label->display();
            _score.setMaxLevel(nextLevel);
        }
    }
}

void SceneGame::collide()
{
    unscheduleAllCallbacks();
    auto total = body.size();
       for(auto i=1; i < total; i++)
    {
        auto seq = Sequence::create(Blink::create(2, 10), FadeOut::create(0.5f), nullptr);
        body.at(i)->runAction(seq);
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
        auto moveTo = MoveTo::create(snakeSpeed, Vec2(snake.getPosAt(i).x, snake.getHeight() - snake.getPosAt(i).y) * tileSize.width);
        body.at(i)->runAction(moveTo);
    }
    auto label = getChildByTag<Label*>(0x10);
    if (label != nullptr) {
        auto posStr = StringUtils::format("%.1f,%.1f : %d", headPos.x, headPos.y, layer->getTileGIDAt(headPos));
        label->setString(posStr);
    }
    auto pos = snake.getPosAt(0);
    auto moveBy = calcViewPointCenter();
    _map->runAction(MoveTo::create(snakeSpeed, moveBy));

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
}

void SceneGame::scoreByLiving(float dt)
{
    static float accum = 0;
    accum += dt;
    if (accum > 1) {
        accum-=1;
        _score.addScore(_level, 1 );
        auto score = _score.getScore(_level);
        auto score_str = StringUtils::format("SCORE: %d", score );
        scoreLabel->setString(score_str);
    }
}

Vec2 SceneGame::calcViewPointCenter()
{
    auto winSize = Director::getInstance()->getWinSize();
    auto cSz = _map->getContentSize();
    auto headPos = Vec2(snake.getPosAt(0).x * tileSize.width , snake.getPosAt(0).y * tileSize.height);
    auto x = winSize.width / 2;
    auto y = winSize.height / 2 + 64;;

    auto dx = cSz.width / 2 - headPos.x;
    auto lmt = cSz.width/2 - winSize.width/2;
    if (dx < -lmt) { dx = -lmt; };
    if (dx > lmt) { dx = lmt; }
    x+= dx;
    
    return Vec2(x , y);
}


void SceneGame::addFood(float dt)
{
    if (!_active) {
        return;
    }
    uint32_t gid;
    do {
        food = Vec2(arc4random() % snake.getWidth() , arc4random() % snake.getHeight() );
        gid = layer->getTileGIDAt(food);
    } while(gid != SPACE_BLOCK && !snake.isOccupying(food));

    _foodAdded++;

    auto head = snake.getPosAt(0);
    auto dist = MAX(4,  food.distance(head) );
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->activate(dist);
    CCLOG("Distance to food %f", dist);

    auto fadeIn = FadeIn::create(0.5f);
    apple->stopAllActions();
    apple->setPosition(Vec2(food.x * tileSize.width, (snake.getHeight() - food.y) * tileSize.height) );
    apple->setScale(0.1f);
    apple->setVisible(true);
    apple->runAction(
                     Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1, 1), fadeIn)
                     );
    showFoodLocator(apple->getPosition());
}

float SceneGame::getAngleDegrees(cocos2d::Vec2 from, cocos2d::Vec2 to)
{
    float deltaX = from.x-to.x;
    float deltaY = from.x-to.y;
    float radians = atan2(deltaY, deltaX); //Math.atan2(deltaY, deltaX)
    float degrees = MATH_RAD_TO_DEG(radians); //(radians * 180) / Math.PI - 90; // rotate
    while (degrees >= 360) degrees -= 360;
    while (degrees < 0) degrees += 360;
    return degrees;
}

void SceneGame::showFoodLocator(Vec2 foodPos)
{
    auto size = Director::getInstance()->getVisibleSize();
    if (_map->getContentSize().width <= size.width)
        return;
    auto _locator = _map->getChildByTag<Sprite*>(0x20);
    
    auto p2 = body.at(0)->getPosition();
    _locator->setVisible(true);
    _locator->setPosition(p2.x, foodPos.y);
    if (_locator->getPosition().x < foodPos.x) {
        _locator->setSpriteFrame("right");
    } else {
        _locator->setSpriteFrame("left");
    }

    auto seq = Sequence::create(Blink::create(2,10),
                                CallFunc::create([&, _locator]() {
        _locator->setVisible(false);
    }), nullptr);
    _locator->runAction(seq);
}

void SceneGame::initBody()
{
    auto total = snake.getLength();
       for(auto i=0; i < total; i++)
       {
           Sprite* sprite;
           if (i == 0) {
               sprite = Sprite::createWithSpriteFrameName("head");
           } else {
               sprite = Sprite::createWithSpriteFrameName("body");
           }
           auto spritePos = Vec2(snake.getPosAt(0).x, snake.getHeight() - snake.getPosAt(0).y) * tileSize.width;
           sprite->setPosition(spritePos);
           sprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
           body.push_back(sprite);
           _map->addChild(sprite, 0);
       }
}

void SceneGame::closeScene(Ref* pSender)
{
    _active = false;
    // if the player ate more than the target, we count it too.
    float target = getFoodTarget();
    float accuracy = (float)(_foodEaten * 100.0 / target);
    _score.setAccuracy(_level, accuracy);
    _score.setFoodEat(_level, _foodEaten);
    _score.setTarget(_level, target);
    _score.setSnakeLength(_level, snake.getLength());

    if (snake.getLength() >= (target+4)) {
        _score.setStars(_level, 0x1);
    }
    if (accuracy > 99) {
        _score.setStars(_level, 0x2);
    }
    _score.setStars(_level, 0xf);
    _score.flush();

    unscheduleAllCallbacks();
    
    auto alert = TrophyModalBox::create();
    alert->setLevel(_level);
    alert->addButton("OK", 30, [=](Ref* pSender) {
        alert->ClosePopup();
        auto scene = SceneMenu::create();
        auto fade = TransitionFade::create(1, scene);
        Director::getInstance()->replaceScene(fade);
    });
    addChild(alert);
}

float SceneGame::getFoodTarget()
{
    return  4 + _level * 2;
}
