//
//  BaseScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/17/21.
//

#include "BaseScene.h"
#include "Config.h"

USING_NS_CC;

bool BaseScene::init(const std::string& bgFilename)
{
    if (!Scene::init())
    {
        return false;
    }
    auto size = Director::getInstance()->getVisibleSize();
    
    auto bg = Sprite::create(bgFilename);
    this->addChild(bg);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    
    auto normal = Sprite::createWithSpriteFrameName(getExitButtonName());
    auto select = Sprite::createWithSpriteFrameName(getExitPressedButtonName());
    auto closeItem = MenuItemSprite::create(normal, select, CC_CALLBACK_1(BaseScene::closeScene, this));
    
    float x = size.width - closeItem->getContentSize().width ;
    float y = closeItem->getContentSize().height;
    closeItem->setPosition(Vec2(x,y));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    return true;
}

void BaseScene::closeScene(Ref* s)
{
    auto scene = getNextScene(); //SceneMenu::create();
    if (scene != nullptr) {
        auto transition = TransitionSlideInR::create(1, scene);
        Director::getInstance()->replaceScene(transition);
        Config::getInstance()->save();
    }
    else {
        Director::getInstance()->end();
    }
}

void BaseScene::rewardPlayer(const char *placementId)
{
    log("BaseScene: uninmplemented rewardPlayer for %s", placementId);
}
