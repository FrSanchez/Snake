//
//  SceneMenu.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#ifndef SceneMenu_hpp
#define SceneMenu_hpp


#include "Scenes/BaseScene.h"
#include "network/CCDownloader.h"
#include "UI/Chooser.h"
#include "Bank.h"
#include "UnityReward.h"
#include "SceneMenu.h"

class SceneMenu : public BaseScene
{
private:
    float _extent = 20;
    float _magnitude = 0;
    int _audioID;
    cocos2d::Vec2 _direction = cocos2d::Vec2(0.8, -1);
    cocos2d::Label* label;
    float _dt;
    int highScore = -1;

    Chooser* _chooser;
    std::vector<std::string> _files;
    std::string _levelFile;
    long _file;
    std::vector<std::string> findLevels();
    Score _score;
    cocos2d::AudioProfile _audioProfile;
    
    std::unique_ptr<cocos2d::network::Downloader> downloader;
    
    void onValueChange(int);
    void resetScores();

public:
    SceneMenu();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void startGame(int level, std::string levelFile);

    CREATE_FUNC(SceneMenu);
    
    void downloadLevel();
    void openStore(cocos2d::Ref* pSender);
    void openSettings(cocos2d::Ref* pSender);   
    const std::string getExitButtonName() {  return "exit"; }
    const std::string getExitPressedButtonName() {  return "exit_pressed"; }
    
    // Unity Ads
    void rewardPlayer(const char *placementId);
    void initUnityAdsFunc();
    void showUnityAdsFunc(cocos2d::Ref* pSender);
};
#endif /* SceneMenu_hpp */
