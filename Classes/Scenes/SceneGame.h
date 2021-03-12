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
#include "Score.h"

// Gestures
#include "Gestures/GestureRecognizer.h"
#include "Gestures/PinchGestureRecognizer.h"
#include "Gestures/TapGestureRecognizer.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Gestures/LongPressGestureRecognizer.h"
#include "Gestures/PanGestureRecognizer.h"
#include "UI/PowerUpIcon.h"
#include "Bank.h"

#include <string>

static cocos2d::Size boardSize = cocos2d::Size(40, 30);

class SceneGame : public cocos2d::Scene
{
public:
    virtual bool init(int, std::string);
    
    void closeScene(cocos2d::Ref* pSender);

    void labelCallback(cocos2d::Label* label);
    // implement the "static create()" method manually
    static cocos2d::Scene* createWithFile(int, std::string);
    
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
    cocos2d::TMXLayer* layer;
    cocos2d::TMXTiledMap* _map;
    int _level = 0;
    Score _score;
    int _foodEaten = 0;
    int _foodAdded = 0;
    bool _active;

    void updateTimer(float dt);
    void scoreByLiving(float dt);
    
    void initBody();
    
    void addFood(float dt);
    void eat();
    void collide();
    void enableSwipe();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void onDpad(int);
    cocos2d::Vec2 calcViewPointCenter();
    void checkForOpenLevel();
    void showFoodLocator(cocos2d::Vec2 foodPosition);
    void onTimerEnd();
    float getAngleDegrees(cocos2d::Vec2 from, cocos2d::Vec2 to);
    float getFoodTarget();
};

#endif /* SceneGame_hpp */
