//
//  GrowToAction.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#include "GrowByAction.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

GrowByAction::GrowByAction(const std::function<int(void)> readIntFunc,
             const std::function<void(int)> writeIntFunc) :
_readIntFunc(readIntFunc),
_writeIntFunc(writeIntFunc)
{
    
}

GrowByAction* GrowByAction::create(float duration, int deltaPercent,
                                   const std::function<int(void)> readIntFunc,
                                   const std::function<void(int)> writeIntFunc)
{
    GrowByAction *ret = new (std::nothrow) GrowByAction(readIntFunc, writeIntFunc);
    
    if (ret && ret->initWithDuration(duration, deltaPercent))
    {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}


bool GrowByAction::initWithDuration(float duration, int deltaPercent)
{
    bool ret = false;
    
    if (ActionInterval::initWithDuration(duration))
    {
        _positionDelta = deltaPercent;
        ret = true;
    }
    
    return ret;
}


GrowByAction* GrowByAction::clone() const
{
    // no copy constructor
    return GrowByAction::create(_duration, _positionDelta, _readIntFunc, _writeIntFunc);
}

void GrowByAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = _readIntFunc();
}

GrowByAction* GrowByAction::reverse() const
{
    return GrowByAction::create(_duration, -_positionDelta, _readIntFunc, _writeIntFunc);
}

void GrowByAction::update(float t)
{
    if (_target)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        float currentPos = _readIntFunc();
        float diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        float newPos =  _startPosition + (_positionDelta * t);
        _previousPosition = newPos;
        _writeIntFunc((int)newPos);
#else
        _target->setPosition3D(_startPosition + _positionDelta * t);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}
