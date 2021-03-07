//
//  PopupNode.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/13/21.
//

#ifndef PopupNode_hpp
#define PopupNode_hpp

#include "cocos2d.h"

class PopupNode : public cocos2d::Node
{
public:
    CREATE_FUNC(PopupNode);
    virtual bool init();    // subclass should have access to call it
    void ClosePopup();
    void addChild(Node * child);

private:
    void BlockPassingTouch();
    void SetupUI();
    
protected:
    cocos2d::Node* _cover;

};

#endif /* PopupNode_hpp */
