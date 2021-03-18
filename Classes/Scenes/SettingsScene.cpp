//
//  SettingsScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/12/21.
//

#include "SettingsScene.h"
#include "SceneMenu.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool SettingsScene::init()
{
    if (!BaseScene::init("storebg.png"))
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();
    
    auto music = IconButton::createWithIconAndText("gear", "Music", CC_CALLBACK_1(SettingsScene::onMusic, this));
    music->setTag(0xa00);
    auto effects = IconButton::createWithIconAndText("gear", "Effects", CC_CALLBACK_1(SettingsScene::onEffects, this));
    effects->setTag(0xa01);
    auto warn = IconButton::createWithIconAndText("gear", "Warn on Purchase", CC_CALLBACK_1(SettingsScene::onCheckbox, this));
    warn->setTag(0xa02);
    auto eula = IconButton::createWithIconAndText("information", "License Agreement", CC_CALLBACK_1(SettingsScene::onEULA, this));
    
    auto menu = Menu::create(music, effects, warn, eula, nullptr);
    menu->setTag(0x100);
    addChild(menu);
    menu->alignItemsVertically();
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->setPosition(Vec2(size.width/2, size.height/2));
    
    setMusicIcon();
    setEffectsIcon();
    setWarnIcon();
    
    return true;
}

void SettingsScene::setMusicIcon()
{
    auto icon = (Config::getInstance()->getAudio() ? "musicOn" : "musicOff");
    auto button = dynamic_cast<IconButton*>((Node*)getChildByTag(0x100)->getChildByTag(0xa00));
    button->setIcon(icon);
}

void SettingsScene::setEffectsIcon()
{
    auto icon = Config::getInstance()->getEffects() ? "audioOn" : "audioOff";
    auto button = dynamic_cast<IconButton*>((Node*)getChildByTag(0x100)->getChildByTag(0xa01));
    button->setIcon(icon);
}

void SettingsScene::setWarnIcon()
{
    auto icon = Config::getInstance()->getShowPurchase()? "check_box_active" : "check_box_normal";
    auto button = dynamic_cast<IconButton*>((Node*)getChildByTag(0x100)->getChildByTag(0xa02));
    button->setIcon(icon);
}


void SettingsScene::onEULA(Ref *s)
{
    Application::getInstance()->openURL("https://sanchezparralabs.com/eula.html");
}


void SettingsScene::onMusic(Ref* s)
{
    auto audio = Config::getInstance()->getAudio();
    Config::getInstance()->setAudio(!audio);
    setMusicIcon();
    if (audio) {
        AudioEngine::pauseAll();
    } else {
        AudioEngine::resumeAll();
    }
}

void SettingsScene::onEffects(Ref *s)
{
    auto effects = Config::getInstance()->getEffects();
    Config::getInstance()->setEffects(!effects);
    setEffectsIcon();
}

void SettingsScene::onCheckbox(Ref* s)
{
    auto check = Config::getInstance()->getShowPurchase();
    Config::getInstance()->setShowPurchase(!check);
    setWarnIcon();
}
