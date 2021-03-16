//
//  AltScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#ifndef AltScene_hpp
#define AltScene_hpp

#include "Head.h"
#include "Gestures/GestureRecognizer.h"
#include "Gestures/PinchGestureRecognizer.h"
#include "Gestures/TapGestureRecognizer.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Gestures/LongPressGestureRecognizer.h"
#include "Gestures/PanGestureRecognizer.h"

#include "cocos2d.h"

class AltScene : public cocos2d::Scene
{
private:
    Head* _head;
public:
    void update(float dt);
    bool init();
    CREATE_FUNC(AltScene);
    
    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
};

#endif /* AltScene_hpp */
