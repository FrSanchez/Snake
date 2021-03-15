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
    bool _audio = true;
    bool _effects = true;
    time_t nextAd = 0;
    Config();

public:
    bool getShowPurchase() { return _showPurchase; }
    bool getAudio() { return _audio; }
    bool getEffects() { return _effects; }
    time_t getNextAd() { return nextAd; }

    void setShowPurchase(bool value);
    void setAudio(bool audio) { _audio = audio; setDirty(); }
    void setEffects(bool effects) { _effects = effects; setDirty(); }
    void setNextAd(time_t value) { nextAd = value; setDirty(); }

    static Config* getInstance();
};

#endif /* Config_hpp */
