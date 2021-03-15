//
//  LostModalBox.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/13/21.
//

#ifndef LostModalBox_hpp
#define LostModalBox_hpp

#include "PopupNode.h"
#include "Bank.h"
#include "Score.h"
#include "ui/CocosGUI.h"
#include <string>


class LostModalBox : public PopupNode
{
private:

    cocos2d::MenuItem* _home;
    cocos2d::MenuItem* _repeat;
    
public:
    bool init();
    void setOnHome(const cocos2d::ccMenuCallback& callback) ;
    void setOnRepeat(const cocos2d::ccMenuCallback& callback);
    CREATE_FUNC(LostModalBox);
};

#endif /* LostModalBox_hpp */
