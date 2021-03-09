//
//  StoreScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef StoreScene_hpp
#define StoreScene_hpp

#include "Store.h"
#include <cocos2d.h>

class StoreScene : public cocos2d::Scene
{
private:
    Store* _store;
public:
    bool init();
    CREATE_FUNC(StoreScene);
};

#endif /* StoreScene_hpp */
