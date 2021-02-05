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

static cocos2d::Size boardSize = cocos2d::Size(40, 30);

class SceneGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void closeScene(cocos2d::Ref* pSender);
    void update(float delta);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
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
    cocos2d::Sprite* leftButton;
    cocos2d::Sprite* rightButton;

    void updateTimer(float dt);
    
    void initBody();
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void addFood();
    void eat();
    void collide();
};

#endif /* SceneGame_hpp */
