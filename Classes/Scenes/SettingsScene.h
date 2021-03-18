//
//  SettingsScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/12/21.
//

#ifndef SettingsScene_hpp
#define SettingsScene_hpp

#include "BaseScene.h"
#include "SceneMenu.h"

class SettingsScene : public BaseScene
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
    void onEULA(Ref* s);

public:
    bool init();
    CREATE_FUNC(SettingsScene);
    cocos2d::Scene* getNextScene() { SceneMenu::create(); }
};

#endif /* SettingsScene_hpp */
