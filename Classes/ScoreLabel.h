//
//  ScoreLabel.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/8/21.
//

#ifndef ScoreLabel_hpp
#define ScoreLabel_hpp

#include "cocos2d.h"

#define LABEL_HIGH_SCORE    0xf1
#define LABEL_PERCENTAGE    0xf2

class ScoreLabel: cocos2d::Label
{
public:
    static Label* createHS();
    static Label* createPct();
    
private:
    static Label* _create(cocos2d::Vec2 pos, cocos2d::Vec2 anchorPoint, cocos2d::TextHAlignment alignment, const std::string& text);
};

#endif /* ScoreLabel_hpp */
