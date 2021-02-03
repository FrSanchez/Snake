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
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SceneGame);
    
private:
    Snake snake = Snake(boardSize.width, boardSize.height);
    cocos2d::Sprite* apple;
    Node* headSprite = nullptr;
    std::vector<cocos2d::Sprite *> body;
    cocos2d::Vec2 food = cocos2d::Vec2::ZERO;
    int lastFood = 0;
    float snakeSpeed = 0.5f;
    cocos2d::Vec2 corner;

    void updateTimer(float dt);
    
    void initBody();
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void addFood();
    void eat();
    void collide();
};

#endif /* SceneGame_hpp */
