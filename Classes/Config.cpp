//
//  Config.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#include "Config.h"
#include "cocos2d.h"

USING_NS_CC;

static Config *s_SharedConfig = nullptr;

Config::Config() : BaseAutoSave("config", this, sizeof(Config))
{
    
}

Config* Config::getInstance()
{
    if (!s_SharedConfig)
    {
        s_SharedConfig = new (std::nothrow) Config();
        CCASSERT(s_SharedConfig, "FATAL: Not enough memory");
        s_SharedConfig->init();
    }

    return s_SharedConfig;
}

void Config::setShowPurchase(bool value)
{
    _showPurchase = value;
    setDirty();
}


