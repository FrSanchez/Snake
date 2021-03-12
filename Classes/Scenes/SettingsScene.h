//
//  SettingsScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/12/21.
//

#ifndef SettingsScene_hpp
#define SettingsScene_hpp

#include "Config.h"
#include "UI/IconButton.h"
#include "cocos2d.h"

class SettingsScene : public cocos2d::Scene
{
private:
    void onMusic(Ref* s);
    void onEffects(Ref* s);
    void onRate(Ref* s);
    void onGameplay(Ref* s);
    void onTheme(Ref* s);
    void onContactUs(Ref* s);
    void onCheckbox(Ref *s);
    void setMusicIcon();
    void setEffectsIcon();
    void setWarnIcon();

public:
    bool init();
    CREATE_FUNC(SettingsScene);
};

#endif /* SettingsScene_hpp */
