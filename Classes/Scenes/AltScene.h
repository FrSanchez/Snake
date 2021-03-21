//
//  AltScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/15/21.
//

#ifndef AltScene_hpp
#define AltScene_hpp

#include "Gestures/GestureRecognizer.h"
#include "Gestures/PinchGestureRecognizer.h"
#include "Gestures/TapGestureRecognizer.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Gestures/LongPressGestureRecognizer.h"
#include "Gestures/PanGestureRecognizer.h"

#include "GameElements/SnakeHead.h"

#include "GameElements/Fruit.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "UnityReward.h"
#include "GrowByAction.h"
#include "GameElements/GameScoreLabel.h"
#include "GameElements/Arena.h"

class AltScene : public cocos2d::Scene, public UnityReward
{
private:
    SnakeHead* _head;
    bool _debugDraw = false;
    int _foodAdded;
    int _foodEaten;

    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
public:
    bool init();
    void rewardPlayer(const char *placementId);
    void closeScene(cocos2d::Ref *pSender = nullptr);
    void addFruit(float dt);
    cocos2d::Node* findNode(cocos2d::Node* a, cocos2d::Node* b, int tag);
    void eat(Fruit* fruit);
    void crash();
    void isDead();
    
    CREATE_FUNC(AltScene);
};

#endif /* AltScene_hpp */
