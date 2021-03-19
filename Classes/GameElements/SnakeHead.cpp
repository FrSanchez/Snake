//
//  SnakeHead.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#include "SnakeHead.h"

USING_NS_CC;

SnakeHead::SnakeHead() :
_speed(100.0f),
_angle(90),
_minDistance( 0.1),
_tLim( 0.5f),
_dir(Vec2(0,1))
{
    
}

SnakeHead::~SnakeHead()
{
    
    bodyParts.clear();
}

bool SnakeHead::init()
{
    if (!Sprite::initWithSpriteFrameName("head"))
    {
        return false;
    }

    auto pb = PhysicsBody::createCircle(getContentSize().height / 2);
    pb->setDynamic(false);
    pb->setCategoryBitmask(0x02);
    pb->setContactTestBitmask(0x0f);
    addComponent(pb);
    scheduleUpdate();
    return true;
}

void SnakeHead::setAngle(float angle)
{
    _angle = angle;
}

void SnakeHead::update(float dt)
{
    static int split = 0;
    if (split > bodyParts.size()) {
        split = 0;
    }
    Vec2 pos = getPosition();
    Vec2 tail = bodyParts.back()->getPosition();
        
    setRotation(_angle);
    Vec2 t = Vec2::forAngle(CC_DEGREES_TO_RADIANS(_angle));
    pos += t * dt * _speed;
    Vec2 deltaBody = Vec2(0,0);
    
    // jump to the other side of the screen
    if (tail.y <= 0 && _angle == 270) {
        deltaBody.y = 1200 - pos.y;
        split = 1;
    }
    if (tail.x <= 0 && _angle == 180) {
        deltaBody.x = 680 - pos.x;
        split = 1;
    }
    if (tail.x > 680 && _angle == 0) {
        deltaBody.x = -pos.x;
        split = 1;
    }
    if (tail.y > 1200 && _angle == 90) {
        deltaBody.y = -pos.y;
        split = 1;
    }
    // endjump


    setPosition(pos + deltaBody);
    float angle = _angle;
    
    Sprite* prevBodyPart = this;
    int i = 0;
    for(const auto curBodyPart : bodyParts)
    {
        curBodyPart->setPosition(curBodyPart->getPosition() + deltaBody);
        auto currentPos = curBodyPart->getPosition();

        auto dist = pos.getDistance(currentPos);
        float angle = curBodyPart->getRotation();
    
        Vec2 newPos = prevBodyPart->getPosition();
        
        float T = dt * dist / _minDistance * _speed;
        if (T > _tLim) { T = _tLim; }
        currentPos.smooth(newPos, dt, T);
        
        curBodyPart->setPosition(currentPos);
        MathUtil::smooth(&angle, prevBodyPart->getRotation(), dt, T);
        curBodyPart->setRotation(angle);
        
        pos = curBodyPart->getPosition();
        prevBodyPart = curBodyPart;
        
        i++;
        if (i == 6) {
            curBodyPart->getPhysicsBody()->setContactTestBitmask(0x02);
        }
    }
}

/*
 public void Move()
     {

         float curspeed = speed;

         if (Input.GetKey(KeyCode.W))
             curspeed *= 2;

         bodyParts[0].Translate(bodyParts[0].forward * curspeed * Time.smoothDeltaTime, Space.World);

         if (Input.GetAxis("Horizontal") != 0)
             bodyParts[0].Rotate(Vector3.up * rotationSpeed * Time.deltaTime * Input.GetAxis("Horizontal"));

         for (int i = 1; i < bodyParts.Count; i++)
         {
             curBodyPart = bodyParts[i];
             PrevBodyPart = bodyParts[i - 1];
             dis = Vector3.Distance(PrevBodyPart.position,curBodyPart.position);
             Vector3 newpos = PrevBodyPart.position;
             newpos.y = bodyParts[0].position.y;
             float T = Time.deltaTime * dis / minDistance * curspeed;
             if (T > 0.5f)
                 T = 0.5f;
             curBodyPart.position = Vector3.Slerp(curBodyPart.position, newpos, T);
             curBodyPart.rotation = Quaternion.Slerp(curBodyPart.rotation, PrevBodyPart.rotation, T);

         }
     }


     public void AddBodyPart()
     {

         Transform newpart = (Instantiate (bodyprefabs, bodyParts[bodyParts.Count - 1].position, bodyParts[bodyParts.Count - 1].rotation) as GameObject).transform;

         newpart.SetParent(transform);

         bodyParts.Add(newpart);
     }
 */

void SnakeHead::addBodyPart()
{
    auto newBodyLink = Sprite::createWithSpriteFrameName("body");
    Vec2 pos = getPosition();
    float angle = _angle;
    
    getParent()->addChild(newBodyLink);
    newBodyLink->setPosition(pos);
    newBodyLink->setRotation(angle);
    newBodyLink->setTag(0x11);
    bodyParts.insert(0, newBodyLink);
    
    auto pb = PhysicsBody::createCircle(newBodyLink->getContentSize().height / 2);
    pb->setDynamic(false);
    pb->setCategoryBitmask(0x04);
    pb->setContactTestBitmask(0x00);
    newBodyLink->addComponent(pb);
}

void SnakeHead::grow(int size)
{
    for (int delta = size - getSize(); delta >0; delta--)
    {
        addBodyPart();
    }
}

void SnakeHead::die(const std::function<void()>& callback)
{
    auto shrink = ResizeTo::create(0.5, Size(10, 10));
    unscheduleUpdate();
    
    for(const auto curBodyPart : bodyParts)
    {
        auto remove = CallFunc::create([&, curBodyPart](){
            curBodyPart->removeFromParentAndCleanup(true);
        });

        auto seq = Sequence::create(shrink->clone(), remove, nullptr);
        curBodyPart->runAction(seq);
    }
    
    auto remove = CallFunc::create([&, callback](){
        this->removeFromParentAndCleanup(true);
        if (callback != nullptr)
        {
            callback();
        }
    });
    
    runAction(Sequence::create(shrink, remove, nullptr));

}
