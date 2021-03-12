//
//  IconButton.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/12/21.
//

#ifndef IconButton_hpp
#define IconButton_hpp

#include "cocos2d.h"


class IconButton : public cocos2d::MenuItemSprite
{
private:
    
public:
    bool init(const std::string &image, const std::string &text, const cocos2d::ccMenuCallback& callback);
    void setIcon(const std::string& image);
    void setText(const std::string &text);
    static IconButton* createWithIconAndText(const std::string &image, const std::string &text, const cocos2d::ccMenuCallback& callback);
};

#endif /* IconButton_hpp */
