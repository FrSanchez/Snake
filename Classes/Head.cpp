//
//  Head.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#include "Head.h"

USING_NS_CC;

Head::Head() :
_speed(100.0f),
_angle(90),
_minDistance( 0.1),
_tLim( 0.5f),
_dir(Vec2(0,1))
{
    
}

Head::~Head()
{
    
    bodyParts.clear();
}

bool Head::init()
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
    return true;
}

void Head::setAngle(float angle)
{
    _angle = angle;
    log("Angle: %f ", _angle);
}

void Head::update(float dt)
{
    Vec2 pos = getPosition();
    Vec2 tail = bodyParts.back()->getPosition();
        
    setRotation(_angle);
    Vec2 t = Vec2::forAngle(CC_DEGREES_TO_RADIANS(_angle));
    pos += t * dt * _speed;
    Vec2 deltaBody = Vec2(0,0);
    
    // jump to the other side of the screen
    if (tail.y <= 0 && _angle == 270) {
        deltaBody.y = 1200 - pos.y;
    }
    if (tail.x <= 0 && _angle == 180) {
        deltaBody.x = 680 - pos.x;
    }
    if (tail.x > 680 && _angle == 0) {
        deltaBody.x = -pos.x;
    }
    if (tail.y > 1200 && _angle == 90) {
        deltaBody.y = -pos.y;
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

void Head::addBodyPart()
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
