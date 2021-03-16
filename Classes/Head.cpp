//
//  Head.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#include "Head.h"

USING_NS_CC;

bool Head::init()
{
    if (!Sprite::initWithSpriteFrameName("head"))
    {
        return false;
    }
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
        
    setRotation(_angle);
    Vec2 t = Vec2::forAngle(CC_DEGREES_TO_RADIANS(_angle));
    pos += t * dt * _speed;
    if (pos.x > 660) {
        pos.x = 0;
    }
    if (pos.x < 0) {
        pos.x = 660;
    }
    setPosition(pos);
    float angle = _angle;
    
    Sprite* prevBodyPart = this;
    for(const auto curBodyPart : bodyParts)
    {
        auto currentPos = curBodyPart->getPosition();
        auto dist = pos.getDistance(currentPos);
        float angle = curBodyPart->getRotation();
    
        Vec2 newPos = pos;
        
        float T = dt * dist / _minDistance * _speed;
        if (T > 0.5) { T= 0.5f; }
        currentPos.smooth(newPos, dt, T);
        
        curBodyPart->setPosition(currentPos);
        MathUtil::smooth(&angle, prevBodyPart->getRotation(), dt, T);
        curBodyPart->setRotation(angle);
        
        pos = curBodyPart->getPosition();
        prevBodyPart = curBodyPart;
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
    auto newPart = Sprite::createWithSpriteFrameName("body");
    Vec2 pos = getPosition();
    float angle = _angle;
    if (bodyParts.size() > 0) {
        pos = bodyParts.back()->getPosition();
        angle = bodyParts.back()->getRotation();
    }
    getParent()->addChild(newPart);
    newPart->setPosition(pos);
    newPart->setRotation(angle);
    newPart->setTag(0x11);
    bodyParts.pushBack(newPart);
}
