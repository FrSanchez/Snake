//
//  Bank.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/8/21.
//

#include "Bank.h"

USING_NS_CC;

static Bank *s_SharedBank = nullptr;

Bank::Bank() : BaseAutoSave("bank", this, sizeof(Bank))
{
    
}

Bank* Bank::getInstance()
{
    if (!s_SharedBank)
    {
        s_SharedBank = new (std::nothrow) Bank;
        CCASSERT(s_SharedBank, "FATAL: Not enough memory");
        s_SharedBank->init();
    }

    return s_SharedBank;
}
