//
//  StoreScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "StoreScene.h"
#include "SceneMenu.h"
#include "ScoreLabel.h"

USING_NS_CC;

bool StoreScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto size = Director::getInstance()->getVisibleSize();;
    auto menu = Menu::create();
    
    auto bg = Sprite::create("storebg.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);

//    _store = new Store();
//    for (unsigned i=0; i<_store->getItems().size(); i++)
//    {
//        auto item = _store->getItems().at(i);
//        auto str = StringUtils::format("%s: %.0f", item.description.c_str(), item.price);
//        auto label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 32);
//        auto mItem = MenuItemLabel::create(label, [=](Ref* s){
//            auto itemId = ((Node*)s)->getTag();
//            _store->purchase(itemId);
//        });
//        mItem->setTag(item.id);
//        menu->addChild(mItem);
//    }
//    menu->alignItemsVertically();
//    menu->setPosition(Vec2(size.width/2, size.height/2));
//    addChild(menu);
//
//
    auto normal = Sprite::createWithSpriteFrameName("exit");
    auto select = Sprite::createWithSpriteFrameName("exit_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  {        
        auto scene = SceneMenu::create();
        auto transition = TransitionFlipX::create(1, scene, TransitionScene::Orientation::RIGHT_OVER);
        Director::getInstance()->replaceScene(transition);
    });

    float x = size.width - closeItem->getContentSize().width ;
    float y = closeItem->getContentSize().height;
    closeItem->setPosition(Vec2(x,y));

    
    normal = Sprite::createWithSpriteFrameName("fruit-store");
    auto foodItem = MenuItemSprite::create(normal, normal, [=](Ref *pSender)  {
        log("show desc for food");
    });
    foodItem->setPosition(Vec2(size.width / 4, size.height/2));

    normal = Sprite::createWithSpriteFrameName("star");
    normal->setScale(0.7);
    auto starItem = MenuItemSprite::create(normal, normal, [=](Ref *pSender) {
        log("Show desc for stars");
    });
    starItem->setPosition(Vec2(size.width * 3/5, size.height/2));
    

    // create menu, it's an autorelease object
    menu = Menu::create(closeItem, foodItem, starItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    Bank bank;
    auto str = StringUtils::format("%d", bank.getFood());
    
    auto label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 76, Size::ZERO, TextHAlignment::LEFT);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(size.width / 4 + normal->getContentSize().width, size.height / 2));
    label->setTextColor(Color4B::ORANGE);
    label->enableShadow();
    addChild(label);
        
    return true;
}
