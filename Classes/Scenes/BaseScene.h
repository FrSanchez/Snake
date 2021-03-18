//
//  BaseScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/17/21.
//
// Used for scenes started from the Main Menu

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "Config.h"
#include "UI/IconButton.h"
#include "UnityReward.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "Score.h"
#include <cocos2d.h>

class BaseScene : public cocos2d::Scene, public UnityReward
{
protected:
    virtual bool init(const std::string& bgFilename);
    virtual void closeScene(cocos2d::Ref *pSender = nullptr);
    void rewardPlayer(const char *placementId);
    virtual cocos2d::Scene* getNextScene() { return nullptr; }
    
    virtual const std::string getExitButtonName() {  return "repeat"; }
    virtual const std::string getExitPressedButtonName() {  return "repeat_pressed"; }
};

#endif /* BaseScene_hpp */
