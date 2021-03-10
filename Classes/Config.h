//
//  Config.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#ifndef Config_hpp
#define Config_hpp

class Config
{
private:
    bool _showPurchase = true;
    
protected:
    Config();

public:
    bool getShowPurchase() { return _showPurchase; }
    void setShowPurchase(bool value) { _showPurchase = value; }
};

#endif /* Config_hpp */
