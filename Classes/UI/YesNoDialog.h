//
//  YesNoDialog.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#ifndef YesNoDialog_hpp
#define YesNoDialog_hpp

#include "PopupNode.h"
#include "ui/CocosGUI.h"

class YesNoDialog : public PopupNode
{
private:
    cocos2d::Menu* menu;
    YesNoDialog();
public:
    
    bool initWithTitle(const std::string &title, const std::string &text, const cocos2d::ccMenuCallback &onYes, const cocos2d::ccMenuCallback &onNo);
    static YesNoDialog* createWithTitle(const std::string &title, const std::string &text, const cocos2d::ccMenuCallback &onYes, const cocos2d::ccMenuCallback &onNo);
    void addButton(const std::string& text, float fontSize, const cocos2d::ccMenuCallback &callback);
protected:
    std::string getBackgroundFrameName() { return "powerupbg";}
};

#endif /* YesNoDialog_hpp */
