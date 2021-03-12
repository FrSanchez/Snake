//
//  PauseButton.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/11/21.
//

#ifndef PauseButton_hpp
#define PauseButton_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PauseButton : public cocos2d::Node
{
private:
    cocos2d::ui::Button* _pause;
    cocos2d::ui::Button* _resume;
    
public:
    bool init();
    CREATE_FUNC(PauseButton);
};

#endif /* PauseButton_hpp */
