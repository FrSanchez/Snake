//
//  BaseAutoSave.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/11/21.
//

#include "BaseAutoSave.h"

USING_NS_CC;

BaseAutoSave::BaseAutoSave(const char* key, void* dst, ssize_t size) :
_dirty(false),
_key(key),
_data(dst),
_size(size)
{
    
}

bool BaseAutoSave::init()
{
    Data data = UserDefault::getInstance()->getDataForKey(_key);
    log("%lu %lu", sizeof(this), sizeof(BaseAutoSave));
    auto temp = malloc(sizeof(BaseAutoSave));
    memcpy(temp, this, sizeof(BaseAutoSave));
    if (!data.isNull() && data.getSize() > 0 ) {
        memcpy(_data, data.getBytes(), data.getSize());
    }
    memcpy(this, temp, sizeof(BaseAutoSave));
    free(temp);
    _dirty = false;
 
    return true;
}

void BaseAutoSave::save()
{
    if (!_dirty) {
        return;
    }
    _dirty = false;
    Data data;
    data.copy((unsigned char*) _data, _size);
    UserDefault::getInstance()->setDataForKey(_key, data);
}

void BaseAutoSave::reset()
{
    auto temp = malloc(sizeof(BaseAutoSave));
    memcpy(temp, this, sizeof(BaseAutoSave));
    memset(_data, 0, _size);
    memcpy(this, temp, sizeof(BaseAutoSave));
    free(temp);
    save();
}

void BaseAutoSave::setDirty()
{
    _dirty = true;
    save();
}
