//
//  SceneGame.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneGame.h"
#include "SceneMenu.h"
#include "UI/DPad.h"
#include "LevelOpened.h"
#include "UI/TimerSprite.h"
#include "audio/include/AudioEngine.h"
#include "Gestures/GestureRecognizerUtils.h"
#include "UI/TrophyModalBox.h"
#include "UI/BouncyPowerup.h"
#include "UI/PauseButton.h"
#include "Config.h"
#include "UI/LostModalBox.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    _score.startLevel(_level);
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    srand(time(NULL));
#endif
        
    _levelFile = levelFile;
    
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _map = TMXTiledMap::create(levelFile);
    addChild(_map, 0, 1);
    _map->setPosition(Vec2(size.width / 2, size.height / 2 ));
    _map->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer = _map->getLayer("layer0");
    auto s = layer->getLayerSize();
    tileSize = _map->getTileSize();
    
    auto loadingBar = LoadingBar::create("sliderProgress",  cocos2d::ui::CheckBox::TextureResType::PLIST);

    loadingBar->setPosition(Vec2(size.width/2, size.height - loadingBar->getContentSize().height));
    loadingBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loadingBar->setTag(0xa11);
    addChild(loadingBar);
    
    
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
    
//    auto levelstr = StringUtils::format("Level %d", level);
//    auto label = Label::createWithTTF(levelstr.c_str(), "Arcade.ttf", 64);
//    label->setPosition(Vec2(size.width/2, size.height - 32));
//    addChild(label);

    scoreLabel = Label::createWithTTF("SCORE: 0", "Arcade.ttf", 64, Size::ZERO, TextHAlignment::CENTER);
    scoreLabel->setPosition(Vec2(size.width / 2, size.height - scoreLabel->getContentSize().height));
    // Analogous colors - Ground: #c0f228
    scoreLabel->setTextColor ( Color4B(0xF2, 0xBF, 0x28, 0xFF) );
//    scoreLabel->enableShadow (Color4B(0x5B, 0xF2, 0x28, 0xFF));

    // Analogous - Border: #5ccf30
//    scoreLabel->enableShadow( Color4B(0x30, 0x5C, 0xCF, 0xFF) );
//    scoreLabel->setTextColor(Color4B(0xcf, 0x30, 0x5c, 0xFF));

    addChild(scoreLabel, 10);
    
    addChild(LevelOpened::create(level + 1));
        
    initBody();
       
    enableSwipe();

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::scoreByLiving), 0.100f);

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
    lastFood = arc4random() % 5 + 1;
    scheduleOnce(CC_SCHEDULE_SELECTOR(SceneGame::addFood), lastFood);

    if (Config::getInstance()->getAudio()) {
        _audioId = AudioEngine::play2d("Arcade-Puzzler.mp3", true, 0.7f);
        if(_audioId != AudioEngine::INVALID_AUDIO_ID) {
            log("Can't play background music");
        }
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
    
    auto bouncy = PowerUpIcon::createWithImage("helmet");
    bouncy->setPosition(Vec2(128, 128));
    bouncy->setTag(HELMET_TAG);
    addChild(bouncy);
    
    auto pause = PauseButton::create();
    pause->setPosition(Vec2(size.width - pause->getContentSize().width, pause->getContentSize().height));
    addChild(pause);

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
        
        auto bar = (cocos2d::ui::LoadingBar*)getChildByTag(0xa11);
        if (bar) {
            bar->setPercent((float)_foodEaten * 100.0f / getFoodTarget());
        }

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
    playEffect("munch.wav");
    scheduleOnce(CC_SCHEDULE_SELECTOR(SceneGame::addFood), lastFood);
    checkForWinLevel();
        
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();

}

void SceneGame::playEffect(const std::string &effect)
{
    if (Config::getInstance()->getEffects()) {
        AudioEngine::play2d(effect, false, 1.0f);
    }
}

void SceneGame::checkForWinLevel()
{
    _foodEaten++;
    if (_foodEaten == getFoodTarget()) {
        int nextLevel = _level + 1;
        if (nextLevel >= _score.getMaxLevel()) {
            _score.setMaxLevel(nextLevel);
        }
        
        unscheduleAllCallbacks();
        
        _active = false;
        _score.setStars(_level, 0x1); // one star for completing the level
        // if the player ate more than the target, we count it too.
        float target = getFoodTarget();
        float accuracy = (float)(_foodEaten * 100.0 / MAX(_foodAdded, target));
        _score.setAccuracy(_level, accuracy);
        _score.setFoodEat(_level, _foodEaten);
        _score.setTarget(_level, target);
        _score.setSnakeLength(_level, snake.getLength());

        if (accuracy >= 85) { // one star for 85% accuracy
            _score.setStars(_level, 0x2);
        }
        if (accuracy >= 99) { // one star for 99% accuracy
            _score.setStars(_level, 0x4);
        }
        _score.flush();
        
        auto alert = TrophyModalBox::create();
        alert->setTarget(MAX(getFoodTarget(), _foodAdded));
        alert->setEaten(_foodEaten);
        alert->setLevel(_level);
        alert->addFood(_foodEaten);
        alert->setScore(_score);
        alert->addButton("home-sm", "home-sm-pressed", 30, [=](Ref* s) {
            alert->ClosePopup();
            auto scene = SceneMenu::create();
            auto fade = TransitionFade::create(1, scene);
            Director::getInstance()->replaceScene(fade);
        });
        alert->addButton("home-sm", "home-sm-pressed", 0, [=](Ref* s) {});
        alert->addButton("home-sm", "home-sm-pressed", 0, [=](Ref* s) {});
        alert->addButton("repeat-normal", "repeat-bt-pressed", 38, [=](Ref*s) {
            alert->ClosePopup();
            auto scene = SceneGame::createWithFile(_level, _levelFile);
            auto fade = TransitionFade::create(1, scene);
            Director::getInstance()->replaceScene(fade);
        });
        alert->start();
        addChild(alert);
    }
}

void SceneGame::collide()
{
    unscheduleAllCallbacks();
    auto timer = static_cast<TimerSprite*>( getChildByTag(TIMER_TAG) );
    timer->stop();
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
    playEffect("crash.wav");

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
    if (!snake.canAdvance()) {
        auto bouncy = (PowerUpIcon*)getChildByTag(HELMET_TAG);
        if (bouncy->getCount() > 0) {
            auto powerup = BouncyPowerup::create();
            powerup->setOnCancel([&,powerup](Ref* pSender){
                collide();
                powerup->ClosePopup();
                Director::getInstance()->resume();
            });
            powerup->setOnSelection([&,powerup, bouncy](int dir){
                log("change direction %d", dir);
                onDpad(dir);
                if (!snake.canAdvance()) {
                    return;
                }
                powerup->ClosePopup();
                bouncy->setCount(bouncy->getCount() - 1);
                Bank::getInstance()->alterbouncyPowerup(-1);
                Director::getInstance()->resume();
                schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
            });
            addChild(powerup);
            Director::getInstance()->pause();
            return;
        } else {
            log("No powerup");
            collide();
        }
    }
    snake.advance();
    Vec2 headPos = snake.getPosAt(0);
    headPos.y = _map->getMapSize().height - headPos.y;
    auto gid = _map->getLayer("layer0")->getTileGIDAt(headPos);
    for(auto i = 0; i < snake.getLength(); i++) {
        auto moveTo = MoveTo::create(snakeSpeed, Vec2(snake.getPosAt(i).x, snake.getHeight() - snake.getPosAt(i).y) * tileSize.width);
        body.at(i)->runAction(calcSnakeMove(i));
    }
    auto pos = snake.getPosAt(0);
    auto moveBy = calcViewPointCenter();
    _map->runAction(MoveTo::create(snakeSpeed, moveBy));

    schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
}

Action* SceneGame::calcSnakeMove(int link)
{
    auto posF = Vec2(snake.getPosAt(link).x, snake.getHeight() - snake.getPosAt(link).y) * tileSize.width;
    Vec2 pos = body.at(link)->getPosition();
    auto delta = posF - pos;
    float dx = (link % 2) ? -1 : 1;
    MoveBy* mb;
    if(snake.getDirection().x == 0) {
        if (fmod(snake.getPosAt(link).y, 2) >= 1) {
            dx = -dx;;
        }
        dx *= tileSize.width / 4;
        mb = MoveBy::create(snakeSpeed / 2, Vec2(dx, 0));
    } else {
        if (fmod(snake.getPosAt(link).x, 2) >= 1) {
            dx = -dx;
        }
        dx *= tileSize.width / 4;
        mb = MoveBy::create(snakeSpeed /2 , Vec2(0, dx));
    }
    auto seq = Sequence::create(mb, mb->reverse(), nullptr);

    auto move = MyMoveTo::create(snakeSpeed, posF);
    if ( !(link%2)) return move;
    auto spawn = Spawn::createWithTwoActions(seq, move);
    return spawn;
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
    auto y = winSize.height / 2 ;

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

    unscheduleAllCallbacks();
    
    auto lost = LostModalBox::create();
    lost->setOnHome([&, lost](Ref* s){
        lost->ClosePopup();
        auto scene = SceneMenu::create();
        auto fade = TransitionFade::create(1, scene);
        Director::getInstance()->replaceScene(fade);
    });
    lost->setOnRepeat([&, lost](Ref *s){
        lost->ClosePopup();
        auto scene = SceneGame::createWithFile(_level, _levelFile);
        auto fade = TransitionFade::create(2, scene);
        Director::getInstance()->replaceScene(fade);
    });
    addChild(lost);
}

float SceneGame::getFoodTarget()
{
    return 1;
    return  4 + _level * 2;
}

void SceneGame::rewardPlayer(const char *placementId)
{
    log("*** Reward player not implemented!");
}
