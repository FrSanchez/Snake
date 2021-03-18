//
//  CompleteBarBy.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#include "MoveBarBy.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

MoveBarBy* MoveBarBy::create(float duration, float deltaPercent)
{
    MoveBarBy *ret = new (std::nothrow) MoveBarBy();
    
    if (ret && ret->initWithDuration(duration, deltaPercent))
    {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}


bool MoveBarBy::initWithDuration(float duration, float deltaPercent)
{
    bool ret = false;
    
    if (ActionInterval::initWithDuration(duration))
    {
        _positionDelta = deltaPercent;
        ret = true;
    }
    
    return ret;
}


MoveBarBy* MoveBarBy::clone() const
{
    // no copy constructor
    return MoveBarBy::create(_duration, _positionDelta);
}

void MoveBarBy::startWithTarget(Node *t)
{
    auto target = dynamic_cast<cocos2d::ui::LoadingBar*>(t);
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPercent();
}

MoveBarBy* MoveBarBy::reverse() const
{
    return MoveBarBy::create(_duration, -_positionDelta);
}

void MoveBarBy::update(float t)
{
    if (_target)
    {
        auto target = dynamic_cast<cocos2d::ui::LoadingBar*>(_target);
#if CC_ENABLE_STACKABLE_ACTIONS
        float currentPos = target->getPercent();
        float diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        float newPos =  _startPosition + (_positionDelta * t);
        target->setPercent(newPos);
        _previousPosition = newPos;
#else
        _target->setPercent(_startPosition + _positionDelta * t);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}
