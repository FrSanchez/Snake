//
//  BouncyPowerup.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/6/21.
//

#ifndef BouncyPowerup_hpp
#define BouncyPowerup_hpp

#include "PopupNode.h"
#include "DPad.h"

class BouncyPowerup : public PopupNode
{
private:
    cocos2d::ccMenuCallback _onCancelCallback;
    DPad::padCallback _onSelectionCallback;
public:
    bool init();
    CREATE_FUNC(BouncyPowerup);
    void onDpad(int);
    void setOnCancel(const cocos2d::ccMenuCallback& callback);
    void setOnSelection(const DPad::padCallback& callback) { _onSelectionCallback = callback; }

protected:
    std::string getBackgroundFrameName() { return "powerupbg";}
};

#endif /* BouncyPowerup_hpp */
