//
//  TrophyModalBox.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/6/21.
//

#ifndef TrophyModalBox_hpp
#define TrophyModalBox_hpp

#include "PopupNode.h"
#include "Bank.h"
#include "Score.h"
#include "ui/CocosGUI.h"
#include <string>

typedef std::function<void(const int)> boxCallback;

class TrophyModalBox : public PopupNode
{
private:
    constexpr static const float offX[] = { -100, 0, 100 };
    constexpr static const float offY[] = { 125, 150, 125 };

    cocos2d::Menu* menu;
    std::function<void(const int)> callback;
    int _target = 0;
    int _eaten = 0;
    Score _score;
    int _level;
    float _pctValue;
    
public:
    bool init();
    void addButton(const std::string& text, const std::string &pressed, float fontSize, const cocos2d::ccMenuCallback& callback);
    void setLevel(int level);
    void addStar(int star);
    void addFood(int qty = 0);
    void setTarget(int value) { _target = value; }
    void setEaten(int value) { _eaten = value; }
    void start();
    void setScore(Score score) { _score = score; }
    void showPct(float dt);
    CREATE_FUNC(TrophyModalBox);
};

#endif /* TrophyModalBox_hpp */
