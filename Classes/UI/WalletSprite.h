//
//  WalletSprite.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/9/21.
//

#ifndef WalletSprite_hpp
#define WalletSprite_hpp

#include "cocos2d.h"

class WalletSprite : public cocos2d::Sprite
{
private:
    int _food;
    int _stars;
    void deductHelper(int qty, int startValue, int tagId, const std::string &key, std::function<void()> onDone);
public:
    bool init();
    CREATE_FUNC(WalletSprite);
    void deductFood(int qty, std::function<void()> onDone = nullptr);
    void deductStars(int qty, std::function<void()> onDone = nullptr);
};

#endif /* WalletSprite_hpp */
