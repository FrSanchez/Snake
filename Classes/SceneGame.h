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
    virtual bool init(std::string);
    
    void closeScene(cocos2d::Ref* pSender);

    void labelCallback(cocos2d::Label* label);
    // implement the "static create()" method manually
    static cocos2d::Scene* createWithFile(std::string);
    
private:
    Snake snake = Snake(0,0);
    cocos2d::Size tileSize;
    cocos2d::Sprite* apple;
    Node* headSprite = nullptr;
    std::vector<cocos2d::Sprite *> body;
    cocos2d::Vec2 food = cocos2d::Vec2::ZERO;
    int lastFood = 0;
    float snakeSpeed = 0.3f;
    int _audioId;
    cocos2d::Label* scoreLabel;
    cocos2d::Label* posLabel;
    int score = 0;
    cocos2d::TMXLayer* layer;
    cocos2d::TMXTiledMap* _map;

    void updateTimer(float dt);
    
    void initBody();
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void addFood(float dt);
    void eat();
    void collide();
    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void onDpad(int);
    cocos2d::Vec2 calcViewPointCenter();
};

#endif /* SceneGame_hpp */
