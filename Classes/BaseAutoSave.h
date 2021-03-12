//
//  BaseAutoSave.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/11/21.
//

#ifndef BaseAutoSave_hpp
#define BaseAutoSave_hpp

#include "cocos2d.h"

class BaseAutoSave
{
private:
    bool _dirty;
    const char* _key;
    void * _data;
    ssize_t _size;
    
protected:
    void setDirty() { _dirty = true; }
    
public:
    BaseAutoSave(const char* key, void * data, ssize_t size);
    ~BaseAutoSave();
    bool init();
    void save();
    void reset();
};

#endif /* BaseAutoSave_hpp */
