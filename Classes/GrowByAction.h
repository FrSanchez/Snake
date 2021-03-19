//
//  GrowToAction.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#ifndef GrowToAction_hpp
#define GrowToAction_hpp

#include "GameElements/SnakeHead.h"
#include "cocos2d.h"

class GrowByAction : public cocos2d::ActionInterval
{
private:
    std::function<int(void)> _readIntFunc = nullptr;
    std::function<void(int)> _writeIntFunc = nullptr;
    
public:
    /**
     * Creates the action.
     *
     * @param duration Duration time, in seconds.
     * @param deltaPercent The detal to fchange the percent bar
     * @return An autoreleased MoveBy object.
     */
    static GrowByAction* create(float duration, int deltaPercent,
                                const std::function<int(void)> readIntFunc,
                                const std::function<void(int)> writeIntFunc);
   

    //
    // Overrides
    //
    GrowByAction* clone() const override;
    GrowByAction* reverse() const  override;
    void startWithTarget(cocos2d::Node *target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    GrowByAction(const std::function<int(void)> readIntFunc,
                 const std::function<void(int)> writeIntFunc);
    virtual ~GrowByAction() {}

    /** initializes the action */
    bool initWithDuration(float duration, int deltaPercent);

protected:
    int _positionDelta;
    int _startPosition;
    int _previousPosition;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(GrowByAction);
};


#endif /* GrowToAction_hpp */
