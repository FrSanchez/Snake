//
//  SceneMenu.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#ifndef SceneMenu_hpp
#define SceneMenu_hpp

#include "cocos2d.h"

class SceneMenu : public cocos2d::Scene
{
private:
    float _extent = 20;
    float _magnitude = 0;
    cocos2d::Size _direction = cocos2d::Size(0.8, -1);
    int _audioID;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void startGame();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMenu);

};
#endif /* SceneMenu_hpp */
