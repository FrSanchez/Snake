//
//  MoveBarBy.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#ifndef MoveBarBy_hpp
#define MoveBarBy_hpp

#include "cocos2d.h"

class MoveBarBy : public cocos2d::ActionInterval
{
public:
    /**
     * Creates the action.
     *
     * @param duration Duration time, in seconds.
     * @param deltaPercent The detal to fchange the percent bar
     * @return An autoreleased MoveBy object.
     */
    static MoveBarBy* create(float duration, float deltaPercent);
   

    //
    // Overrides
    //
    virtual MoveBarBy* clone() const override;
    virtual MoveBarBy* reverse() const  override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    MoveBarBy(){}
    virtual ~MoveBarBy() {}

    /** initializes the action */
    bool initWithDuration(float duration, float deltaPercent);

protected:
    float _positionDelta;
    float _startPosition;
    float _previousPosition;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveBarBy);
};

#endif /* MoveBarBy_hpp */
