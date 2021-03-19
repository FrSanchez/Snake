//
//  ScoreLabel.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/19/21.
//

#ifndef GameScoreLabel_h
#define GameScoreLabel_h

#include "cocos2d.h"

#define SCORE_TAG 0xf10

class GameScoreLabel : public cocos2d::Label
{
private:
    int _value;
public:
    bool init();
    CREATE_FUNC(GameScoreLabel);
    
    int getValue() { return _value; }
    void setValue(int value);
};

#endif /* ScoreLabel_hpp */
