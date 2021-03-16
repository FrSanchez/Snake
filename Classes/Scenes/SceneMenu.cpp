//
//  SceneMenu.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneMenu.h"
#include "SceneGame.h"
#include "UI/ScoreLabel.h"
#include "UI/Chooser.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "UI/ModalMessageBox.h"
#include "UI/TrophyModalBox.h"
#include "StoreScene.h"
#include "Config.h"
#include "SettingsScene.h"
#include "AltScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

SceneMenu::SceneMenu()
{
    _audioProfile.name = "Music";
    _audioProfile.maxInstances = 1;
    _audioProfile.minDelay = 1.0;
    downloader.reset(new network::Downloader());
}

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
    
    initUnityAdsFunc();
    
    auto size = Director::getInstance()->getVisibleSize();
    findLevels();

    auto bg = Sprite::create("background.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    auto numLabel = Label::createWithTTF("1", "Arcade.ttf", 96);


    label = Label::createWithTTF("Play", "Arcade.ttf", 180);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2::ZERO);
    auto black = Color4B( Color4B::BLACK);
    auto grey = Color4B(128, 128, 128, 128);
    auto blue = Color4B(88, 157, 214, 255);
    label->setTextColor(Color4B::WHITE);
    
    _file = 0;
    _chooser = Chooser::create();
    
    auto playItem = MenuItemLabel::create(label, [=](Ref *pSender){
        showUnityAdsFunc(pSender);
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
    


    auto normal = Sprite::createWithSpriteFrameName("exit");
    auto select = Sprite::createWithSpriteFrameName("exit_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  {
        Director::getInstance()->end();
    });
   
    closeItem->setScale(1.5f);
    float x = size.width - closeItem->getContentSize().width ;
    float y = closeItem->getContentSize().height;
    closeItem->setPosition(Vec2(x,y));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    normal = Sprite::createWithSpriteFrameName("shoppingCart");
    select = Sprite::createWithSpriteFrameName("shoppingCart-pressed");
    auto storeItem = MenuItemSprite::create(normal, select, CC_CALLBACK_1(SceneMenu::openStore, this));
    
    auto gear = Sprite::createWithSpriteFrameName("gear");
    auto gearp = Sprite::createWithSpriteFrameName("gear-pressed");
    auto settingsItem = MenuItemSprite::create(gear, gearp, CC_CALLBACK_1(SceneMenu::openSettings, this));
    
    normal = Sprite::createWithSpriteFrameName("pi-button");
    normal = Sprite::createWithSpriteFrameName("pi-pressed");
    auto piItem = MenuItemSprite::create(normal, normal, [=](Ref* s) {
        auto scene = AltScene::create();
        auto transition = TransitionSlideInB::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    menu = Menu::create(piItem, storeItem, settingsItem, nullptr);
    menu->alignItemsHorizontally();
    menu->setPosition(Vec2(size.width /2, closeItem->getContentSize().height));
    addChild(menu);
    
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
        
    if (Config::getInstance()->getAudio()) {
        _audioID = AudioEngine::play2d("Bonkers-for-Arcades.mp3", true, 1, &_audioProfile);
    }
    
    if(_audioID != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }
    
    _score.setMaxLevel((int)_files.size());
    _chooser->setValue(1);
    _chooser->setMinValue(1);
    _chooser->setMaxValue(MIN(_score.getMaxLevel(), (int) _files.size()));
    
    _chooser->setPosition(Vec2(size.width / 2, size.height / 2 - 180));
    _chooser->setOnValueChangeCallback(CC_CALLBACK_1(SceneMenu::onValueChange, this));
    addChild(_chooser);
    _score.loadAllLevels();
    onValueChange(1);
    
    downloadLevel();
    
#ifdef CHEAT
    Bank::getInstance()->alterfood(50);
    Bank::getInstance()->alterstars(20);
#endif
    return true;
}

void SceneMenu::openSettings(cocos2d::Ref* pSender)
{
    auto scene = SettingsScene::create();
    auto transition = TransitionSlideInL::create(1, scene);
    Director::getInstance()->replaceScene(transition);
}

void SceneMenu::openStore(cocos2d::Ref* pSender)
{
//    AudioEngine::stopAll();
    auto scene = StoreScene::create();
    auto transition = TransitionSlideInL::create(1, scene);
    Director::getInstance()->replaceScene(transition);
}

void SceneMenu::downloadLevel()
{
    // This shows how to download a file from any url and store it locally
    return;
    
    auto name = "level10.tmx";
    auto url = "https://raw.githubusercontent.com/FrSanchez/snake/main/Resources/level10.tmx";
    char path[256];
    sprintf(path, "%slevel10.tmx", FileUtils::getInstance()->getWritablePath().c_str());
    downloader->createDownloadFileTask(url, path, name);
    downloader->onFileTaskSuccess = ([] (const network::DownloadTask& task) {
        log("downloader task success: %s", task.identifier.c_str());
        log("storage path %s", task.storagePath.c_str());
    });
    downloader->onTaskError = ([] (const network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr) {
        log("downloader task failed : %s, identifier(%s) error code(%d), internal error code(%d) desc(%s)"
            , task.requestURL.c_str()
            , task.identifier.c_str()
            , errorCode
            , errorCodeInternal
            , errorStr.c_str());
    });
}


void SceneMenu::resetScores()
{
    auto alert = ModalMessageBox::create();
    alert->setString("All the scores and achievements will reset to zero!");
    alert->setTag(0x123);
    alert->addButton("OK", 30, [=](Ref* pSender) {
        alert->ClosePopup();
        _score.reset();
        Bank::getInstance()->reset();
        Config::getInstance()->reset();
    });
    alert->addButton("Cancel", 30,  [=](Ref* pSender) {
        alert->ClosePopup();
    });
    
    addChild(alert);
}

void SceneMenu::onValueChange(int level)
{
    int score = _score.getMaxScore(level);
    auto score_str = StringUtils::format("SCORE: %d", score);
    auto label = getChildByTag<Label*>(LABEL_HIGH_SCORE);
    if (!label) {
        addChild(label = ScoreLabel::createHS());
    }
    label->setString(score_str);
    
    auto accuracy = _score.getAccuracy(level);
    label = getChildByTag<Label*>(LABEL_PERCENTAGE);
    auto pecent_str = StringUtils::format("FOOD: %.0f%%", accuracy);
    if (!label) {
        addChild(label = ScoreLabel::createPct());
    }
    label->setString(pecent_str);
}

std::vector<std::string> SceneMenu::findLevels()
{
    auto sharedFileUtils = FileUtils::getInstance();
    int level = 0;
    _files.clear();
    bool isExist = false;
    do {
        level++;
        auto levelFile = StringUtils::format("level%d.tmx", level);
        isExist = sharedFileUtils->isFileExist(levelFile);
        if (isExist) {
            _files.push_back(levelFile);
        }
    } while(isExist);
    CCLOG("Max level: %d", level - 1);
    return _files;
}


void SceneMenu::initUnityAdsFunc()
{
    const char* gameId = "4034261"; // for Android

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    gameId = "4034260";
#endif
    
    UnityAdsInit(gameId, true);
    log("[UnityAds cpp] version %s", UnityAdsGetVersion().c_str());
}

void SceneMenu::showUnityAdsFunc(Ref* pSender)
{
    const char* zoneString = "Interstitial_iOS";
    AudioEngine::stopAll();
    if(UnityAdsIsReady(zoneString)) {
//        UnityAdsShow(zoneString);
        rewardPlayer(zoneString);
    } else {
        CCLOG("[UnityAds cpp test] yet cannot show");
    }
}

void SceneMenu::rewardPlayer(const char *placementId)
{
    CCLOG("[UnityAds cpp test] rewarded");
    const char* targetStr = "Rewarded_iOS";
    if(strcmp(placementId, targetStr) == 0){
//        if(titleLabel){
//            const char* text = "Congrats!";
//            titleLabel->setString(text);
//        }
    }
    if(strcmp(placementId, "Interstitial_iOS") == 0) {
        auto level = _chooser->getValue();
        startGame(level, _files.at(level - 1));
    }
}
