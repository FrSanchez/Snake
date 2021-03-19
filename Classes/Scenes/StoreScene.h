//
//  StoreScene.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#ifndef StoreScene_hpp
#define StoreScene_hpp

#include "Store.h"
#include "BaseScene.h"
#include "SceneMenu.h"
#include <cocos2d.h>

class StoreScene : public BaseScene
{
private:
    Store* _store;
public:
    bool init();
    void doPurchase(Ref* pSender);
    CREATE_FUNC(StoreScene);
    cocos2d::Scene* getNextScene() { SceneMenu::create(); }
    void closeScene(cocos2d::Ref *pSender = nullptr);
};

#endif /* StoreScene_hpp */
