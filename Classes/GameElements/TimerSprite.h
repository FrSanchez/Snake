//
//  Timer.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/15/21.
//

#ifndef Timer_hpp
#define Timer_hpp

#include "cocos2d.h"

#define TIMER_TAG 0x31

class TimerSprite : public cocos2d::Node
{
private:
    std::function<void()> _callback;
public:
    TimerSprite();
    bool init();
    void activate(float duration);
    void stop();
    void setOnTimerEndCallback(const std::function<void()>& callback ) { _callback = callback; }
    CREATE_FUNC(TimerSprite);
    
private:
};


#endif /* Timer_hpp */
