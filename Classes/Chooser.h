//
//  Chooser.hpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/11/21.
//

#ifndef Chooser_hpp
#define Chooser_hpp

#include <cocos2d.h>

class CC_DLL Chooser : public cocos2d::Node
{  
private:
    long _maxValue;
    long _minValue;
    long _value;
    void upDownCallback(cocos2d::Ref* pSender);
    bool init();
    
public:
    
    CREATE_FUNC(Chooser);
    long getValue() { return _value; }
    void setMaxValue(long value) { _maxValue = value; }
    void setMinValue(long value) { _minValue = value; }
    void setValue(long value) { _value = value; }
};

#endif /* Chooser_hpp */
