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
    if (!Scene::init())
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();;
    
    auto bg = Sprite::create("storebg.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    auto music = IconButton::createWithIconAndText("gear", "Music", CC_CALLBACK_1(SettingsScene::onMusic, this));
    music->setTag(0xa00);
    auto effects = IconButton::createWithIconAndText("gear", "Effects", CC_CALLBACK_1(SettingsScene::onEffects, this));
    effects->setTag(0xa01);
    auto warn = IconButton::createWithIconAndText("gear", "Warn on Purchase", CC_CALLBACK_1(SettingsScene::onCheckbox, this));
    warn->setTag(0xa02);
    auto eula = IconButton::createWithIconAndText("information", "License Agreement", CC_CALLBACK_1(SettingsScene::onEULA, this));
//    auto rate;
//    auto gameplay;
//    auto theme;
//    auto contactUs;
    
    auto menu = Menu::create(music, effects, warn, eula, nullptr);
    menu->setTag(0x100);
    addChild(menu);
    menu->alignItemsVertically();
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->setPosition(Vec2(size.width/2, size.height/2));
    
    auto normal = Sprite::createWithSpriteFrameName("repeat");
    auto select = Sprite::createWithSpriteFrameName("repeat_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  {
        auto scene = SceneMenu::create();
        auto transition = TransitionSlideInR::create(1, scene);
        Director::getInstance()->replaceScene(transition);
        Config::getInstance()->save();
    });
    
    float x = size.width - closeItem->getContentSize().width ;
    float y = closeItem->getContentSize().height;
    closeItem->setPosition(Vec2(x,y));

    menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
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
