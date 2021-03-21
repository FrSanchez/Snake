//
//  Arena.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/19/21.
//

#include "Arena.h"

USING_NS_CC;

bool Arena::init()
{
    if (!Node::init())
    {
        return false;
    }
    for (int i = 0; i < 21; i++)
    {
        auto img = Sprite::createWithSpriteFrameName("grass-dark");
        img->setPosition(Vec2(i * img->getContentSize().width, 2 * img->getContentSize().height));
        addChild(img);

        img = Sprite::createWithSpriteFrameName("grass-dark");
        img->setPosition(Vec2(i * img->getContentSize().width, img->getContentSize().height * 35));
        addChild(img);
    }
    
    for (int i = 2 ; i < 35; i++)
    {
        auto img = Sprite::createWithSpriteFrameName("grass-dark");
        img->setPosition(Vec2(0, img->getContentSize().height * i));
        addChild(img);

        img = Sprite::createWithSpriteFrameName("grass-dark");
        img->setPosition(Vec2(20 * img->getContentSize().width, img->getContentSize().height * i));
        addChild(img);
    }
    setContentSize(Size(32 * 22, 35 * 32));
    
    auto pb = PhysicsBody::create();
    PhysicsMaterial mat(PHYSICS_INFINITY, 1.0f, 0.0f);
    pb->addShape(PhysicsShapeBox::create(Size(32*22, 32), mat, Vec2(0, 35 * 16)));
    pb->addShape(PhysicsShapeBox::create(Size(32*22, 32), mat, Vec2(0, -31 * 16)));
    
    pb->addShape(PhysicsShapeBox::create(Size(32, 33*32), mat, Vec2(-22 * 16, 32)));
    pb->addShape(PhysicsShapeBox::create(Size(32, 33*32), mat, Vec2(18 * 16, 32)));

    pb->setGravityEnable(false);
    pb->setDynamic(false);
    pb->setCategoryBitmask(0x01);
    pb->setContactTestBitmask(0x0f);
    addComponent(pb);
    return true;
}
