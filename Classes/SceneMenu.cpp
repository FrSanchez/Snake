//
//  SceneMenu.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneMenu.h"
#include "SceneGame.h"
#include "ScoreLabel.h"
#include "Chooser.h"
#include "audio/include/AudioEngine.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ModalMessageBox.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* SceneMenu::createScene()
{
    return SceneMenu::create();
}

void SceneMenu::startGame(int level, std::string levelFile)
{
    if (_audioID != AudioEngine::INVALID_AUDIO_ID) {
        AudioEngine::stop(_audioID);
        _audioID = AudioEngine::INVALID_AUDIO_ID;
    }
    
    auto scene = SceneGame::createWithFile(level, levelFile);
    auto fade = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(fade);
}

// on "init" you need to initialize your instance
bool SceneMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();;
    auto files = findLevels();

    auto bg = Sprite::create("background.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    auto numLabel = Label::createWithTTF("1", "fonts/Arcade.ttf", 96);


    label = Label::createWithTTF("Play", "fonts/Arcade.ttf", 180);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2::ZERO);
    auto black = Color4B( Color4B::BLACK);
    auto grey = Color4B(128, 128, 128, 128);
    auto blue = Color4B(88, 157, 214, 255);
    label->setTextColor(Color4B::WHITE);
    
    _file = 0;
    auto chooser = Chooser::create();
    
    auto playItem = MenuItemLabel::create(label, [=](Ref *pSender){
        auto level = chooser->getValue();
        startGame(level, files.at(level - 1));
    });
    auto menuPlay = Menu::create(playItem, NULL);
    addChild(menuPlay);
    menuPlay->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    
    auto button = Button::create();
    button->setPosition(Vec2(64, 64));
    button->setName("print");
    addChild(button);
    
    auto print = Sprite::createWithSpriteFrameName("print");
    auto printItem = MenuItemSprite::create(print, print, [=](Ref* pSender){
        resetScores();
    });
    auto menuPrint = Menu::create(printItem, NULL);
    addChild(menuPrint);
    menuPrint->setPosition(Vec2(64, 64));
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)

    auto normal = Sprite::createWithSpriteFrameName("CloseNormal");
    auto select = Sprite::createWithSpriteFrameName("CloseSelected");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  { Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    this->~SceneMenu();
//    exit(0);
#endif
    });

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        log("'CloseNormal.png' and 'CloseSelected.png'");
        return false;
    }
    else
    {
        closeItem->setScale(2.0f);
        float x = size.width - closeItem->getContentSize().width ;
        float y = closeItem->getContentSize().height;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
#endif
    _dt = 0;
    schedule([&](float time){
        _dt += time;
        if (_dt > 1.0) {
            _dt -= 1.0;
        }
        _magnitude = _dt * _extent;
        auto offset = _direction;
        offset.x *= _magnitude;
        offset.y *= _magnitude;
        grey.a = (uint8_t) (128.0 * _dt);
        label->enableShadow(grey, Size(offset.x, offset.y), 50);
    }, "shadow");
        
    _audioID = AudioEngine::play2d("Bonkers-for-Arcades.mp3", true, 0.7f);
    
    if(_audioID != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }
        
    chooser->setValue(1);
    chooser->setMinValue(1);
    chooser->setMaxValue(MIN(_score.getMaxLevel(), (int) files.size()));
    chooser->setPosition(Vec2(size.width / 2, size.height / 2 - 180));
    chooser->setOnValueChangeCallback(CC_CALLBACK_1(SceneMenu::onValueChange, this));
    addChild(chooser);
    _score.loadAllLevels();
    onValueChange(1);

    return true;
}

void SceneMenu::resetScores()
{
    auto alert = ModalMessageBox::create();
    alert->setString("All the scores and achievements will reset to zero!");
    alert->setTag(0x123);
    alert->addButton("OK", 30, [=](Ref* pSender) {
        alert->ClosePopup();
        _score.reset();
    });
    alert->addButton("Cancel", 30,  [=](Ref* pSender) {
        alert->ClosePopup();
    });
    addChild(alert);
}

void SceneMenu::onValueChange(int value)
{
    float score = _score.getMaxScore(value);
    auto score_str = StringUtils::format("HIGH SCORE: %.1f", score);
    auto label = getChildByTag<Label*>(0xf1);
    if (!label) {
        addChild(label = ScoreLabel::createHS(), 1, 0xf1);
    }
    label->setString(score_str);
}


std::vector<std::string> SceneMenu::findLevels()
{
    auto sharedFileUtils = FileUtils::getInstance();
    int level = 0;
    std::vector<std::string> files;
    bool isExist = false;
    do {
        level++;
        auto levelFile = StringUtils::format("level%d.tmx", level);
        isExist = sharedFileUtils->isFileExist(levelFile);
        CCLOG("Looking for %s: %s", levelFile.c_str(), isExist ? "true": "false");
        if (isExist) {
            files.push_back(levelFile);
        }
    } while(isExist);
    CCLOG("Max level: %d", level - 1);
    return files;
}
