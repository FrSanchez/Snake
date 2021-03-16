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
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

class AltScene : public cocos2d::Scene
{
private:
    Head* _head;
    bool _debugDraw = false;

    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
public:
    void update(float dt);
    bool init();
    CREATE_FUNC(AltScene);
    
};

#endif /* AltScene_hpp */
