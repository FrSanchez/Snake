//
//  ModalMessageBox.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/13/21.
//

#ifndef ModalMessageBox_hpp
#define ModalMessageBox_hpp

#include "PopupNode.h"
#include "ui/CocosGUI.h"
#include <string>

typedef std::function<void(const int)> boxCallback;

class ModalMessageBox : public PopupNode
{
private:
    std::vector<cocos2d::ui::Button*> buttons;
    cocos2d::Menu* menu;
    std::function<void(const int)> dirCallback;
    
public:
    bool init();
    void addButton(const std::string& text, float fontSize, const cocos2d::ccMenuCallback& callback);
    void buttonCall(Ref* sender);
    void setString(const std::string& text) ;
    CREATE_FUNC(ModalMessageBox);
    
};
#endif /* ModalMessageBox_hpp */
