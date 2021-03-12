//
//  Config.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#ifndef Config_hpp
#define Config_hpp

#include "BaseAutoSave.h"
#include <ctime>

class Config : public BaseAutoSave
{
private:
    bool _showPurchase = true;
    float volume = 1.0f;
    time_t nextAd = 0;
    Config();

public:
    bool getShowPurchase() { return _showPurchase; }
    void setShowPurchase(bool value); 
    static Config* getInstance();
};

#endif /* Config_hpp */
