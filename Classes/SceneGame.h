//
//  SceneGame.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#ifndef SceneGame_hpp
#define SceneGame_hpp

#include "cocos2d.h"
#include "Snake.h"
// Gestures
#include "Gestures/GestureRecognizer.h"
#include "Gestures/PinchGestureRecognizer.h"
#include "Gestures/TapGestureRecognizer.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Gestures/LongPressGestureRecognizer.h"
#include "Gestures/PanGestureRecognizer.h"

#include <string>

static cocos2d::Size boardSize = cocos2d::Size(40, 30);

class SceneGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void closeScene(cocos2d::Ref* pSender);
    void update(float delta);
    
    void labelCallback(cocos2d::Label* label);
    // implement the "static create()" method manually
    CREATE_FUNC(SceneGame);
    
private:
    Snake snake = Snake(0,0);
    cocos2d::Size tileSize;
    cocos2d::Sprite* apple;
    Node* headSprite = nullptr;
    std::vector<cocos2d::Sprite *> body;
    cocos2d::Vec2 food = cocos2d::Vec2::ZERO;
    int lastFood = 0;
    float snakeSpeed = 0.5f;
    cocos2d::Vec2 corner;
    int _audioId;
    cocos2d::Label* scoreLabel;
    cocos2d::Label* posLabel;
    int score = 0;
    cocos2d::TMXLayer* layer;

    void updateTimer(float dt);
    
    void initBody();
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void addFood();
    void eat();
    void collide();
    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void onDpad(int);
};

#endif /* SceneGame_hpp */
