//
//  StoreScene.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "StoreScene.h"
#include "SceneMenu.h"
#include "UI/ScoreLabel.h"
#include "UI/WalletSprite.h"
#include "UI/YesNoDialog.h"
#include "UI/PowerUpIcon.h"
#include "Config.h"

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

    auto normal = Sprite::createWithSpriteFrameName("exit");
    auto select = Sprite::createWithSpriteFrameName("exit_pressed");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  {        
        auto scene = SceneMenu::create();
        auto transition = TransitionSlideInR::create(1, scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    float x = size.width - closeItem->getContentSize().width ;
    float y = closeItem->getContentSize().height;
    closeItem->setPosition(Vec2(x,y));


    // create menu, it's an autorelease object
    menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    auto wallet = WalletSprite::create();
    wallet->setTag(0x91);
    addChild(wallet);
    wallet->setPosition(Vec2(size.width/2, size.height - 128));
//    auto seq = Sequence::create(DelayTime::create(1), MoveTo::create(1, Vec2(size.width/2, size.height - 64)), nullptr);
//    labelBg->runAction(seq);

    menu = Menu::create();
    _store = new Store();
    for (unsigned i=0; i<_store->getItems().size(); i++)
    {
        auto item = _store->getItems().at(i);
        auto str = StringUtils::format("%s: %.0f", item.description.c_str(), item.price);
        auto label = Label::createWithTTF(str.c_str(), "Stick-Regular.ttf", 46);
        label->setTextColor(Color4B::ORANGE);
        auto mItem = MenuItemLabel::create(label, [&, item](Ref* s){
            if (Config::getInstance()->getShowPurchase())
            {
                auto dialog = YesNoDialog::createWithTitle("Purchase powerup", "All sales are final, are you sure?", CC_CALLBACK_1(StoreScene::doPurchase, this), nullptr);
                dialog->setTag(item.id);
                addChild(dialog);
            } else {
                doPurchase(s);
            }
        });
        mItem->setTag(item.id);
        menu->addChild(mItem);
    }
    menu->alignItemsVertically();
    menu->setPosition(Vec2(size.width/2, size.height/2));
    addChild(menu);
    
    menu->setPosition(Vec2(size.width / 2, size.height * 2 / 3));
    
    auto bouncy = PowerUpIcon::createWithImage("helmet");
    bouncy->setPosition(Vec2(size.width/2, 92));
    bouncy->setTag(0x21);
    addChild(bouncy);
    return true;
}

void StoreScene::doPurchase(Ref* s)
{
    auto wallet = (WalletSprite*) getChildByTag(0x91);
    auto itemId = ((Node*)s)->getTag();
    auto result = _store->purchase(itemId);
    if (result.success)
    {
        wallet->deductFood(result.paid, [&, result](){
            log("Food removed! , power up added %d", result.purchased);
            auto bouncy = (PowerUpIcon*)getChildByTag(0x21);
            bouncy->setCount(bouncy->getCount() + result.purchased);
        });
    }
}
