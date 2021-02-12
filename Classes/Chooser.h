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
    int _maxValue;
    int _minValue;
    int _value;
    void upDownCallback(cocos2d::Ref* pSender);
    bool init();
    std::function<void(int value)> _callback = nullptr;

public:
    CREATE_FUNC(Chooser);
    ~Chooser();
    int getValue() { return _value; }
    void setMaxValue(int value);
    void setMinValue(int value) { _minValue = value; }
    void setValue(int value) { _value = value; }
    void setOnValueChangeCallback(const std::function<void(int value)>& callback ) { _callback = callback; }
};

#endif /* Chooser_hpp */
