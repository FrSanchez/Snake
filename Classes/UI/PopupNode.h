//
//  PopupNode.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/13/21.
//

#ifndef PopupNode_hpp
#define PopupNode_hpp

#include "cocos2d.h"

class PopupNode : public cocos2d::Sprite
{
public:
    CREATE_FUNC(PopupNode);
    virtual bool init();    // subclass should have access to call it
    void ClosePopup();

private:
    void BlockPassingTouch();
    
protected:
    virtual std::string getBackgroundFrameName();

};

#endif /* PopupNode_hpp */
