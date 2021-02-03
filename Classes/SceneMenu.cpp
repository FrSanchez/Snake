//
//  SceneMenu.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneMenu.h"
#include "SceneGame.h"
#include "audio/include/AudioEngine.h"


USING_NS_CC;

Scene* SceneMenu::createScene()
{
    return SceneMenu::create();
}

void SceneMenu::startGame()
{
    auto audioId = AudioEngine::play2d("audio/select_001.ogg");
    if (audioId == AudioEngine::INVALID_AUDIO_ID) {
        log("Failed to play select_001.ogg");
    }
    auto scene = SceneGame::createScene();
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
    
    auto bg = Sprite::create("background.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    auto label = Label::createWithTTF("Play", "Arcade.ttf", 180);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    auto black = Color4B( Color4B::BLACK);
    auto grey = Color4B(128, 128, 128, 128);
    auto blue = Color4B(88, 157, 214, 255);
    label->setTextColor(blue);
    this->addChild(label);
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           [=](Ref *pSender)  { Director::getInstance()->end();
        } );

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        log("'CloseNormal.png' and 'CloseSelected.png'");
        return false;
    }
    else
    {
        float x = size.width - closeItem->getContentSize().width/2;
        float y = closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    float t = 2.0f;
    auto func = CallFunc::create([&]() {
        _magnitude = 0.1 * _extent;
        Size offset = _direction;
        offset.width *= _magnitude;
        offset.height *= _magnitude;
        grey.a = (uint8_t) (128 * 0.1);
        label->enableShadow(grey, offset, 50);
    });
    auto delay = DelayTime::create(0.1);
    auto seq = Sequence::create(func, delay, nullptr);
//    label->runAction(RepeatForever::create(seq));
    
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        log("Key with keycode %d pressed", keyCode);
        startGame();
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){
        startGame();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

// Implementation of the keyboard event callback function prototype
void SceneMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SceneMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}
