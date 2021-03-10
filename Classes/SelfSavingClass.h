//
//  SelfSavingBaseClass.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#ifndef SelfSavingBaseClass_hpp
#define SelfSavingBaseClass_hpp

#include "cocos2d.h"

USING_NS_CC;

template <class T>
class SelfSavingClass
{
private:
    bool _dirty;
    const char* _key;
    T* _data;
    
    void setDirty() { _dirty = true; }
    void load() {
        Data data = UserDefault::getInstance()->getDataForKey(_key);
        if (!data.isNull() && sizeof(T) > 0 ) {
            memcpy(_data, data.getBytes(), data.getSize());
        }
        _dirty = false;
    }

public:
    SelfSavingClass() :
    _dirty(false) ,
    _key(typeid(T).name())
    {
        _data = new T();
        load();
    }
    
    void save() {
        _dirty = false;
        Data data;
        data.copy((unsigned char*) _data, sizeof(T));
        UserDefault::getInstance()->setDataForKey(_key, data);
    }
    ~SelfSavingClass() { save(); }
    T* getData() { _dirty = true; return _data; }
};


#endif /* SelfSavingBaseClass_hpp */
